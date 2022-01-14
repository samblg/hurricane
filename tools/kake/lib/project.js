'use strict';

const path = require('path');
const utils = {
    fs: require('./util/fs'),
    string: require('./util/string'),
    config: require('./util/config'),
    child_process: require('./util/child_process'),
    loggers: require('./util/loggers'),
    xml: require('./util/xml')
};

const template = require('./template');
const fs = require('fs');
const child_process = require('child_process');
const logger = utils.loggers.kake;

class Project {
    constructor(projectDir, options, solution) {
        let self = this;

        this._solution = solution;
        this._solutionPath = options.solutionPath;
        this._innerPath = options.innerPath;
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
        Project.AssignIfNotExist(this._solutionConfiguration, 'options', {});

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

        finalConfiguration.os = config.os;
        finalConfiguration.arch = config.arch;
        finalConfiguration.configurationName = config.configurationName;
        finalConfiguration.basePath = this._solution._basePath;

        this.combineConfigFrom(finalConfiguration,
                this._solutionConfiguration,
                this._projectConfig.common);

        let conditions = this._solutionConfiguration.options;
        Object.keys(conditions).forEach(condition => {
            let conditionValue = this._solutionConfiguration.options[condition];
            if ( conditionValue ) {
                let projectOptionConfs = this._options[condition];
                
                if ( !(projectOptionConfs instanceof Array) ) {
                    projectOptionConfs = [projectOptionConfs];
                }

                projectOptionConfs.forEach(projectOptionConf => {
                    if ( projectOptionConf && projectOptionConf.$eq === conditionValue ) {
                        this.combineConfigFrom(finalConfiguration, finalConfiguration, projectOptionConf);
                    }
                });
            }
        });

        this._buildPath = path.join(this._solution._buildPath, this._name);
        utils.fs.mkdir(this._buildPath);
        this._buildPath = utils.fs.absolutePath(this._buildPath);
        logger.debug(`Use build path: ${this._buildPath}`);
    }

    combineConfigFrom(dest, base, extra) {
        throw new Error('Not implemented');
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
    }

    generateConfig() {
        throw new Error('Not implemented');
    }

    build(parallism) {
        child_process.execSync(`make -s -j ${parallism}`, {
            cwd: this._buildPath,
            stdio: ['inherit', 'inherit', 'inherit' ]
        });
    }

    collect(parallism) {
        child_process.execSync(`make collect -s -j ${parallism}`, {
            cwd: this._buildPath,
            stdio: ['inherit', 'inherit', 'inherit' ]
        });
    }

    doPackage(parallism) {
        child_process.execSync(`make package -s -j ${parallism}`, {
            cwd: this._buildPath,
            stdio: ['inherit', 'inherit', 'inherit' ]
        });
    }

    clean() {
        child_process.execSync('make clean -s', {
            cwd: this._buildPath,
            stdio: ['inherit', 'inherit', 'inherit' ]
        });
    }

    generateMakefileHint(options) {
        return `echo -e "[\\e[32m${options.type}\\033[0m] ${options.message}"`;
    }
}

Project.Create = function(projectDir, options, solution) {
    projectDir = utils.fs.absolutePath(projectDir);
    let kakefile = path.join(projectDir, '/Kakefile');
    let projectConfig = require(kakefile);

    try {
        let projectClass = require(`./modules/${projectConfig.type}`);
        return new projectClass(projectDir, options, solution);
    }
    catch (e) {
        logger.fatal(`Unknown project type: ${projectConfig.type}`);
        logger.error(e);
        process.exit(1);
    }
}

Project.PushIfNotExist = function(container, key, element) {
    if ( container[key] ) {
        if ( container[key].indexOf(element) == -1 ) {
            container[key].push(element);
        }
    }
    else {
        container[key] = [ element ];
    }
}

Project.AssignIfNotExist = function(container, key, value) {
    if ( !container[key] ) {
        container[key] = value;
    }
}

Project.CreateNewProject = function(options) {
    let projectName = options.name;
    let targetType = options.target;

    let projectTemplateContent = template.readTemplateFile('project/CppProject.kake');
    let kakefileContent = projectTemplateContent.replace('##ProjectName##', projectName);
    kakefileContent = kakefileContent.replace('##ProjectVersion##', options.version);
    kakefileContent = kakefileContent.replace('##TargetType##', targetType);

    utils.fs.mkdir(projectName);

    let kakefilePath = path.join(projectName, 'Kakefile');
    fs.writeFileSync(kakefilePath, kakefileContent);
    logger.info(`Generating ${kakefilePath} ...`);
    logger.info('Bootstrap finished.');
}

module.exports = {
    Project: Project
};
