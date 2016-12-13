'use strict';

const fs = require('fs');
const path = require('path');
const child_process = require('child_process');
const utils = {
    string: require('./util/string'),
    fs: require('./util/fs'),
    loggers: require('./util/loggers')
};

const Project = require('./project');
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

        this._projectNames = solutionConfig.projects;
    }

    readConfig(config) {
        const configFileName = `${config.os}-${config.arch}-${config.configuration}.kake`;
        const configFilePath = path.join(this._solutionPath, 'config', configFileName);

        this._os = config.os;
        this._arch = config.arch;
        this._configurationName = config.configuration;
        this._configuration = require(configFilePath);

        this._buildPath = path.join(this._targetPath, 
                this._os, this._arch, this._configurationName, 'build');
        this._binPath = path.join(this._targetPath, 
                this._os, this._arch, this._configurationName, 'bin');
        this._libPath = path.join(this._targetPath, 
                this._os, this._arch, this._configurationName, 'lib');

        utils.fs.mkdir(this._buildPath);
        utils.fs.mkdir(this._binPath);
        utils.fs.mkdir(this._libPath);

        this.processDeps();
    }

    parseProjects() {
        this._projects = this._projectNames.map(projectName => {
            const projectPath = path.join(this._solutionPath, projectName);

            if ( !utils.fs.exists(projectPath) ) {
                logger.fatal(`Missing project: ${projectName}`);
            }

            const project = new Project(`${projectPath}`, {
                solutionPath: this._solutionPath,
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
    }

    getProject(projectName) {
        return this._projects.find(project => {
            return project._name == projectName;
        });
    }

    replaceVariables(str) {
        str = utils.string.replaceAll(str, '$(Platform)', `${this._os}/${this._arch}`);
        str = utils.string.replaceAll(str, '$(SolutionDir)', this._solutionPath);

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
            }
        });
    }

    addProject(options) {
        let project = Project.CreateNewProject(options);
    }

    generateMakefile() {
        this._projects.forEach(project => {
            project.generateMakefile();
        });

        let makefileLines = ['all:'];
        this._projects.forEach(project => {
            makefileLines.push(`\tcd ${project._name};make`);
        });

        makefileLines.push('clean:');
        this._projects.forEach(project => {
            makefileLines.push(`\tcd ${project._name};make clean`);
        });

        makefileLines.push('install:');
        this._projects.forEach(project => {
            makefileLines.push(`\tcd ${project._name};make install`);
        });

        let makefileContent = makefileLines.join('\n');
        let makefilePath = path.join(this._buildPath, 'Makefile');
        logger.info(`Generate solution makefile: ${makefilePath}`);
        fs.writeFileSync(makefilePath, makefileContent);
    }

    build(parallism) {
        logger.info(`Build solution: ${this._name}`);
        try {
            this._projects.forEach(project => {
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
            this._projects.forEach(project => {
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

    clean() {
        logger.info(`Clean solution: ${this._name}`);
        try {
            this._projects.forEach(project => {
                logger.info(`Clean project: ${project._name}`);
                project.clean();
            });
        }
        catch (e) {
            logger.error('Clean error:');

            let output = '';
            if ( e.stdout ) {
                output += e.stdout.toString();
            }

            if ( e.stderr ) {
                output += e.stderr.toString();
            }
            console.log(output);

            logger.error('Clean failed.');

            process.exit(1);
        }

        logger.info('Clean successfully');
    }
}

Solution.CreateNewSolution = function(options) {
    let solutionName = options.name;
    let basePath = options.basePath;

    let kakefilePath = 'Kakefile';
    logger.info(`Generating ${kakefilePath} ...`);

    let solutionTemplateContent = template.readTemplateFile('solution/Solution.kake');
    let kakefileContent = solutionTemplateContent.replace('$(SolutionName)', solutionName);
    kakefileContent = kakefileContent.replace('$(BasePath)', basePath);

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

module.exports = Solution;
