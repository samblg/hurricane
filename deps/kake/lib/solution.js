'use strict';

let fs = require('fs');
let utils = {
    string: require('./util/string')
};

let Project = require('./project');
let template = require('./template');

class Solution {
    constructor(solutionDir) {
        this._basePath = solutionDir;
        this._completePath = fs.realpathSync(solutionDir);
        this._kakefile = this._completePath + '/Kakefile';
        this._deps = require(this._completePath + '/deps.kake');
        this.processDeps();
        let solutionConfig = require(this._kakefile);

        this._name = solutionConfig.name;
        this._version = solutionConfig.version;
        this._type = solutionConfig.type;
        this._projects = solutionConfig.projects.map(project => {
            return new Project(`${this._basePath}/${project}`, {
                solutionPath: this._completePath,
                deps: this._deps
            }, this);
        });
    }

    getProject(projectName) {
        return this._projects.find(project => {
            return project._name == projectName;
        });
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
                return utils.string.replaceAll(include, '${Platform}', 'linux/x64');
            });
            dep.lib = utils.string.replaceAll(dep.lib, '${Platform}', 'linux/x64');
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
        let makefilePath = `${this._projects[0]._targetPath}/build/linux/x64/Release/Makefile`;
        console.log(makefilePath);
        fs.writeFileSync(makefilePath, makefileContent);
    }
}

Solution.CreateNewSolution = function(options) {
    let solutionName = options.name;

    let kakefilePath = solutionName + '/Kakefile';
    console.log(`Generating ${kakefilePath} ...`);

    let solutionTemplateContent = template.readTemplateFile('solution/SolutionSample.kake');
    let kakefileContent = solutionTemplateContent.replace('SampleSolution', solutionName);

    try {
        fs.mkdirSync(solutionName);
        console.log(`Created directory ${solutionName}`);
    }
    catch (e) {
        console.log(`The solution dir ${solutionName} has been existed. Skip create directory.`);
    }

    fs.writeFileSync(kakefilePath, kakefileContent);

    let depsPath = solutionName + '/deps.kake';
    console.log(`Generating ${depsPath} ...`);

    let depsContent = template.readTemplateFile('solution/deps.kake');
    fs.writeFileSync(depsPath, depsContent);

    console.log('Bootstrap finished.');
}

module.exports = Solution;
