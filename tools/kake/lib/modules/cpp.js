'use strict';

const path = require('path');
const utils = {
    fs: require('../util/fs'),
    string: require('../util/string'),
    config: require('../util/config'),
    child_process: require('../util/child_process'),
    loggers: require('../util/loggers'),
    xml: require('../util/xml')
};

const template = require('../template');
const fs = require('fs');
const child_process = require('child_process');
const logger = utils.loggers.kake;

const XmlElement = utils.xml.XmlElement;
const XmlGenerator = utils.xml.XmlGenerator;

const Project = require('../project').Project;
const AssignIfNotExist = Project.AssignIfNotExist;
const PushIfNotExist = Project.PushIfNotExist;

class CppProject extends Project {
    constructor(projectDir, options, solution) {
        super(projectDir, options, solution);
    }

    readConfig(config) {
        super.readConfig(config);

        let finalConfiguration = this._finalConfiguration;

        this._binPath = this._solution._binPath;
        this._libPath = this._solution._libPath;
        this._packagePath = this._solution._packagePath;

        if ( this._target == 'executable' ) {
            this._collectPath = this._binPath;
        }
        else {
            this._collectPath = this._libPath;
        }

        utils.fs.mkdir(this._binPath);
        utils.fs.mkdir(this._libPath);
        utils.fs.mkdir(this._collectPath);
        utils.fs.mkdir(this._packagePath);

        logger.debug(`Use bin path: ${this._binPath}`);
        logger.debug(`Use lib path: ${this._libPath}`);
        logger.debug(`Use collect path: ${this._collectPath}`);
        logger.debug(`Use package path: ${this._packagePath}`);

        this._binPath = utils.fs.absolutePath(this._binPath);
        this._libPath = utils.fs.absolutePath(this._libPath);
        this._collectPath = utils.fs.absolutePath(this._collectPath);
        this._packagePath = utils.fs.absolutePath(this._packagePath);

        AssignIfNotExist(finalConfiguration.compiler, 'exts', ['cpp']);

        finalConfiguration.compiler.src = finalConfiguration.compiler.src.map(srcPath => {
            return fs.realpathSync(path.join(this._solution._basePath, srcPath));
        });

        let srcDirs = finalConfiguration.compiler.src.filter(srcPath => fs.statSync(srcPath).isDirectory());
        let nvccSrc = []; 
        let cppSrc = [];

        finalConfiguration.compiler.src.forEach(srcPath => {
            if ( !fs.statSync(srcPath).isFile() ) {
                return;
            }

            let pathParseResult = path.parse(srcPath);
            if ( !pathParseResult.ext ) {
                return;
            }

            if ( finalConfiguration.compiler.nvccExts.indexOf(pathParseResult.ext.slice(1)) != -1 ) {
                nvccSrc.push(srcPath);
            }
            else {
                cppSrc.push(srcPath);
            }
        });

        finalConfiguration.compiler.src = cppSrc;
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

            finalConfiguration.compiler.nvccSrc = nvccSrc;
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

    combineConfigFrom(dest, base, extra) {
        dest.compiler = utils.config.combineConfig(
                base.compiler, extra.compiler);
        dest.linker = utils.config.combineConfig(
                base.linker, extra.linker);
        dest.dependencies = utils.config.combineConfig(
                base.dependencies, extra.dependencies);
        dest.package = utils.config.combineConfig(
                base.package, extra.package);
    }

    generateMakefile() {
        super.generateMakefile();

        let makefileLines = [
            '-include Makefile.config\n',
            '-include ../Makefile.deps\n'
        ];
        let objectFiles = [];
        let dependencyLines = [];

        let finalConfiguration = this._finalConfiguration;
        PushIfNotExist(finalConfiguration.compiler, 'cxxflags', '-fPIC');
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
        this._objectFiles = objectFiles;

        if ( finalConfiguration.linker && finalConfiguration.linker.linkDepObjects ) {
            const depObjectFiles = this.generateDepObjectFiles();
            makefileLines.push(`DEP_OBJS := ${depObjectFiles.join(' \\\n')}\n`);
        }

        let targetName = this._name;
        if ( this._target == 'dynamic_library' ) {
            targetName = `lib${targetName}.so`;
        }
        else if ( this._target == 'static_library' ) {
            targetName = `lib${targetName}.a`;
        }
        this._targetName = targetName;
        makefileLines.push(`all: collect\n`);
        makefileLines.push(this.generateLinkLine(targetName) + '\n');

        let binPath = utils.fs.relativePath(this._collectPath, this._buildPath);
        this.generateCollectLines(makefileLines, targetName, binPath);
        this.generatePackageLines(makefileLines, binPath, targetName);
        this.generateCleanLines(makefileLines, targetName);

        let makefileContent = `${makefileLines.join('\n')}\n${dependencyLines.join('')}`;
        let makefilePath = `${this._buildPath}/Makefile`;

        fs.writeFileSync(makefilePath, makefileContent);
    }

    generateMsvsCxxFlags(finalConfiguration) {
        let cxxFlags = new Set();
    }

    generateMsvsDeps(finalConfiguration) {
        let includePaths = new Set();
        let libDirPaths = new Set();
        let inputFiles = new Set();

        if ( finalConfiguration.compiler.includePaths ) {
            includePaths = new Set(finalConfiguration.compiler.includePaths);
        }

        for ( let depName of this._directInternalDeps ) {
            const dep = this._deps[depName];

            if ( !this._solution._projectsMap.has(depName) ) {
                logger.fatal(`Internal dependency ${depName} is not found`);
                process.exit(1);
            }

            const depProject = this._solution._projectsMap.get(depName);
            depProject._finalConfiguration.compiler.includePaths.forEach(includePath => {
                let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
                includePath = `$(ProjectDir)${path.sep}${incRelPath}`;
                includePaths.add(includePath);
            });
        }

        for ( let depName of this._externalDeps ) {
            const dep = this._solution._deps[depName];
            
            if ( !dep ) {
                logger.fatal(`External dependency ${depName} is not found`);
                process.exit(1);
            }

            if ( dep.scope === 'custom' ) {
                dep.completeIncludes.forEach(include => {
                    includePaths.add(include);
                });

                dep.completeLibs.forEach(libDir => {
                    libDirPaths.add(libDir);
                });

                dep.inputs.forEach(input => {
                    if ( input.endsWith('.lib') ) {
                        inputFiles.add(input);
                    }
                    else {
                        inputFiles.add(`lib${input}.lib`);
                    }
                });
            }
            else if ( dep.scope === 'system' ) {
                dep.inputs.forEach(input => {
                    if ( input.endsWith('.lib') ) {
                        inputFiles.add(input);
                    }
                    else {
                        inputFiles.add(`lib${input}.lib`);
                    }
                });
            }
        }

        includePaths = Array.from(includePaths);
        let includes = '%(AdditionalIncludeDirectories)';
        if ( includePaths.length > 0 ) {
            includes = `${includePaths.join(';')};${includes}`;
        }

        libDirPaths = Array.from(libDirPaths);
        let libDirs = '%(AdditionalLibraryDirectories)';
        if ( libDirPaths.length > 0 ) {
            libDirs = `${libDirPaths.join(';')};${libDirs}`;
        }

        inputFiles = Array.from(inputFiles);
        let libs = '%(AdditionalDependencies)';
        if ( inputFiles.length > 0 ) {
            libs = `${inputFiles.join(';')};${libs}`;
        }

        return {
            includes,
            libDirs,
            libs
        };
    }

    generateMsvs(msvsProject) {
        let targetType = null;
        let subSystem = null;
        let platform = this._solution._arch;
        let configurationName = this._solution._configurationName;
        let finalConfiguration = this._finalConfiguration;

        if ( this._target === 'executable' ) {
            targetType = 'Application';
            subSystem = 'Console';
        }
        else if ( this._target === 'dynamic_library' ) {
            targetType = 'DynamicLibrary';
            subSystem = 'Windows';
        }

        if ( this._target === 'dynamic_library' ) {
            PushIfNotExist(finalConfiguration.compiler, 'defines', '_USRDLL');
        }

        let definitions = '%(PreprocessorDefinitions)';
        if ( finalConfiguration.compiler.defines ) {
            definitions = `${finalConfiguration.compiler.defines.join(';')};${definitions}`;
        }

        let additionalOptions = '%(AdditionalOptions)';
        if ( finalConfiguration.compiler.cxxflags ) {
            additionalOptions = `${finalConfiguration.compiler.cxxflags.join(' ')} ${additionalOptions}`;
        }

        const { includes, libDirs, libs } = this.generateMsvsDeps(finalConfiguration);

        let projectConfiguration = {
            configuration: configurationName,
            platform: platform,
            type: targetType,
            useDebugLibraries: false,
            platformToolset: 'v120',
            characterSet: 'MultiByte',
            wholeProgramOptimization: false,
            localAppDataPlatform:  '$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props',
            linker: {
                basic: {
                    linkIncremental: false,
                    outDir: '$(SolutionDir)..\\bin\\',
                    intDir: '$(ProjectDir)build\\'
                },
                other: {
                    subSystem: subSystem,
                    generateDebugInformation: true,
                    optimizeReferences: true,
                    additionalLibraryDirectories: libDirs,
                    additionalDependencies: libs
                }
            },
            compiler: {
                precompiledHeader: 'NotUsing',
                warningLevel: 'Level3',
                optimization: 'MaxSpeed',
                functionLevelLinking: true,
                intrinsicFunctions: true,
                preprocessorDefinitions: definitions,
                additionalIncludeDirectories: includes,
                precompiledHeaderOutputFile: '$(ProjectDir)build\\ipch\\$(TargetName).pch',
                additionalOptions: additionalOptions
            }
        };
        
        msvsProject.addConfigurations(projectConfiguration);

        this.writeMsvsProject(msvsProject._projectPath, msvsProject.toProjectFile());
        this.writeMsvsProject(`${msvsProject._projectPath}.filters`, msvsProject.toFilterFile());
    }

    writeMsvsProject(filePath, project) {
        const generator = new XmlGenerator({
            version: '1.0',
            encoding: 'utf-8'
        });

        generator.write(filePath, project);
    }

    generateCollectLines(makefileLines, targetName, binPath) {
        let collectTargetHint = this.generateMakefileHint({
            type: 'COLLECT',
            message: `${this._name}/${targetName} -> ${binPath}`
        });

        makefileLines.push(`collect: ${binPath}/${targetName}\n`);
        makefileLines.push(`${binPath}/${targetName}: ${targetName}`);
        makefileLines.push(`\t@${collectTargetHint}`);
        makefileLines.push(`\t@cp ${targetName} ${binPath}\n`);
    }

    generatePackageLines(makefileLines, binPath, targetName) {
        let packageDir = utils.fs.relativePath(this._packagePath, this._buildPath);
        
        let packageTargetHint = this.generateMakefileHint({
            type: 'PACKAGE',
            message: this._name
        });

        makefileLines.push(`package:`);
        makefileLines.push(`\t@${packageTargetHint}`);

        let filesToPackage = this.getFilesInPackage(binPath, targetName);
        filesToPackage.forEach(fileInfo => {
            fileInfo.dest = path.join(packageDir, fileInfo.dest, fileInfo.relative);

            let operationHint = this.generateMakefileHint({
                type: 'COPY',
                message: `${fileInfo.src} => ${fileInfo.dest}`
            });

            let destDir = path.parse(fileInfo.dest).dir;
            makefileLines.push(`\t@${operationHint}`);
            makefileLines.push(`\t@mkdir -p ${destDir}`);
            makefileLines.push(`\t@cp -u ${fileInfo.src} ${fileInfo.dest}`);
        });

        makefileLines.push('');
    }

    getFilesInPackage(binPath, targetName) {
        let packageFiles = [];
        let packageOptions = JSON.parse(JSON.stringify(this._finalConfiguration.package));
        let packageContent = packageOptions.content;

        if ( !packageContent ) {
            return packageFiles;
        }

        let packageHeaders = packageContent.headers;
        if ( packageHeaders ) {
            packageHeaders.includes = packageHeaders.includes.map(includePath => {
                if ( path.isAbsolute(includePath) ) {
                    return includePath;
                }

                return fs.realpathSync(path.join(this._solution._basePath, includePath));
            });

            let includeDirs = packageHeaders.includes.filter(includePath => fs.statSync(includePath).isDirectory());
            let includeFiles = packageHeaders.includes.filter(includePath => fs.statSync(includePath).isFile());
            includeDirs.forEach(includeDir => {
                let dirIncludeFiles = utils.fs.findFiles({
                    path: includeDir,
                    exts: ['h', 'hpp', 'hxx']
                });

                includeFiles = includeFiles.concat(dirIncludeFiles);
            });

            packageFiles = packageFiles.concat(
                    includeFiles.map(includeFile => {
                        return {
                            src: path.relative(this._buildPath, includeFile),
                            dest: '.',
                            relative: path.relative(this._solution._basePath, includeFile)
                        }
                    }));
        }

        let packageTarget = packageContent.target;
        if ( packageTarget ) {
            if ( packageTarget.all ) {
                let targetPath = '';
                if ( this._target === 'executable' ) {
                    targetPath = 'bin';
                }
                else {
                    targetPath = 'lib';
                }
                
                packageFiles.push({
                    src: targetName,
                    dest: targetPath,
                    relative: targetName
                });
            }
        }

        return packageFiles;
    }

    generateCleanLines(makefileLines, targetName) {
        let cleanTargetHint = this.generateMakefileHint({
            type: 'CLEAN',
            message: `${this._name}/${targetName}`
        });

        makefileLines.push(`clean:`);
        makefileLines.push(`\t@${cleanTargetHint}`);
        makefileLines.push(`\t@rm -f ${targetName}`);

        let cleanObjectsHint = this.generateMakefileHint({
            type: 'CLEAN',
            message: `${this._name}/*.o`
        });
        makefileLines.push(`\t@${cleanObjectsHint}`);
        makefileLines.push(`\t@rm -f *.o\n`);
    }

    generateLinkLine(targetName) {
        let dependenciesObjects = '$(OBJS)';
        if ( this._finalConfiguration.linker && this._finalConfiguration.linker.linkDepObjects ) {
            dependenciesObjects += ' $(DEP_OBJS)';
        }

        let linkLines = [
            `${targetName}: ${dependenciesObjects}`,
            this.generateLinkerCommand(targetName)
        ];

        return linkLines.join('\n');
    }

    generateDepObjectFiles() {
        let objectFiles = [];

        for ( let dep of this._linkingDeps ) {
            const depName = dep._name;

            if ( !this._solution._projectsMap.has(depName) ) {
                logger.fatal(`Internal dependency ${depName} is not found`);
                process.exit(1);
            }

            if ( dep._target != 'static_library' ) {
                continue;
            }
            
            const depProject = this._solution._projectsMap.get(depName);
            let relLibPath = utils.fs.relativePath(depProject._buildPath, this._buildPath);

            let depObjectFiles = depProject._objectFiles.map(objectFile => {
                return path.join(relLibPath, objectFile);
            });

            objectFiles = objectFiles.concat(depObjectFiles);
        }

        return objectFiles;
    }

    generateLinkerCommand(targetName) {
        let finalConfiguration = this._finalConfiguration;

        let command = `\t@$(LD) $(OBJS)`;
        if ( this._finalConfiguration.linker && this._finalConfiguration.linker.linkDepObjects ) {
            command += ' $(DEP_OBJS)'
        }

        if ( this._target == 'static_library' ) {
            command = `\t@$(AR)`;
        }
        else if ( this._target == 'dynamic_library' ) {
            command += ' -shared';
        }

        if ( this._target == 'static_library' ) {
            let makefileHint = this.generateMakefileHint({
                type: 'AR',
                message: targetName
            });

            return `\t@${makefileHint}\n${command} rcs ${targetName} $(OBJS)`;
        }

        let libraryPaths = [];
        let inputs = [];

        for ( let dep of this._linkingDeps ) {
            const depName = dep._name;
            if ( !this._solution._projectsMap.has(depName) ) {
                logger.fatal(`Internal dependency ${depName} is not found`);
                process.exit(1);
            }

            if ( dep._target === 'static_library' && 
                    this._finalConfiguration.linker && this._finalConfiguration.linker.linkDepObjects ) {
                continue;
            }
            
            const depProject = this._solution._projectsMap.get(depName);
            let relLibPath = utils.fs.relativePath(depProject._buildPath, this._buildPath);

            libraryPaths.push(`-L"${relLibPath}"`);
            inputs.push(`-l${depProject._name}`);
        }

        for ( let depName of this._externalDeps ) {
            const dep = this._solution._deps[depName];
            
            if ( !dep ) {
                logger.fatal(`External dependency ${depName} is not found`);
                process.exit(1);
            }

            if ( dep.scope != 'system' ) {
                libraryPaths.push(`$(${depName.toUpperCase()}_LIB)`);
            }
            inputs = inputs.concat(dep.inputs.map(input => `-l${input}`));
        }

        let ldflags = '';
        if ( this._target != 'static_library' && finalConfiguration.linker.ldflags ) {
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
            AR: this._finalConfiguration.linker.ar,
            LD: this._finalConfiguration.linker.ld,
            LDFLAGS: this._finalConfiguration.linker.ldflags,
            OS: this._solution._os,
            CPU: this._solution._arch,
            PLATFORM: '$(OS)/$(CPU)'
        };

        return config;
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

        for ( let depName of this._internalDeps ) {
            const dep = this._deps[depName];

            if ( !this._solution._projectsMap.has(depName) ) {
                logger.fatal(`Internal dependency ${depName} is not found`);
                process.exit(1);
            }

            const depProject = this._solution._projectsMap.get(depName);
            if ( depProject._type === 'cpp' ) {
                depProject._finalConfiguration.compiler.includePaths.forEach(includePath => {
                    let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
                    commandArguments.push(`-I${incRelPath}`);
                });
            }
        }

        for ( let depName of this._externalDeps ) {
            const dep = this._solution._deps[depName];
            
            if ( !dep ) {
                logger.fatal(`External dependency ${depName} is not found`);
                process.exit(1);
            }

            if ( dep.scope === 'custom' ) {
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

        for ( let depName of this._internalDeps ) {
            const dep = this._deps[depName];

            if ( !this._solution._projectsMap.has(depName) ) {
                logger.fatal(`Internal dependency ${depName} is not found`);
                process.exit(1);
            }

            const depProject = this._solution._projectsMap.get(depName);
            if ( depProject._type === 'cpp' ) {
                depProject._finalConfiguration.compiler.includePaths.forEach(includePath => {
                    let incRelPath = utils.fs.relativePath(includePath, this._buildPath);
                    commandArguments.push(`-I${incRelPath}`);
                });
            }
        }

        for ( let depName of this._externalDeps ) {
            const dep = this._solution._deps[depName];
            
            if ( !dep ) {
                logger.fatal(`External dependency ${depName} is not found`);
                process.exit(1);
            }

            if ( dep.scope === 'custom' ) {
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

module.exports = CppProject;
