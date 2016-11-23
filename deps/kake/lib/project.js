'use strict';

let utils = {
    fs: require('./util/fs'),
    string: require('./util/string'),
    child_process: require('./util/child_process')
};

let template = require('./template');
let fs = require('fs');
let child_process = require('child_process');

class Project {
    constructor(projectDir, options, solution) {
        let self = this;

        this._solution = solution;
        this._solutionPath = options.solutionPath;
        this._deps = options.deps;
        this._basePath = projectDir;
        this._completePath = fs.realpathSync(projectDir);
        this._kakefile = this._completePath + '/Kakefile';
        let projectConfig = require(this._kakefile);
        this._projectConfig = projectConfig;

        this._name = projectConfig.name;
        this._version = projectConfig.version;
        this._type = projectConfig.type;
        this._target = projectConfig.target;
        this._targetPath = fs.realpathSync(projectDir + '/' + projectConfig.targetPath);
        this._buildPath = this._targetPath + `/build/linux/x64/Release/${this._name}`;
        this._binPath = fs.realpathSync(`${this._targetPath}/bin/linux/x64/Release`);
        this._libPath = fs.realpathSync(`${this._targetPath}/lib/linux/x64/Release`);
        if ( this._target == 'executable' ) {
            this._installPath = this._binPath;
        }
        else {
            this._installPath = this._libPath;
        }

        // Ignore when the directory exists.
        try {
            fs.mkdirSync(this._buildPath);
        }
        catch (e) {
        }
        this._buildPath = fs.realpathSync(this._buildPath);

        this._compiler = projectConfig.compiler;
        this._linker = projectConfig.linker;
        this._dependencies = projectConfig.dependencies;

        if ( !this._compiler.exts ) {
            this._compiler.exts = ['cpp'];
        }
        if ( this._target == 'dynamic_library' ) {
            if ( this._compiler.cxxflags ) {
                this._compiler.cxxflags.push('-fPIC');
            }
            else {
                this._compiler.cxxflags = ['-fPIC'];
            }
        }
        
        this._compiler.src = this._compiler.src.map(srcPath => {
            return fs.realpathSync(projectDir + '/' + srcPath);
        });

        let srcDirs = this._compiler.src.filter(srcPath => fs.statSync(srcPath).isDirectory());
        this._compiler.src = this._compiler.src.filter(srcPath => fs.statSync(srcPath).isFile());
        srcDirs.forEach(srcDir => {
            let srcFiles = utils.fs.findFiles({
                path: srcDir,
                exts: self._compiler.exts
            });

            srcFiles.forEach(srcFile => {
                this._compiler.src.push(srcFile);
            });
        });

        if ( this._compiler.useCuda ) {
            if ( !this._compiler.nvccExts ) {
                this._compiler.nvccExts = ['cu'];
            }

            this._compiler.nvccSrc = [];
            srcDirs.forEach(srcDir => {
                let srcFiles = utils.fs.findFiles({
                    path: srcDir,
                    exts: self._compiler.nvccExts
                });

                srcFiles.forEach(srcFile => {
                    this._compiler.nvccSrc.push(srcFile);
                });
            });
        }

        this._compiler.includePaths = this._compiler.includePaths.map(includePath => {
            return fs.realpathSync(projectDir + '/' + includePath);
        });
    }

    generateMakefile() {
        let configs = this.generateConfig();
        let configLines = Object.keys(configs).map(configKey => {
            let configValue = configs[configKey];

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

        this._compiler.src.forEach(srcPath => {
            let {objectFileName, dependencyLine} = this.generateDependency(srcPath);

            let compilerCommand = this.generateCompilerCommand(srcPath);
            dependencyLines.push(dependencyLine);
            dependencyLines.push(compilerCommand + '\n\n');

            objectFiles.push(objectFileName);
        });

        if ( this._compiler.useCuda ) {
            this._compiler.nvccSrc.forEach(srcPath => {
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
        makefileLines.push(`install: ${binPath}/${targetName}\n`);
        makefileLines.push(`${binPath}/${targetName}: ${targetName}`);
        makefileLines.push(`\tcp ${targetName} ${binPath}\n`);

        makefileLines.push(`clean:`);
        makefileLines.push(`\trm -f ${targetName}`);
        makefileLines.push(`\trm -f *.o\n`);
        
        let makefileContent = `${makefileLines.join('\n')}\n${dependencyLines.join('')}`;
        let makefilePath = `${this._buildPath}/Makefile`;

        fs.writeFileSync(makefilePath, makefileContent);
    }

    generateLinkLine(targetName) {
        let linkLines = [
            `${targetName}: $(OBJS)`,
            this.generateLinkerCommand(targetName)
        ];

        return linkLines.join('\n');
    }

    generateLinkerCommand(targetName) {
        let command = `\t$(LD) $(OBJS)`;
        if ( this._target == 'dynamic_library' ) {
            command += ' -shared';
        }

        let libraryPaths = [];
        let inputs = [];

        if ( this._dependencies ) {
            Object.keys(this._dependencies).forEach(depName => {
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
        if ( this._linker.ldflags ) {
            ldflags = this._linker.ldflags.join(' ');
        }

        return `${command} -o ${targetName} ${libraryPaths.join(' ')} ${inputs.join(' ')} ${ldflags}`;
    }

    generateConfig() {
        let config = {
            CXX: this._compiler.cxx,
            NVCC: this._compiler.nvcc,
            LD: this._linker.ld,
            LDFLAGS: this._linker.ldflags,
            OS: 'linux',
            BITS: '64',
            CPU: 'x64',
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
        console.log(`Resolve dependency: ${srcPath}`);
        let cwd = this._buildPath;
        let srcRelativePath = utils.fs.relativePath(srcPath, this._buildPath);
        let commandArguments = [srcRelativePath, '-c','-MM'];
        commandArguments = this.fillCompilerCommandArgs(commandArguments, {
            useIncludeVariable: false
        });

        let resolveResult = child_process.execFileSync('g++', commandArguments, {
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
        console.log(`Resolve dependency: ${srcPath}`);
        let cwd = this._buildPath;
        let srcRelativePath = utils.fs.relativePath(srcPath, this._buildPath);
        let commandArguments = [srcRelativePath, '-c', '--verbose', '--dryrun'];
        commandArguments = this.fillNvidiaCompilerCommandArgs(commandArguments, {
            useIncludeVariable: false
        });

        let nvccResult = utils.child_process.execFile('nvcc', commandArguments, {
            cwd: cwd,
            encoding: 'utf8'
        });

        let cbinCommand = nvccResult.stderr.split('\n').find(line => {
            return line.startsWith('#$ gcc');
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

        return '\t' + commandArguments.join(' ');
    }

    generateNvccCompilerCommand(srcPath) {
        let srcRelativePath = utils.fs.relativePath(srcPath, this._buildPath);
        let commandArguments = ['$(NVCC)', srcRelativePath, '-c', '-o', '$@'];
        commandArguments = this.fillNvidiaCompilerCommandArgs(commandArguments, {
            useIncludeVariable: true
        });

        return '\t' + commandArguments.join(' ');
    }

    fillCompilerCommandArgs(commandArguments, options) {
        this._compiler.includePaths.forEach(includePath => {
            let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
            commandArguments.push(`-I${incRelPath}`);
        });

        if ( this._dependencies ) {
            Object.keys(this._dependencies).forEach(depName => {
                let dep = this._deps[depName];

                if ( !dep ) {
                    let depProject = this._solution.getProject(depName);
                    depProject._compiler.includePaths.forEach(includePath => {
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

        if ( this._compiler.std ) {
            commandArguments.push(`-std=${this._compiler.std}`);
        }

        if ( this._compiler.optimize ) {
            commandArguments.push(`-O${this._compiler.optimize}`);
        }
        else {
            commandArguments.push(`-O2`);
        }

        commandArguments = commandArguments.concat(this._compiler.cxxflags);

        if ( this._compiler.defines ) {
            commandArguments = commandArguments.concat(this._compiler.defines.map(definition => {
                return `-D${definition}`;
            }));
        }

        return commandArguments;
    }

    fillNvidiaCompilerCommandArgs(commandArguments, options) {
        this._compiler.includePaths.forEach(includePath => {
            let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
            commandArguments.push(`-I${incRelPath}`);
        });

        if ( this._dependencies ) {
            Object.keys(this._dependencies).forEach(depName => {
                let dep = this._deps[depName];

                if ( !dep ) {
                    let depProject = this._solution.getProject(depName);
                    depProject._compiler.includePaths.forEach(includePath => {
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

        if ( this._compiler.std ) {
            commandArguments.push(`-std=${this._compiler.std}`);
        }
        this._compiler.cxxflags.forEach(cxxflag => {
            commandArguments.push('-Xcompiler');
            commandArguments.push(cxxflag);
        });

        if ( this._compiler.nvccflags ) {
            this._compiler.nvccflags.forEach(nvccflag => {
                commandArguments.push(nvccflag);
            });
        }

        if ( this._compiler.defines ) {
            commandArguments = commandArguments.concat(this._compiler.defines.map(definition => {
                return `-D${definition}`;
            }));
        }

        return commandArguments;
    }

}

Project.CreateNewProject = function(options) {
    let projectName = options.name;

    let projectTemplateContent = template.readTemplateFile('project/CppProject.kake');
    let kakefileContent = projectTemplateContent.replace('CppProject', projectName);

    try {
        fs.mkdirSync(projectName);
        console.log(`Created directory ${projectName}`);
    }
    catch (e) {
        console.log(`The project dir ${projectName} has been existed. Skip create directory.`);
    }

    let kakefilePath = projectName + '/Kakefile';
    fs.writeFileSync(kakefilePath, kakefileContent);
    console.log(`Generating ${kakefilePath} ...`);
    console.log('Bootstrap finished.');
}

module.exports = Project;
