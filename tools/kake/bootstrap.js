'use strict'

const Fiber = require('fibers');
const Solution = require('./lib/solution');
const Project = require('./lib/project').Project;
const os = require('os');
const util = {
    fs: require('./lib/util/fs'),
    loggers: require('./lib/util/loggers'),
    terminal: require('./lib/util/terminal')
};
const logger = util.loggers.kake;

const DefaultAction = 'config';
const DefaultSystem = getSystemName();
const DefaultArch = os.arch();
const DefaultConfigurationName = 'Release';

const DefaultToolchains = {
    linux: {
        c: {
            compiler: 'gcc',
            ld: 'gcc',
            build: 'make'
        },
        cpp: {
            compiler: 'g++',
            ld: 'g++',
            build: 'make'
        }
    },
    win32: {
        c: {
            compiler: 'cl',
            ld: 'cl',
            build: 'msbuild'
        },
        cpp: {
            compiler: 'cl',
            ld: 'cl',
            build: 'msbuild'
        }
    }
};

const ItemTypes = [ 'solution', 'project' ];
const ProjectTypes = [ 'cpp', 'proto' ];
const TargetTypes = [ 'executable', 'dynamic_library', 'static_library' ];

const StandardTerminal = new util.terminal.Terminal();

function main() {
    let bootstrapItem = StandardTerminal.choice('Item type', ItemTypes);
    
    if ( bootstrapItem === 'solution' ) {
        let solutionName = StandardTerminal.text('Solution name', {
            notNull: true
        });
        let solutionVersion = StandardTerminal.text('Solution version', {
            defaultValue: '1.0.0'
        }); 
        let basePath = StandardTerminal.text('Solution base path', {
            notNull: true
        });
        let system = DefaultSystem;
        let arch = DefaultArch;
        let configurationName = DefaultConfigurationName;

        logger.info(`Solution name: ${solutionName}`);
        logger.info(`Solution version: ${solutionVersion}`);
        logger.info(`Project base path: ${basePath}`);
        logger.debug(`Sepcify system: ${system}`);
        logger.debug(`Sepcify architecture: ${arch}`);

        Solution.CreateNewSolution({
            name: solutionName,
            version: solutionVersion,
            basePath: basePath,
            system: system,
            arch: arch,
            configurationNames: [ configurationName ]
        });
    }
    else if ( bootstrapItem === 'project' ) {
        let projectType = StandardTerminal.choice('Project type', ProjectTypes);
        let projectVersion = StandardTerminal.text('Project version', {
            defaultValue: '1.0.0'
        }); 
        let projectName = StandardTerminal.text('Project name: ', {
            notNull: true
        });
        let targetType = StandardTerminal.choice('Target type', TargetTypes);

        logger.info(`Project name: ${projectName}`);
        logger.info(`Project version: ${projectVersion}`);
        logger.info(`Project type: ${projectType}`);
        logger.info(`Target type: ${targetType}`);

        Project.CreateNewProject({
            name: projectName,
            version: projectVersion,
            target: targetType
        });
    }

    process.exit(0);
}

function getSystemName() {
    const osType = os.type();
    if ( osType == 'Linux' ) {
        return 'linux';
    }

    return 'unknown';
}

Fiber(main).run();
