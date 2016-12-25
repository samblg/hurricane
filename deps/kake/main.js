'use strict'

const Fiber = require('fibers');
const Solution = require('./lib/solution');
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
    if ( process.argv.length < 3 ) {
        console.log('Usage: kake <Kake Directory>');
        process.exit(-1);
    }

    const kakefileDirectory = util.fs.absolutePath(process.argv[2]);
    logger.debug(`Kakefile directory: ${kakefileDirectory}`);

    let action = DefaultAction;
    if ( process.argv.length >= 4 ) {
        action = process.argv[3];
    }

    logger.debug(`Sepcify action: ${action}`);

    let system = DefaultSystem;
    logger.debug(`Sepcify system: ${system}`);

    let arch = DefaultArch;
    logger.debug(`Sepcify architecture: ${arch}`);

    let configurationName = DefaultConfigurationName;
    if ( process.argv.length >= 5 ) {
        configurationName = process.argv[4];
    }
    logger.debug(`Sepcify configuration: ${configurationName}`);

    let toolchain = DefaultToolchains[system];
    logger.debug(`Sepcify toolchain: ${toolchain.cpp.build}`);

    if ( action == 'config' ) {
        let solutionDir = kakefileDirectory;
        let solution = new Solution(solutionDir);

        logger.info(`Found solution: ${solutionDir}`);
        solution.readConfig({
            os: system,
            arch: arch,
            configuration: configurationName
        });

        solution.parseProjects();
        solution.generateMakefile();
    }
    else if ( action == 'build' ) {
        let solutionDir = kakefileDirectory;
        let solution = new Solution(solutionDir);

        logger.info(`Found solution: ${solutionDir}`);
        solution.readConfig({
            os: system,
            arch: arch,
            configuration: configurationName
        });

        let parallism = 1;
        if ( process.argv.length >= 6 ) {
            parallism = Number(process.argv[5]);
        }

        solution.parseProjects();
        if ( parallism > 1 ) {
            logger.info(`Use parallism: ${parallism}`);
        }
        solution.build(parallism);
    }
    else if ( action == 'install' ) {
        let solutionDir = kakefileDirectory;
        let solution = new Solution(solutionDir);

        logger.info(`Found solution: ${solutionDir}`);
        solution.readConfig({
            os: system,
            arch: arch,
            configuration: configurationName
        });

        let parallism = 1;
        if ( process.argv.length >= 6 ) {
            parallism = Number(process.argv[5]);
        }

        solution.parseProjects();
        if ( parallism > 1 ) {
            logger.info(`Use parallism: ${parallism}`);
        }
        solution.install(parallism);
    }
    else if ( action == 'clean' ) {
        let solutionDir = kakefileDirectory;
        let solution = new Solution(solutionDir);

        logger.info(`Found solution: ${solutionDir}`);
        solution.readConfig({
            os: system,
            arch: arch,
            configuration: configurationName
        });

        solution.parseProjects();
        solution.clean();
    }
    //else if ( action == 'execute' ) {
    //    let solution = new Solution('.');

    //    let command = process.argv[3];
    //    if ( command == 'add' ) {
    //        let itemType = process.argv[4];
    //        if ( itemType == 'project' ) {
    //            let projectName = process.argv[5];
    //            solution.addProject({
    //                name: projectName
    //            });
    //        }
    //    }
    //}
    //else if ( action == 'build' ) {
    //    let solution = new Solution('.');
    //}


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
