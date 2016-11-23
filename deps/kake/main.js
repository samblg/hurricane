'use strict'

let Fiber = require('fibers');
let Solution = require('./lib/solution');

function main() {
    if ( process.argv.length < 3 ) {
        console.log('Usage: kake <action>');
        process.exit(-1);
    }

    let action = process.argv[2];

    if ( action == 'bootstrap' ) {
        if ( process.argv.length < 5 ) {
            console.log('Usage: kake bootstrap <projectType> <projectName>');
            process.exit(-1);
        }

        let projectType = process.argv[3];
        let solutionName = process.argv[4];

        if ( projectType != 'solution' ) {
            console.log('Kake only support bootstrap solution now');
            process.exit(-1);
        }

        let solution = Solution.CreateNewSolution({
            name: solutionName
        });
    }
    else if ( action == 'generate' ) {
        let solutionDir = '.';
        if ( process.argv.length > 3 ) {
            solutionDir = process.argv[3];
        }

        let solution = new Solution(solutionDir);

        solution.generateMakefile();
    }
    else if ( action == 'execute' ) {
        let solution = new Solution('.');

        let command = process.argv[3];
        if ( command == 'add' ) {
            let itemType = process.argv[4];
            if ( itemType == 'project' ) {
                let projectName = process.argv[5];
                solution.addProject({
                    name: projectName
                });
            }
        }
    }
    else if ( action == 'build' ) {
        let solution = new Solution('.');
    }


    process.exit();
}

Fiber(main).run();
