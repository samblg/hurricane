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

class ProtoProject extends Project {
    constructor(projectDir, options, solution) {
        super(projectDir, options, solution);
    }

    readConfig(config) {
        super.readConfig(config);
        let finalConfiguration = this._finalConfiguration;

        console.log(finalConfiguration);
    }

    generateMakefile() {
        super.generateMakefile();

        let finalConfiguration = this._finalConfiguration;
        let dependencyLines = [];
        let protoOutputs = [];
        finalConfiguration.compiler.src.forEach(srcPath => {
            srcPath = path.join(this._solution._basePath, srcPath);

            let dependencyResult = this.generateDependency(srcPath);
            let compilerCommand = this.generateCompilerCommand(dependencyResult);

            dependencyLines.push(dependencyResult.dependencyLine);
            dependencyLines.push(compilerCommand + '\n\n');

            protoOutputs.push(dependencyResult.headerFile);
            protoOutputs.push(dependencyResult.sourceFile);
        });

        let makefileLines = [
            '-include Makefile.config\n',
            '-include ../Makefile.deps\n'
        ];
        makefileLines.push(`OUTPUTS := ${protoOutputs.join(' \\\n')}\n`);
        this._outputFiles = protoOutputs;

        makefileLines.push(`all: $(OUTPUTS)\n\n`);

        this.generateCollectLines(makefileLines);
        this.generatePackageLines(makefileLines);
        this.generateCleanLines(makefileLines, protoOutputs);

        let makefileContent = `${makefileLines.join('\n')}\n${dependencyLines.join('')}`;
        let makefilePath = `${this._buildPath}/Makefile`;

        fs.writeFileSync(makefilePath, makefileContent);
    }

    generateDependency(srcPath) {
        const srcPathComponents = path.parse(srcPath);

        let finalConfiguration = this._finalConfiguration;
        let cppOutputConf = finalConfiguration.compiler.output.cpp;

        let headersOutputDir = 
            path.relative(this._buildPath,
                path.join(this._solution._basePath, cppOutputConf.headers));
        let sourcesOutputDir = 
            path.relative(this._buildPath,
                path.join(this._solution._basePath, cppOutputConf.sources));
        let srcRelativePath = path.relative(this._buildPath, srcPath);

        let headerFile = `${headersOutputDir}/${srcPathComponents.name}.pb.h`;
        let sourceFile = `${sourcesOutputDir}/${srcPathComponents.name}.pb.cc`;
        let dependency = srcRelativePath;
        let dependencyLine = `${headerFile} ${sourceFile}: ${dependency}\n`;

        return {
            headerFile: headerFile,
            sourceFile: sourceFile,
            dependency: dependency,
            dependencyLine: dependencyLine
        };
    }

    generateCompilerCommand(dependencyResult) {
        let protoPath = path.parse(dependencyResult.dependency).dir;
        let compilerCommand = 
            `@LD_LIBRARY_PATH=$(PROTO_LIB_DIR) $(PROTOC) $< --proto_path=${protoPath} --cpp_out=.`;

        let outputHeaderFile = path.parse(dependencyResult.dependency).base.replace('.proto', '.pb.h');
        let outputSourceFile = path.parse(dependencyResult.dependency).base.replace('.proto', '.pb.cc');

        let collectHeaderCommand = 
            `@mv -f ${outputHeaderFile} ${dependencyResult.headerFile}`;
        let collectSourceCommand = 
            `@mv -f ${outputSourceFile} ${dependencyResult.sourceFile}`;

        let compileHint = this.generateMakefileHint({
            type: 'PROTOC',
            message: dependencyResult.dependency
        });

        return [
            `\t@${compileHint}`,
            `\t${compilerCommand}`,
            `\t${collectHeaderCommand}`,
            `\t${collectSourceCommand}`
        ].join('\n');
    }

    generateCollectLines(makefileLines) {
        let collectTargetHint = this.generateMakefileHint({
            type: 'COLLECT',
            message: this._name
        });

        makefileLines.push(`collect:`);
        makefileLines.push(`\t@${collectTargetHint}`);
        makefileLines.push('');
    }

    generatePackageLines(makefileLines) {
        let packageTargetHint = this.generateMakefileHint({
            type: 'PACKAGE',
            message: this._name
        });

        makefileLines.push(`package:`);
        makefileLines.push(`\t@${packageTargetHint}`);
        makefileLines.push('');
    }

    generateCleanLines(makefileLines, protoOutputs) {
        let cleanTargetHint = this.generateMakefileHint({
            type: 'CLEAN',
            message: this._name
        });

        makefileLines.push(`clean:`);
        makefileLines.push(`\t@${cleanTargetHint}`);
        makefileLines.push(`\t@rm -f *.cc *.h`);
        makefileLines.push(`\t@rm -f ${protoOutputs.join(' ')}`);
    }

    combineConfigFrom(dest, base, extra) {
        if ( base.proto ) {
            base = base.proto;
        }

        dest.compiler = utils.config.combineConfig(
                base.compiler, extra.compiler);
        dest.dependencies = utils.config.combineConfig(
                base.dependencies, extra.dependencies);
        dest.package = utils.config.combineConfig(
                base.package, extra.package);
    }

    generateConfig() {
        return {
            SHELL: '/bin/bash',
            PROTO_HOME: this._finalConfiguration.compiler.basePath,
            PROTO_LIB_DIR: path.join(this._finalConfiguration.compiler.basePath, 'lib'),
            PROTOC: path.join(this._finalConfiguration.compiler.basePath, 'bin', 'protoc'),
            OS: this._solution._os,
            CPU: this._solution._arch,
            PLATFORM: '$(OS)/$(CPU)'
        };
    }
}

module.exports = ProtoProject;
