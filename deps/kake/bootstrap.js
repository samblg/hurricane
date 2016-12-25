'use strict'

const Fiber = require('fibers');
const Solution = require('./lib/solution');
const Project = require('./lib/project');
const os = require('os');
const util = {
    fs: require('./lib/util/fs'),
    loggers: require('./lib/util/loggers')
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

function main() {
    if ( process.argv.length < 4 ) {
        console.log('Usage: kake-bootstrap <Project Type> <Project Name> [Optional arguments ...]');
        process.exit(-1);
    }

    let projectType = process.argv[2];
    let projectName = process.argv[3];
    logger.info(`Project type: ${projectType}`);
    logger.info(`Project name: ${projectName}`);
    
    if ( projectType === 'solution' ) {
        let basePath = process.argv[4];
        logger.info(`Project base path: ${basePath}`);

        let system = DefaultSystem;
        logger.debug(`Sepcify system: ${system}`);

        let arch = DefaultArch;
        logger.debug(`Sepcify architecture: ${arch}`);

        let configurationName = DefaultConfigurationName;

        Solution.CreateNewSolution({
            name: projectName,
            basePath: basePath,
            system: system,
            arch: arch,
            configurationNames: [ configurationName ]
        });

        process.exit(0);
    }
    else if ( projectType === 'cpp' ) {
        let targetType = process.argv[4];
        logger.info(`Target type: ${targetType}`);

        Project.CreateNewProject({
            name: projectName,
            target: targetType
        });

        process.exit(0);
    }

    console.log('Unknown project type');
    process.exit(-1);


    process.exit();
}

function getSystemName() {
    const osType = os.type();
    if ( osType == 'Linux' ) {
        return 'linux';
    }

    return 'unknown';
}

Fiber(main).run();
