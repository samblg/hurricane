'use strict'

const Fiber = require('fibers');
const Solution = require('./lib/solution');
const KakeCache = require('./lib/cache');
const os = require('os');
const util = {
    fs: require('./lib/util/fs'),
    loggers: require('./lib/util/loggers')
};
const logger = util.loggers.kake;

const KakeVersion = '0.3.1';
const DefaultAction = 'config';
const DefaultSystem = getSystemName();
const DefaultArch = os.arch();
const DefaultConfigurationName = 'Release';

const DefaultToolchains = {
    linux: {
        c: {
            compiler: 'gcc',
            ar: 'ar',
            ld: 'gcc',
            build: 'make'
        },
        cpp: {
            compiler: 'g++',
            ar: 'ar',
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

function initKakeCache(actionArgs) {
    let kakeCache = new KakeCache();
    try {
        kakeCache.read('KakeCache.json');
    }
    catch (e) {
        if ( actionArgs.length === 0 ) {
            logger.error('Please specify kake solution directory.');
            process.exit(1);
        }

        kakeCache.options.solutionDir = util.fs.absolutePath(actionArgs[0]);
        actionArgs = actionArgs.slice(1);
    }

    kakeCache.write('KakeCache.json');

    return kakeCache;
}

function main() {
    let env = {
        system: DefaultSystem,
        arch: DefaultArch,
        configurationName: DefaultConfigurationName,
        action: DefaultAction
    };

    const options = parseOptions(env);

    const system = env.system;
    const arch = env.arch;
    const configurationName = env.configurationName;

    logger.debug(`Sepcify system: ${system}`);
    logger.debug(`Sepcify architecture: ${arch}`);
    logger.debug(`Sepcify configuration: ${configurationName}`);

    let toolchain = DefaultToolchains[system];
    logger.debug(`Sepcify toolchain: ${toolchain.cpp.build}`);

    if ( options.others.length === 0 ) {
        logger.error('Please specify a action');
        process.exit(1);
    }

    let action = options.others[0];
    logger.debug(`Sepcify action: ${action}`);

    let actionArgs = options.others.slice(1);

    if ( action == 'config' ) {
        const kakeCache = initKakeCache(actionArgs);
        const kakefileDirectory = kakeCache.options.solutionDir;
        logger.debug(`Kakefile directory: ${kakefileDirectory}`);

        let solutionDir = kakefileDirectory;
        let solution = new Solution(solutionDir);

        logger.info(`Found solution: ${solutionDir}`);
        solution.readConfig({
            os: system,
            arch: arch,
            configuration: configurationName
        });

        solution.parseProjects();
        if ( toolchain.cpp.build == 'make' ) {
            solution.generateMakefile();
        }
        else if ( toolchain.cpp.build == 'msbuild' ) {
            solution.generateMsvs();
        }
        else {
            logger.fatal(`Unknown toolchain: ${toolchain.cpp.build}`);
        }
    }
    else if ( action == 'build' ) {
        const kakeCache = initKakeCache(actionArgs);
        const kakefileDirectory = kakeCache.options.solutionDir;
        logger.debug(`Kakefile directory: ${kakefileDirectory}`);

        let solutionDir = kakefileDirectory;
        let solution = new Solution(solutionDir);

        logger.info(`Found solution: ${solutionDir}`);
        solution.readConfig({
            os: system,
            arch: arch,
            configuration: configurationName
        });

        let parallism = 1;
        if ( options.options.parallelism ) {
            parallism = Number(options.options.parallelism);
        }

        solution.parseProjects();
        if ( parallism > 1 ) {
            logger.info(`Use parallism: ${parallism}`);
        }
        solution.build(parallism);
    }
    else if ( action == 'install' ) {
        const kakeCache = initKakeCache(actionArgs);
        const kakefileDirectory = kakeCache.options.solutionDir;
        logger.debug(`Kakefile directory: ${kakefileDirectory}`);

        let solutionDir = kakefileDirectory;
        let solution = new Solution(solutionDir);

        logger.info(`Found solution: ${solutionDir}`);
        solution.readConfig({
            os: system,
            arch: arch,
            configuration: configurationName
        });

        let parallism = 1;
        if ( options.options.parallelism ) {
            parallism = Number(options.options.parallelism);
        }

        solution.parseProjects();
        if ( parallism > 1 ) {
            logger.info(`Use parallism: ${parallism}`);
        }
        solution.install(parallism);
    }
    else if ( action == 'package' ) {
        const kakeCache = initKakeCache(actionArgs);
        const kakefileDirectory = kakeCache.options.solutionDir;
        logger.debug(`Kakefile directory: ${kakefileDirectory}`);

        let solutionDir = kakefileDirectory;
        let solution = new Solution(solutionDir);

        logger.info(`Found solution: ${solutionDir}`);
        solution.readConfig({
            os: system,
            arch: arch,
            configuration: configurationName
        });

        let parallism = 1;
        if ( options.options.parallelism ) {
            parallism = Number(options.options.parallelism);
        }

        solution.parseProjects();
        if ( parallism > 1 ) {
            logger.info(`Use parallism: ${parallism}`);
        }
        solution.doPackage(parallism);
    }
    else if ( action == 'clean' ) {
        const kakeCache = initKakeCache(actionArgs);
        const kakefileDirectory = kakeCache.options.solutionDir;
        logger.debug(`Kakefile directory: ${kakefileDirectory}`);

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

    process.exit();
}

function getSystemName() {
    const osType = os.type();
    if ( osType == 'Linux' ) {
        return 'linux';
    }
    else if ( osType == 'Windows_NT' ) {
        return 'win32';
    }

    return 'unknown';
}

function parseOptions(env) {
    let optionSuite = null;
    try {
        let getopt = require('./lib/util/getopt');
        optionSuite = getopt()
            .on({
                name: 'help',
                patterns: [ '-h', '--help' ],
                handler: help,
                doc: {
                    contents: [
                        '-h, --help:    Show this message.'
                    ]
                }
            }, {
                name: 'version',
                patterns: [ '-v', '--verbose' ],
                handler: showVersion,
                doc: {
                    contents: [
                        '-v, --version: Display kake version.'
                    ]
                }
            }, {
                name: 'config',
                patterns: [ '-c', '--config' ],
                hasValue: true,
                handler(config) {
                    env.configurationName = config;
                },
                doc: {
                    contents: [
                        '-c <config>    ',
                        '--config=      Specify the configuration name.'
                    ]
                }
            }, {
                name: 'arch',
                patterns: [ '-a', '--arch' ],
                hasValue: true,
                handler(arch) {
                    env.arch = arch;
                },
                doc: {
                    contents: [
                        '-a <arch>',
                        '--arch=        Specify the architecture name.'
                    ]
                }
            }, {
                name: 'system',
                patterns: [ '-s', '--system' ],
                hasValue: true,
                handler(system) {
                    env.system = system;
                },
                doc: {
                    contents: [
                        '-s <system>',
                        '--system=      Specify the system name.'
                    ]
                }
            }, {
                name: 'parallelism',
                patterns: [ '-p', '--parallelism' ],
                hasValue: true,
                doc: {
                    contents: [
                        '-p <parallelism>',
                        '--parallelism= Specify the parallelism in building stage.'
                    ]
                }
            })
            .doc({
                title: 'kake [options, ...] <action> [arguments ...]\n' 
            });

        let parseResult = optionSuite.parse(process.argv.slice(2));

        return parseResult;
    }
    catch ( e ) {
        console.log(e);
        help();
    }

    function help() {
        console.log(optionSuite.toHelpDoc());
        process.exit(1);
    }
}

function showVersion() {
    console.log(`kake ${KakeVersion}`);
    process.exit(0);
}

Fiber(main).run();
