'use strict';

const Fiber = require('fibers');
const readline = require('readline');

const util = {
    loggers: require('./loggers'),
};
const logger = util.loggers.kake;

class Terminal {
    constructor(options) {
        this._choices = [];
        this._rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout,
            completer: this.completer.bind(this)
        });
    }

    completer(line) {
        return [ this._choices.filter(choice => choice.startsWith(line)), line];
    }

    question(question, choices) {
        let fiber = Fiber.current;
        let result = null;
        if ( !choices ) {
            choices = [];
        }

        this._choices = choices;
        this._rl.question(question, answer => {
            result = answer;

            fiber.run();
        });

        Fiber.yield();

        return result;
    }

    close() {
        this._rl.close();
    }

    choice(question, choices) {
        while ( true ) {
            let answer = this.question(`${question} (? for help): `, choices);
            
            if ( answer === '?' ) {
                printChoices();
                continue;
            }

            if ( choices.indexOf(answer) === -1 ) {
                logger.error(`Unknown choice: ${answer}`);
                printChoices();
                continue;
            }

            return answer;
        }

        function printChoices() {
            console.log('Avaliable choices: ');
            choices.forEach((choice, choiceIndex) => {
                console.log(`  ${choiceIndex + 1}) ${choice}`);
            });
            console.log('');
        }
    }

    text(question, options) {
        if ( !options ) {
            options = {};
        }

        while ( true ) {
            let answer = null;

            if ( options.defaultValue ) {
                answer = this.question(`${question} (${options.defaultValue}): `);
                if ( !answer || !answer.trim() ) {
                    answer = options.defaultValue;
                }
            }
            else {
                answer = this.question(`${question}: `);
            }

            if ( !answer || !answer.trim() && options.notNull ) {
                logger.error('Please input a non-empty answer');
                continue;
            }

            return answer;
        }
    }
}

module.exports = {
    Terminal: Terminal
};
