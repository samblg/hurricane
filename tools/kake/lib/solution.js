'use strict';

const fs = require('fs');
const path = require('path');
const child_process = require('child_process');
const utils = {
    string: require('./util/string'),
    fs: require('./util/fs'),
    loggers: require('./util/loggers'),
    config: require('./util/config')
};

const generator = require('./generator');

const Project = require('./project').Project;
const KakeCache = require('./cache');
const template = require('./template');
const logger = utils.loggers.kake;

class Solution {
    constructor(solutionDir) {
        this._solutionPath = utils.fs.absolutePath(solutionDir);
        this._kakefile = this._solutionPath + '/Kakefile';

        const depFilePath = path.join(this._solutionPath, 'config/deps.kake');
        if ( utils.fs.exists(depFilePath) ) {
            this._deps = require(depFilePath);
        }
        else {
            this._deps = {};
        }

        let solutionConfig = require(this._kakefile);

        this._name = solutionConfig.name;
        this._version = solutionConfig.version;
        this._type = solutionConfig.type;
        this._targetPath = '.';
        this._basePath = utils.fs.absolutePath(path.join(this._solutionPath, solutionConfig.basePath));
        this._options = solutionConfig.options;
        if ( !this._options ) {
            this._options = {};
        }
        this._projectNames = solutionConfig.common.projects;
        this._packageOptions = solutionConfig.common.package;
    }

    readGlobalConfig(config) {
        logger.info(`Use base configuration: ${config.baseConfiguration}`);
        const configFileName = `${config.os}-${config.arch}-${config.baseConfiguration}.kake`;
        const configFilePath = `../config/cpp/${configFileName}`;

        return require(configFilePath);
    }

    readConfig(config) {
        const configFileName = `${config.os}-${config.arch}-${config.configuration}.kake`;
        const configFilePath = path.join(this._solutionPath, 'config', configFileName);

        this._os = config.os;
        this._arch = config.arch;
        this._configurationName = config.configuration;
        this._configuration = require(configFilePath);
        this._baseConfigurationName = this._configuration.baseConf;
        if ( !this._baseConfigurationName ) {
            this._baseConfigurationName = this._configurationName;
        }
        config.baseConfiguration = this._baseConfigurationName;
        let globalConfig = this.readGlobalConfig(config);

        this._configuration.compiler = utils.config.combineConfig(globalConfig.compiler, this._configuration.compiler);
        this._configuration.linker = utils.config.combineConfig(globalConfig.linker, this._configuration.linker);
        this._configuration.options = utils.config.combineConfig(globalConfig.options, this._configuration.options);
        this._configuration.package = utils.config.combineConfig(globalConfig.package, this._configuration.package);

        this._buildPath = path.join(this._targetPath, 
                this._os, this._arch, this._configurationName, 'build');
        this._binPath = path.join(this._targetPath, 
                this._os, this._arch, this._configurationName, 'bin');
        this._libPath = path.join(this._targetPath, 
                this._os, this._arch, this._configurationName, 'lib');
        this._packagePath = path.join(this._targetPath, 
                this._os, this._arch, this._configurationName, 'package');

        utils.fs.mkdir(this._buildPath);
        utils.fs.mkdir(this._binPath);
        utils.fs.mkdir(this._libPath);
        utils.fs.mkdir(this._packagePath);

        this.processConfigs();
        this.processDeps();
    }

    processConfigs() {
        let conditions = this._options;
        Object.keys(conditions).forEach(condition => {
            let conditionValue = this._configuration.options[condition];
            let solutionOptionConfs = this._options[condition];
            let solutionOptionConf = solutionOptionConfs;
            if ( solutionOptionConfs instanceof Array ) {
                solutionOptionConf = solutionOptionConfs.find(solutionOptionConf => {
                    return solutionOptionConf && solutionOptionConf.$eq === conditionValue;
                });
            }

            if ( solutionOptionConf && solutionOptionConf.$eq === conditionValue ) {
                this._configuration.compiler = utils.config.combineConfig(
                        this._configuration.compiler, solutionOptionConf.compiler);
                this._configuration.linker = utils.config.combineConfig(
                        this._configuration.linker, solutionOptionConf.linker);
                this._configuration.dependencies = utils.config.combineConfig(
                        this._configuration.dependencies, solutionOptionConf.dependencies);
                this._configuration.package = utils.config.combineConfig(
                        this._configuration.package, solutionOptionConf.package);

                if ( solutionOptionConf.projects ) {
                    this._projectNames = this._projectNames.concat(solutionOptionConf.projects);
                }
            }
        });
    }

    parseProjects() {
        this._projects = this._projectNames.map(projectName => {
            const projectPath = path.join(this._solutionPath, projectName);

            if ( !utils.fs.exists(projectPath) ) {
                logger.fatal(`Missing project: ${projectName}`);
            }

            const project = Project.Create(`${projectPath}`, {
                solutionPath: this._solutionPath,
                innerPath: projectName,
                deps: this._deps
            }, this);
            logger.info(`Found project: ${projectName}`);

            project.readConfig({
                os: this._os,
                arch: this._arch,
                configurationName: this._configurationName,
                configuration: this._configuration
            });

            return project;
        });

        this.calculateDependencies();
    }

    getProject(projectName) {
        return this._projects.find(project => {
            return project._name == projectName;
        });
    }

    replaceVariables(str) {
        str = utils.string.replaceAll(str, '$(Platform)', `${this._os}/${this._arch}`);
        str = utils.string.replaceAll(str, '$(SolutionDir)', this._solutionPath);
        str = utils.string.replaceAll(str, '$(SolutionName)', this._name);
        str = utils.string.replaceAll(str, '$(OS)', this._os);
        str = utils.string.replaceAll(str, '$(Arch)', this._arch);
        str = utils.string.replaceAll(str, '$(ConfigurationName)', this._configurationName);

        return str;
    }

    processDeps() {
        Object.keys(this._deps).forEach(depName => {
            let dep = this._deps[depName];

            if ( !(dep.scope) ) {
                dep.scope = 'system';
            }

            if ( !(dep.include) ) {
                dep.include = 'include';
            }

            if ( !(dep.lib) ) {
                dep.lib = 'lib';
            }

            if ( typeof(dep.include) == 'string' ) {
                dep.include = [ dep.include ];
            }
            dep.include = dep.include.map(include => {
                return this.replaceVariables(include);
            });

            if ( typeof(dep.lib) == 'string' ) {
                dep.lib = [ dep.lib ];
            }
            dep.lib = dep.lib.map(lib => {
                return this.replaceVariables(lib);
            });

            if ( dep.basePath ) {
                dep.basePath = utils.fs.absolutePath(this.replaceVariables(dep.basePath));
                if ( !dep.basePath ) {
                    logger.fatal(`Base path of dependency ${depName} is not found`);
                    process.exit(1);
                }

                dep.completeIncludes = dep.include.map(includePath => {
                    return utils.fs.absolutePath(path.join(dep.basePath, includePath));
                });

                dep.completeLibs = dep.lib.map(libPath => {
                    return utils.fs.absolutePath(path.join(dep.basePath, libPath));
                });
            }
        });
    }

    calculateDependencies() {
        let projectsMap = new Map();
        
        this._projects.forEach(project => {
            projectsMap.set(project._name, project);
            project._referencedProjects = new Set();
        });

        this._projects.forEach(project => {
            let dependencies = project._finalConfiguration.dependencies;
            let depNames = Object.keys(dependencies);

            project._internalDeps = new Set();
            project._externalDeps = new Set();
            depNames.forEach(depName => {
                if ( projectsMap.has(depName) ) {
                    project._internalDeps.add(depName);
                    projectsMap.get(depName)._referencedProjects.add(project._name);
                }
                else if ( this._deps[depName] )  {
                    project._externalDeps.add(depName);
                }
                else {
                    logger.fatal(`Dependency ${depName} of project ${project._name} is not found`);
                    process.exit(1);
                }
            });
        });

        this._orderedProjects = this.getProjectOrders(projectsMap);
        this.fillProjectCompleteDeps(projectsMap);
        this._projectsMap = projectsMap;
    }

    getProjectOrders(projectsMap) {
        let noDepsProjects = [];

        for ( let [projectName, project] of projectsMap ) {
            if ( project._referencedProjects.size === 0 ) {
                noDepsProjects.push(project);
            }
        }

        const existedProjects = new Set();
        const orderedProjects = [];
        noDepsProjects.forEach(noDepProject => {
            let orderedDeps = this.orderProjectDeps(noDepProject, projectsMap, new Set());

            orderedDeps.reverse().forEach(orderedDep => {
                if ( !existedProjects.has(orderedDep._name) ) {
                    orderedProjects.push(orderedDep);
                    existedProjects.add(orderedDep._name);
                }
            });
        });

        return orderedProjects;
    }

    fillProjectCompleteDeps(projectsMap) {
        this._orderedProjects.forEach(project => {
            project._directInternalDeps = new Set(project._internalDeps);
            project._directExternalDeps = new Set(project._externalDeps);
            project._indirectInternalDeps = new Set();
            project._indirectExternalDeps = new Set();
            project._linkingDeps = new Set();

            for ( let internalDepName of project._directInternalDeps ) {
                const internalDep = projectsMap.get(internalDepName);

                internalDep._internalDeps.forEach(dep => {
                    if ( !project._internalDeps.has(dep) ) {
                        project._internalDeps.add(dep);
                        project._indirectInternalDeps.add(dep);
                    }
                });
                internalDep._externalDeps.forEach(dep => {
                    if ( !project._externalDeps.has(dep) ) {
                        project._externalDeps.add(dep);
                        project._indirectExternalDeps.add(dep);
                    }
                });

                if ( internalDep._target === 'static_library' ||
                    internalDep._target === 'dynamic_library' ) {
                    project._linkingDeps.add(internalDep);
                }

                internalDep._linkingDeps.forEach(dep => {
                    if ( dep._target === 'dynamic_library' ) {
                        project._linkingDeps.add(dep);
                    }
                    if ( dep._target === 'static_library' &&
                        internalDep._target === 'static_library' ) {
                        project._linkingDeps.add(dep);
                    }
                });
            }
        });
    }

    orderProjectDeps(project, projectsMap, existedProjects) {
        if ( existedProjects.has(project._name) ) {
            return;
        }

        let orderedProjectDeps = [ project ];
        for ( let internalDepName of project._internalDeps ) {
            let internalDepProject = projectsMap.get(internalDepName);

            orderedProjectDeps = orderedProjectDeps.concat(
                    this.orderProjectDeps(internalDepProject, projectsMap, existedProjects));
        }

        return orderedProjectDeps;
    }

    addProject(options) {
        let project = Project.CreateNewProject(options);
    }

    generateMakefile() {
        this._orderedProjects.forEach(project => {
            project.generateMakefile();
        });

        let makefileLines = ['all:'];
        this._orderedProjects.forEach(project => {
            makefileLines.push(`\t@make -s -C ${project._name}`);
        });

        makefileLines.push('clean:');
        this._orderedProjects.forEach(project => {
            makefileLines.push(`\t@make -s -C ${project._name} clean`);
        });

        makefileLines.push('install:');
        this._orderedProjects.forEach(project => {
            makefileLines.push(`\t@make -s -C ${project._name} install`);
        });

        makefileLines.push('package:');
        if ( this._packageOptions && this._packageOptions.includes ) {
            this._packageOptions.includes.forEach(projectPath => {
                let projectIndex = this._projectNames.indexOf(projectPath);
                if ( projectIndex === -1 ) {
                    logger.fatal(`Project ${projectPath} is not found`);
                    process.exit(1);
                }

                let project = this._projects[projectIndex];
                makefileLines.push(`\t@make -s -C ${project._name} package`);
            });
        }
        this.generatePackageLines(makefileLines);

        this.writeDepConfigs();

        let makefileContent = makefileLines.join('\n');
        let makefilePath = path.join(this._buildPath, 'Makefile');
        logger.info(`Generate solution makefile: ${makefilePath}`);
        fs.writeFileSync(makefilePath, makefileContent);
    }

    writeDepConfigs() {
        let depConfigs = this.generateDepConfigs();
        let depConfigsLines = Object.keys(depConfigs).map(configKey => {
            let configValue = depConfigs[configKey];

            if ( configValue instanceof Array ) {
                configValue = configValue.join(' ');
            }

            return `${configKey} := ${configValue}`
        });

        let depConfigContent = depConfigsLines.join('\n');
        let depConfigFilePath = path.join(this._buildPath, 'Makefile.deps');
        logger.info(`Generate dependencies config file: ${depConfigFilePath}`);
        fs.writeFileSync(depConfigFilePath, depConfigContent);
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

            let libValues = dep.lib.map(lib => {
                let libValue = `${dep.basePath}/${dep.lib}`;

                if ( !(libValue.startsWith('/')) ) {
                    libValue = utils.fs.relativePath(`${this._solutionPath}/${libValue}`, this._buildPath);
                }

                return `-L"${libValue}"`;
            });

            configs[includeKey] = includeValues;
            configs[libKey] = libValues;
        });

        return configs;
    }

    generateMsvs() {
        let msvsGenerator = generator.msvs;

        const solutionCachePath = path.join(this._buildPath, 'KakeCache.json');
        const solutionCache = openKakeCache(solutionCachePath);

        let msvsProjectsMap = new Map();
        let msvsProjects = this._orderedProjects.map(project => {
            let msvsProject = msvsGenerator.Project.create({
                keyword: 'Win32Proj',
                rootNamespace: project._name,
                buildPath: this._buildPath,
                path: `${project._name}\\${project._name}.vcxproj`,
                innerPath: project._innerPath,
                projectOptions: project._finalConfiguration,
                cache: solutionCache
            });
            
            logger.info(`Check internal dependencies`);
            for ( let internalDepName of project._directInternalDeps) {
                let msvsInternalDep = msvsProjectsMap.get(internalDepName);

                msvsProject.addInternalDep(msvsInternalDep);
            }

            project.generateMsvs(msvsProject);
            msvsProjectsMap.set(project._name, msvsProject);

            return msvsProject;
        });

        let msvsSolution = msvsGenerator.Solution.create({
            formatVersion: '12.00',
            vsVersion: '12.0.31101.0',
            minVsVersion: '10.0.40219.1',
            projects: msvsProjects,
            cache: solutionCache
        });

        let solutionContent = msvsSolution.toSolutionContent();
        let solutionFilePath = path.join(this._buildPath, `${this._name}.sln`);
        logger.info(`Generate msvs solution file: ${solutionFilePath}`);
        fs.writeFileSync(solutionFilePath, solutionContent);

        solutionCache.write(solutionCachePath);
    }

    build(parallism) {
        logger.info(`Build solution: ${this._name}`);
        try {
            this._orderedProjects.forEach(project => {
                logger.info(`Build project: ${project._name}`);
                project.build(parallism);
            });
        }
        catch (e) {
            logger.error('Build failed.');

            process.exit(1);
        }

        logger.info('Build successfully');
    }

    install(parallism) {
        logger.info(`Install solution: ${this._name}`);
        try {
            this._orderedProjects.forEach(project => {
                logger.info(`Install project: ${project._name}`);
                project.install(parallism);
            });
        }
        catch (e) {
            logger.error('Install failed.');

            process.exit(1);
        }

        logger.info('Install successfully');
    }

    doPackage(parallism) {
        logger.info(`Package solution: ${this._name}`);
        child_process.execSync(`make package -s -j ${parallism}`, {
            cwd: this._buildPath,
            stdio: ['inherit', 'inherit', 'inherit' ]
        });

        logger.info('Package successfully');
    }

    clean() {
        logger.info(`Clean solution: ${this._name}`);
        try {
            this._orderedProjects.forEach(project => {
                logger.info(`Clean project: ${project._name}`);
                project.clean();
            });
        }
        catch (e) {
            logger.error('Clean failed.');

            process.exit(1);
        }

        logger.info('Clean successfully');
    }
    
    generatePackageLines(makefileLines) {
        if ( !this._packageOptions ) {
            makefileLines.push('');
            return;
        }

        let packageDir = utils.fs.relativePath(this._packagePath, this._buildPath);
        
        let filesToPackage = this.getFilesInPackage();
        filesToPackage.forEach(fileInfo => {
            fileInfo.dest = path.join(packageDir, fileInfo.dest, fileInfo.relative);

            let operationHint = this.generateMakefileHint({
                type: 'COPY',
                message: `${fileInfo.src} => ${fileInfo.dest}`
            });

            let destDir = path.parse(fileInfo.dest).dir;
            makefileLines.push(`\t@bash -c '${operationHint}'`);
            makefileLines.push(`\t@mkdir -p ${destDir}`);
            makefileLines.push(`\t@cp -u ${fileInfo.src} ${fileInfo.dest}`);
        });

        let packageType = this._packageOptions.type;
        let packageOutput = this.replaceVariables(this._packageOptions.output);

        if ( packageType === 'tgz' ) {
            let packageOutputPath = path.join('..', `${packageOutput}.tar.gz`);
            let operationHint = this.generateMakefileHint({
                type: 'TAR',
                message: `${packageDir} => ${packageOutputPath}`
            });

            makefileLines.push(`\t@bash -c '${operationHint}'`);
            makefileLines.push(`\t@rm -f ${packageOutputPath}`);
            makefileLines.push(`\t@tar czf ${packageOutputPath} ${packageDir}`);
        }

        makefileLines.push('');
    }

    getFilesInPackage() {
        let packageFiles = [];
        let packageOptions = this._packageOptions;
        if ( !packageOptions ) {
            return packageFiles;
        }

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

                return fs.realpathSync(path.join(this._basePath, includePath));
            });

            let includeDirs = packageHeaders.includes.filter(includePath => fs.statSync(includePath).isDirectory());
            const includeFiles = packageHeaders.includes.filter(includePath => fs.statSync(includePath).isFile());
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
                            relative: path.relative(this._basePath, includeFile)
                        }
                    }));
        }

        let packageDepOptions = packageOptions.deps;
        if ( packageDepOptions && packageDepOptions.includes ) {
            let packageDeps = packageDepOptions.includes.map(depName => {
                let dep = this._deps[depName];
                if ( !dep ) {
                    logger.fatal(`Dep ${depName} is not found`);
                    process.exit(1);
                }

                return dep;
            });

            if ( packageDepOptions.libs === 'copy' ) {
                packageDeps.forEach(dep => {
                    dep.completeLibs.forEach(completeLibDir => {
                        dep.inputs.forEach(input => {
                            let libFileName = `lib${input}.so`;
                            let srcPath = path.join(completeLibDir, libFileName);
                            if ( utils.fs.exists(srcPath) ) {
                                packageFiles.push({
                                    src: srcPath,
                                    dest: '.',
                                    relative: path.join('3rdparty', 'lib', libFileName),
                                });
                            }
                        });
                    });
                });
            }
        }

        return packageFiles;
    }

    generateMakefileHint(options) {
        return `echo -e "[\\e[32m${options.type}\\033[0m] ${options.message}"`;
    }
}

Solution.CreateNewSolution = function(options) {
    let solutionName = options.name;
    let basePath = options.basePath;

    let kakefilePath = 'Kakefile';
    logger.info(`Generating ${kakefilePath} ...`);

    let solutionTemplateContent = template.readTemplateFile('solution/Solution.kake');
    let kakefileContent = solutionTemplateContent.replace('##SolutionName##', solutionName);
    kakefileContent = kakefileContent.replace('##SolutionVersion##', options.version);
    kakefileContent = kakefileContent.replace('##BasePath##', basePath);

    fs.writeFileSync(kakefilePath, kakefileContent);

    utils.fs.mkdir(path.join('config', 'example'));

    let depsPath = path.join('config', 'example', 'deps.kake');
    logger.info(`Generating ${depsPath} ...`);

    let depsContent = template.readTemplateFile(path.join('solution', 'deps.kake'));
    fs.writeFileSync(depsPath, depsContent);

    options.configurationNames.forEach(configurationName => {
        let configFileName = `${options.system}-${options.arch}-${configurationName}.kake`;
        let configPath = path.join('config', 'example', configFileName);
        logger.info(`Generating ${configPath} ...`);

        let configContent = template.readTemplateFile(path.join('solution', 'config-Release.kake'));
        fs.writeFileSync(configPath, configContent);
    });

    logger.info('Bootstrap finished.');

    logger.warn('Please copy the configuration files from config/example to config');
    logger.warn('We suggest to put kake file under config/example rather than config into source code repository');
}

function openKakeCache(path) {
    let kakeCache = new KakeCache();
    try {
        kakeCache.read(path);
    }
    catch (e) {
    }

    return kakeCache;
}

module.exports = Solution;
