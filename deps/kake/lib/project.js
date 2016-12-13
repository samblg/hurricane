'use strict';

const path = require('path');
const utils = {
    fs: require('./util/fs'),
    string: require('./util/string'),
    config: require('./util/config'),
    child_process: require('./util/child_process'),
    loggers: require('./util/loggers')
};

const template = require('./template');
const fs = require('fs');
const child_process = require('child_process');
const logger = utils.loggers.kake;

function PushIfNotExist(container, key, element) {
    if ( container[key] ) {
        if ( container[key].indexOf(element) == -1 ) {
            container[key].push(element);
        }
    }
    else {
        container[key] = [ element ];
    }
}

function AssignIfNotExist(container, key, value) {
    if ( !container[key] ) {
        container[key] = value;
    }
}

class Project {
    constructor(projectDir, options, solution) {
        let self = this;

        this._solution = solution;
        this._solutionPath = options.solutionPath;
        this._deps = options.deps;
        this._projectDir = utils.fs.absolutePath(projectDir);
        this._kakefile = path.join(this._projectDir, '/Kakefile');
        let projectConfig = require(this._kakefile);
        this._projectConfig = projectConfig;

        this._name = projectConfig.name;
        this._version = projectConfig.version;
        this._type = projectConfig.type;
        this._target = projectConfig.target;
        this._targetPath = utils.fs.absolutePath('.');
        this._options = projectConfig.options;
        if ( !this._options ) {
            this._options = {};
        }
    }

    readConfig(config) {
        const configFileName = `${config.os}-${config.arch}-${config.configurationName}.kake`;
        const configFilePath = path.join(this._projectDir, 'config', configFileName);
        this._solutionConfiguration = config.configuration;
        AssignIfNotExist(this._solutionConfiguration, 'options', {});

        if ( utils.fs.exists(configFilePath) ) {
            logger.info(`Read project config: ${configFileName}`);
            this._configuration = require(configFilePath);
        }
        else {
            logger.warn(`Use default configuration because can not find project config: ${configFileName}`);
            this._configuration = {};
        }

        // Combine configurations
        let finalConfiguration = Object.create(null);
        this._finalConfiguration = finalConfiguration;

        finalConfiguration.compiler = utils.config.combineConfig(
                this._solutionConfiguration.compiler, this._projectConfig.common.compiler);
        finalConfiguration.linker = utils.config.combineConfig(
                this._solutionConfiguration.linker, this._projectConfig.common.linker);
        finalConfiguration.dependencies = utils.config.combineConfig(
                this._solutionConfiguration.dependencies, this._projectConfig.common.dependencies);

        let conditions = this._solutionConfiguration.options;
        Object.keys(conditions).forEach(condition => {
            let conditionValue = this._solutionConfiguration.options[condition];
            if ( conditionValue ) {
                let solutionOptionConf = this._solution._options[condition];
                if ( solutionOptionConf && solutionOptionConf.value === conditionValue ) {
                    finalConfiguration.compiler = utils.config.combineConfig(
                            finalConfiguration.compiler, solutionOptionConf.compiler);
                    finalConfiguration.linker = utils.config.combineConfig(
                            finalConfiguration.linker, solutionOptionConf.linker);
                    finalConfiguration.dependencies = utils.config.combineConfig(
                            finalConfiguration.dependencies, solutionOptionConf.dependencies);
                }

                let projectOptionConf = this._options[condition];
                if ( projectOptionConf && projectOptionConf.value === conditionValue ) {
                    finalConfiguration.compiler = utils.config.combineConfig(
                            finalConfiguration.compiler, projectOptionConf.compiler);
                    finalConfiguration.linker = utils.config.combineConfig(
                            finalConfiguration.linker, projectOptionConf.linker);
                    finalConfiguration.dependencies = utils.config.combineConfig(
                            finalConfiguration.dependencies, projectOptionConf.dependencies);
                }
            }
        });

        this._buildPath = path.join(this._solution._buildPath, this._name);
        this._binPath = this._solution._binPath;
        this._libPath = this._solution._libPath;

        if ( this._target == 'executable' ) {
            this._installPath = this._binPath;
        }
        else {
            this._installPath = this._libPath;
        }

        utils.fs.mkdir(this._buildPath);
        utils.fs.mkdir(this._binPath);
        utils.fs.mkdir(this._libPath);
        utils.fs.mkdir(this._installPath);

        logger.debug(`Use build path: ${this._buildPath}`);
        logger.debug(`Use bin path: ${this._binPath}`);
        logger.debug(`Use lib path: ${this._libPath}`);
        logger.debug(`Use install path: ${this._installPath}`);

        this._buildPath = utils.fs.absolutePath(this._buildPath);
        this._binPath = utils.fs.absolutePath(this._binPath);
        this._libPath = utils.fs.absolutePath(this._libPath);
        this._installPath = utils.fs.absolutePath(this._installPath);

        AssignIfNotExist(finalConfiguration.compiler, 'exts', ['cpp']);
        if ( this._target == 'dynamic_library' ) {
            PushIfNotExist(finalConfiguration.compiler, 'cxxflags', '-fPIC');
        }

        finalConfiguration.compiler.src = finalConfiguration.compiler.src.map(srcPath => {
            return fs.realpathSync(path.join(this._solution._basePath, srcPath));
        });

        let srcDirs = finalConfiguration.compiler.src.filter(srcPath => fs.statSync(srcPath).isDirectory());
        finalConfiguration.compiler.src = finalConfiguration.compiler.src.filter(srcPath => fs.statSync(srcPath).isFile());
        srcDirs.forEach(srcDir => {
            let srcFiles = utils.fs.findFiles({
                path: srcDir,
                exts: finalConfiguration.compiler.exts
            });

            srcFiles.forEach(srcFile => {
                finalConfiguration.compiler.src.push(srcFile);
            });
        });

        if ( finalConfiguration.compiler.useCuda ) {
            AssignIfNotExist(finalConfiguration.compiler, 'nvccExts', ['cu']);

            finalConfiguration.compiler.nvccSrc = [];
            srcDirs.forEach(srcDir => {
                let srcFiles = utils.fs.findFiles({
                    path: srcDir,
                    exts: finalConfiguration.compiler.nvccExts
                });

                srcFiles.forEach(srcFile => {
                    finalConfiguration.compiler.nvccSrc.push(srcFile);
                });
            });
        }

        finalConfiguration.compiler.includePaths = finalConfiguration.compiler.includePaths.map(includePath => {
            return fs.realpathSync(path.join(this._solution._basePath, includePath));
        });
    }

    generateMakefile() {
        let configs = this.generateConfig();
        let configLines = Object.keys(configs).map(configKey => {
            let configValue = configs[configKey];

            if ( !configValue ) {
                configValue = '';
            }

            if ( configValue instanceof Array ) {
                configValue = configValue.join(' ');
            }

            return `${configKey} := ${configValue}`
        });

        let configContent = configLines.join('\n');
        let configFilePath = `${this._buildPath}/Makefile.config`;
        fs.writeFileSync(configFilePath, configContent);

        let depConfigs = this.generateDepConfigs();
        let depConfigsLines = Object.keys(depConfigs).map(configKey => {
            let configValue = depConfigs[configKey];

            if ( configValue instanceof Array ) {
                configValue = configValue.join(' ');
            }

            return `${configKey} := ${configValue}`
        });

        let depConfigContent = depConfigsLines.join('\n');
        let depConfigFilePath = `${this._buildPath}/../Makefile.deps`;
        fs.writeFileSync(depConfigFilePath, depConfigContent);

        let makefileLines = [
            '-include Makefile.config\n',
            '-include ../Makefile.deps\n'
        ];
        let objectFiles = [];
        let dependencyLines = [];

        let finalConfiguration = this._finalConfiguration;
        finalConfiguration.compiler.src.forEach(srcPath => {
            let {objectFileName, dependencyLine} = this.generateDependency(srcPath);

            let compilerCommand = this.generateCompilerCommand(srcPath);
            dependencyLines.push(dependencyLine);
            dependencyLines.push(compilerCommand + '\n\n');

            objectFiles.push(objectFileName);
        });

        if ( finalConfiguration.compiler.useCuda ) {
            finalConfiguration.compiler.nvccSrc.forEach(srcPath => {
                let {objectFileName, dependencyLine} = this.generateNvccDependency(srcPath);

                let compilerCommand = this.generateNvccCompilerCommand(srcPath);
                dependencyLines.push(dependencyLine);
                dependencyLines.push(compilerCommand + '\n\n');
                
                objectFiles.push(objectFileName);
            });
        }

        makefileLines.push(`OBJS := ${objectFiles.join(' \\\n')}\n`);

        let targetName = this._name;
        if ( this._target == 'dynamic_library' ) {
            targetName = `lib${targetName}.so`;
        }
        this._targetName = targetName;
        makefileLines.push(`all: ${targetName}\n`);
        makefileLines.push(this.generateLinkLine(targetName) + '\n');

        let binPath = utils.fs.relativePath(this._installPath, this._buildPath);
        this.generateInstallLines(makefileLines, targetName, binPath);
        this.generateCleanLines(makefileLines, targetName);

        let makefileContent = `${makefileLines.join('\n')}\n${dependencyLines.join('')}`;
        let makefilePath = `${this._buildPath}/Makefile`;

        fs.writeFileSync(makefilePath, makefileContent);
    }

    generateInstallLines(makefileLines, targetName, binPath) {
        let installTargetHint = this.generateMakefileHint({
            type: 'INSTALL',
            message: `${targetName} -> ${binPath}`
        });

        makefileLines.push(`install: ${binPath}/${targetName}\n`);
        makefileLines.push(`${binPath}/${targetName}: ${targetName}`);
        makefileLines.push(`\t@${installTargetHint}`);
        makefileLines.push(`\t@cp ${targetName} ${binPath}\n`);
    }

    generateCleanLines(makefileLines, targetName) {
        let cleanTargetHint = this.generateMakefileHint({
            type: 'CLEAN',
            message: targetName
        });

        makefileLines.push(`clean:`);
        makefileLines.push(`\t@${cleanTargetHint}`);
        makefileLines.push(`\t@rm -f ${targetName}`);
        makefileLines.push(`\t@rm -f *.o\n`);
    }

    build(parallism) {
        child_process.execSync(`make -j ${parallism}`, {
            cwd: this._buildPath,
            stdio: ['inherit', 'inherit', 'inherit' ]
        });
    }

    install(parallism) {
        child_process.execSync(`make install -j ${parallism}`, {
            cwd: this._buildPath,
            stdio: ['inherit', 'inherit', 'inherit' ]
        });
    }

    clean() {
        child_process.execSync('make clean', {
            cwd: this._buildPath,
            stdio: ['inherit', 'inherit', 'inherit' ]
        });
    }

    generateLinkLine(targetName) {
        let linkLines = [
            `${targetName}: $(OBJS)`,
            this.generateLinkerCommand(targetName)
        ];

        return linkLines.join('\n');
    }

    generateMakefileHint(options) {
        return `echo -e "\\e[32m[${options.type}] \\033[0m${options.message}"`;
    }

    generateLinkerCommand(targetName) {
        let finalConfiguration = this._finalConfiguration;

        let command = `\t@$(LD) $(OBJS)`;
        if ( this._target == 'dynamic_library' ) {
            command += ' -shared';
        }

        let libraryPaths = [];
        let inputs = [];

        if ( finalConfiguration.dependencies ) {
            Object.keys(finalConfiguration.dependencies).forEach(depName => {
                let dep = this._deps[depName];

                if ( !dep ) {
                    let depProject = this._solution.getProject(depName);
                    let relLibPath = utils.fs.relativePath(depProject._buildPath, this._buildPath);

                    libraryPaths.push(`-L"${relLibPath}"`);
                    inputs.push(`-l${depProject._name}`);

                    return;
                }

                if ( dep.scope != 'system' ) {
                    libraryPaths.push(`-L"$(${depName.toUpperCase()}_LIB)"`);
                }
                inputs = inputs.concat(dep.inputs.map(input => `-l${input}`));
            });
        }

        let ldflags = '';
        if ( finalConfiguration.linker.ldflags ) {
            ldflags = finalConfiguration.linker.ldflags.join(' ');
        }

        let makefileHint = this.generateMakefileHint({
            type: 'LINK',
            message: targetName
        });

        return `\t@${makefileHint}\n${command} -o ${targetName} ${libraryPaths.join(' ')} ${inputs.join(' ')} ${ldflags}`;
    }

    generateConfig() {
        let config = {
            SHELL: '/bin/bash',
            CXX: this._finalConfiguration.compiler.cxx,
            NVCC: this._finalConfiguration.compiler.nvcc,
            LD: this._finalConfiguration.linker.ld,
            LDFLAGS: this._finalConfiguration.linker.ldflags,
            OS: this._solution._os,
            CPU: this._solution._arch,
            PLATFORM: '$(OS)/$(CPU)'
        };

        return config;
    }

    generateDepConfigs() {
        let configs = {};

        Object.keys(this._deps).forEach(depName => {
            let dep = this._deps[depName];

            if ( dep.scope == 'system' ) {
                return;
            }

            let includeKey = `${depName}_INCLUDE`.toUpperCase();
            let libKey = `${depName}_LIB`.toUpperCase();

            let includeValues = dep.include.map(include => {
                let includeValue = `${dep.basePath}/${include}`;
                if ( !(includeValue.startsWith('/')) ) {
                    includeValue = utils.fs.relativePath(`${this._solutionPath}/${includeValue}`, this._buildPath);
                }

                return `-I"${includeValue}"`;
            });

            let libValue = `${dep.basePath}/${dep.lib}`;

            if ( !(libValue.startsWith('/')) ) {
                libValue = utils.fs.relativePath(`${this._solutionPath}/${libValue}`, this._buildPath);
            }

            configs[includeKey] = includeValues;
            configs[libKey] = libValue;
        });

        return configs;
    }

    generateDependency(srcPath) {
        let finalConfiguration = this._finalConfiguration;

        logger.info(`Resolve dependency: ${srcPath}`);
        let cwd = this._buildPath;
        let srcRelativePath = utils.fs.relativePath(srcPath, this._buildPath);
        let commandArguments = [srcRelativePath, '-c','-MM'];
        commandArguments = this.fillCompilerCommandArgs(commandArguments, {
            useIncludeVariable: false
        });

        let resolveResult = child_process.execFileSync(finalConfiguration.compiler.cxx, commandArguments, {
            cwd: cwd,
            encoding: 'utf8'
        });

        let fileNameEndIndex = resolveResult.indexOf(':');
        let objectFileName = srcRelativePath;
        objectFileName = utils.string.replaceAll(objectFileName, '_', '__');
        objectFileName = utils.string.replaceAll(objectFileName, '-', '--');
        objectFileName = utils.string.replaceAll(objectFileName, '.', '_');
        objectFileName = utils.string.replaceAll(objectFileName, '/', '-');
        objectFileName += '.o';

        let dependencyLine = objectFileName + 
            resolveResult
            .slice(fileNameEndIndex)
            .split('\n')
            .filter(line => !(line.trim().startsWith('/')))
            .join('\n');

        if ( dependencyLine.trim().endsWith('\\') ) {
            dependencyLine = dependencyLine.trim();
            dependencyLine = dependencyLine.slice(0, dependencyLine.length - 1) + '\n';
        }

        return {
            objectFileName: objectFileName,
            dependencyLine: dependencyLine
        };
    }

    generateNvccDependency(srcPath) {
        let finalConfiguration = this._finalConfiguration;

        logger.info(`Resolve dependency: ${srcPath}`);
        let cwd = this._buildPath;
        let srcRelativePath = utils.fs.relativePath(srcPath, this._buildPath);
        let commandArguments = [srcRelativePath, '-c', '--verbose', '--dryrun'];
        commandArguments = this.fillNvidiaCompilerCommandArgs(commandArguments, {
            useIncludeVariable: false
        });

        let nvccResult = utils.child_process.execFile(finalConfiguration.compiler.nvcc, commandArguments, {
            cwd: cwd,
            encoding: 'utf8'
        });

        let cxxDirName = path.dirname(finalConfiguration.compiler.cxx);
        let cxxBaseName = path.basename(finalConfiguration.compiler.cxx);
        let lineStart = `#$ ${finalConfiguration.compiler.cxx}`;
        if ( cxxDirName.startsWith('/') ) {
            lineStart = `#$ "${cxxDirName}"/${cxxBaseName}`
        }

        let cbinCommand = nvccResult.stderr.split('\n').find(line => {
            return line.startsWith(lineStart);
        });

        let redirectIndex = cbinCommand.indexOf('>');
        cbinCommand = cbinCommand.slice(3, redirectIndex) + ' -MM';

        let resolveResult = child_process.execSync(cbinCommand, {
            cwd: cwd,
            encoding: 'utf8'
        });

        let fileNameEndIndex = resolveResult.indexOf(':');
        let objectFileName = srcRelativePath;
        objectFileName = utils.string.replaceAll(objectFileName, '_', '__');
        objectFileName = utils.string.replaceAll(objectFileName, '-', '--');
        objectFileName = utils.string.replaceAll(objectFileName, '.', '_');
        objectFileName = utils.string.replaceAll(objectFileName, '/', '-');
        objectFileName += '.o';

        let dependencyLine = objectFileName + 
            resolveResult
            .slice(fileNameEndIndex)
            .split('\n')
            .filter(line => !(line.trim().startsWith('/')))
            .join('\n');

        if ( dependencyLine.trim().endsWith('\\') ) {
            dependencyLine = dependencyLine.trim();
            dependencyLine = dependencyLine.slice(0, dependencyLine.length - 1) + '\n';
        }

        return {
            objectFileName: objectFileName,
            dependencyLine: dependencyLine
        };
    }

    generateCompilerCommand(srcPath) {
        let srcRelativePath = utils.fs.relativePath(srcPath, this._buildPath);
        let commandArguments = ['$(CXX)', srcRelativePath, '-c', '-o', '$@'];
        commandArguments = this.fillCompilerCommandArgs(commandArguments, {
            useIncludeVariable: true
        });

        let compileHint = this.generateMakefileHint({
            type: 'CXX',
            message: `${srcRelativePath} -> $@`
        });

        return `\t@${compileHint}\n` + '\t@' + commandArguments.join(' ');
    }

    generateNvccCompilerCommand(srcPath) {
        let srcRelativePath = utils.fs.relativePath(srcPath, this._buildPath);
        let commandArguments = ['$(NVCC)', srcRelativePath, '-c', '-o', '$@'];
        commandArguments = this.fillNvidiaCompilerCommandArgs(commandArguments, {
            useIncludeVariable: true
        });

        let compileHint = this.generateMakefileHint({
            type: 'NVCC',
            message: `${srcRelativePath} -> $@`
        });

        return `\t@${compileHint}\n` + '\t@' + commandArguments.join(' ');
    }

    fillCompilerCommandArgs(commandArguments, options) {
        let finalConfiguration = this._finalConfiguration;

        finalConfiguration.compiler.includePaths.forEach(includePath => {
            let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
            commandArguments.push(`-I${incRelPath}`);
        });

        if ( finalConfiguration.dependencies ) {
            Object.keys(finalConfiguration.dependencies).forEach(depName => {
                let dep = this._deps[depName];

                if ( !dep ) {
                    let depProject = this._solution.getProject(depName);
                    depProject._finalConfiguration.compiler.includePaths.forEach(includePath => {
                        let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
                        commandArguments.push(`-I${incRelPath}`);
                    });

                    return;
                }

                if ( dep.scope == 'custom' ) {
                    if ( options.useIncludeVariable ) {
                        commandArguments.push(`$(${depName.toUpperCase()}_INCLUDE)`);
                    }
                    else {
                        dep.include.forEach(include => {
                            let includePath = `${dep.basePath}/${include}`;
                            if ( !includePath.startsWith('/') ) {
                                includePath = `${this._solutionPath}/${includePath}`;
                                let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
                                commandArguments.push(`-I${incRelPath}`);
                            }
                            else {
                                commandArguments.push(`-I${includePath}`);
                            }
                        });
                    }
                }
            });
        }

        if ( finalConfiguration.compiler.std ) {
            commandArguments.push(`-std=${finalConfiguration.compiler.std}`);
        }

        if ( finalConfiguration.compiler.optimize ) {
            commandArguments.push(`-O${finalConfiguration.compiler.optimize}`);
        }
        else {
            commandArguments.push(`-O2`);
        }

        if ( finalConfiguration.compiler.cxxflags ) {
            commandArguments = commandArguments.concat(finalConfiguration.compiler.cxxflags);
        }

        if ( finalConfiguration.compiler.defines ) {
            commandArguments = commandArguments.concat(finalConfiguration.compiler.defines.map(definition => {
                return `-D${definition}`;
            }));
        }

        return commandArguments;
    }

    fillNvidiaCompilerCommandArgs(commandArguments, options) {
        let finalConfiguration = this._finalConfiguration;

        commandArguments.push('-ccbin');
        commandArguments.push(finalConfiguration.compiler.cxx);

        finalConfiguration.compiler.includePaths.forEach(includePath => {
            let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
            commandArguments.push(`-I${incRelPath}`);
        });

        if ( finalConfiguration.dependencies ) {
            Object.keys(finalConfiguration.dependencies).forEach(depName => {
                let dep = this._deps[depName];

                if ( !dep ) {
                    let depProject = this._solution.getProject(depName);
                    depProject.finalConfiguration.compiler.includePaths.forEach(includePath => {
                        let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
                        commandArguments.push(`-I${incRelPath}`);
                    });

                    return;
                }

                if ( dep.scope == 'custom' ) {
                    if ( options.useIncludeVariable ) {
                        commandArguments.push(`$(${depName.toUpperCase()}_INCLUDE)`);
                    }
                    else {
                        dep.include.forEach(include => {
                            let includePath = `${dep.basePath}/${include}`;
                            if ( !includePath.startsWith('/') ) {
                                includePath = `${this._solutionPath}/${includePath}`;
                                let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
                                commandArguments.push(`-I${incRelPath}`);
                            }
                            else {
                                commandArguments.push(`-I${includePath}`);
                            }
                        });
                    }
                }
            });
        }

        if ( finalConfiguration.compiler.std ) {
            commandArguments.push(`-std=${finalConfiguration.compiler.std}`);
        }

        if ( finalConfiguration.compiler.cxxflags ) {
            finalConfiguration.compiler.cxxflags.forEach(cxxflag => {
                commandArguments.push('-Xcompiler');
                commandArguments.push(cxxflag);
            });
        }

        if ( finalConfiguration.compiler.nvccflags ) {
            finalConfiguration.compiler.nvccflags.forEach(nvccflag => {
                commandArguments.push(nvccflag);
            });
        }

        if ( finalConfiguration.compiler.defines ) {
            commandArguments = commandArguments.concat(finalConfiguration.compiler.defines.map(definition => {
                return `-D${definition}`;
            }));
        }

        return commandArguments;
    }

}

Project.CreateNewProject = function(options) {
    let projectName = options.name;
    let targetType = options.target;

    let projectTemplateContent = template.readTemplateFile('project/CppProject.kake');
    let kakefileContent = projectTemplateContent.replace('$(ProjectName)', projectName);
    kakefileContent = kakefileContent.replace('$(TargetType)', targetType);

    utils.fs.mkdir(projectName);

    let kakefilePath = path.join(projectName, 'Kakefile');
    fs.writeFileSync(kakefilePath, kakefileContent);
    logger.info(`Generating ${kakefilePath} ...`);
    logger.info('Bootstrap finished.');
}

module.exports = Project;
