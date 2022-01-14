'use strict';

class KOptionSuite {
    constructor(options) {
        this._options = options;
        this._allOptions = [];
        this._shortOptions = {};
        this._longOptions = {};
        this._doc = {};
    }

    doc(doc) {
        this._doc = doc;

        return this;
    }

    parse(argv) {
        let self = this;
        let result = {};
        let remaining = [];

        if ( !(argv instanceof Array) ) {
            throw 'The type of arguments must be an array of string';
        }

        for ( let argvIndex = 0; argvIndex < argv.length; argvIndex ++ ) {
            let arg = argv[argvIndex];

            if ( arg.startsWith('--') ) {
                arg = arg.slice(2);
                this.parseLongOption(arg, result);
            }
            else if ( arg.startsWith('-') ) {
                arg = arg.slice(1);
                if ( arg.length > 1 ) {
                    argvIndex += this.parseMultiShortOptions(arg, result, argv, argvIndex);
                }
                else {
                    argvIndex += this.parseShortOptions(arg, result, argv, argvIndex);
                }
            }
            else {
                remaining.push(arg);
            }
        }

        return {
            options: result,
            others: remaining
        };
    }

    parseShortOptions(arg, result, argv, argvIndex) {
        let argName = arg[0];

        let argOption = this._shortOptions[argName];
        if ( !argOption || argOption.type != 'short' ) {
            throw `Invalid option: -${argName} is not a short option`;
        }

        if ( result[argOption.name] !== undefined ) {
            if ( argOption.hasValue ) {
                return 1;
            }

            return 0;
        }

        if ( !argOption.hasValue ) {
            result[argOption.name] = true;

            if ( argOption.handler ) {
                argOption.handler();
            }

            return 0;
        }

        argvIndex ++;
        if ( argvIndex == argv.length ) {
            throw `Invalid option: -${argName} must have a value`;
        }

        let argValue = argv[argvIndex];

        result[argOption.name] = argValue;
        if ( argOption.handler ) {
            argOption.handler(argValue);
        }

        return 1;
    }

    parseMultiShortOptions(arg, result, argv, argvIndex) {
        let self = this;
        let originArgvIndex = argvIndex;

        Array.from(arg).forEach((argName, argIndex) => {
            let argOption = self._shortOptions[argName];

            if ( !argOption || argOption.type != 'short' ) {
                throw `Invalid option: -${argName} is not a short option`;
            }
            
            if ( result[argOption.name] !== undefined ) {
                if ( argIndex < arg.length - 1 ) {
                    throw `Invalid option: -${argName} must be the last arguments in this style.`;
                }

                if ( argOption.hasValue ) {
                    argvIndex ++;
                }

                return 0;
            }

            if ( !argOption.hasValue ) {
                result[argOption.name] = true;

                if ( argOption.handler ) {
                    argOption.handler();
                }

                return 0;
            }

            if ( argIndex < arg.length - 1 ) {
                throw `Invalid option: -${argName} must be the last arguments in this style.`;
            }

            argvIndex ++;
            if ( argvIndex == argv.length ) {
                throw `Invalid option: -${argName} must have a value`;
            }

            let argValue = argv[argvIndex];

            result[argOption.name] = argValue;
            if ( argOption.handler ) {
                argOption.handler(argValue);
            }
        });

        return argvIndex - originArgvIndex;
    }

    parseLongOption(arg, result) {
        let argParts = arg.split('=');

        let argName = argParts[0];
        let argOption = this._longOptions[argName];

        if ( !argOption || argOption.type != 'long' ) {
            throw `Invalid option: --${argName} is not a long option`;
        }

        if ( result[argOption.name] !== undefined ) {
            return;
        }

        if ( !argOption.hasValue && argParts.length == 2 ) {
            throw `Invalid option: --${argName} does not have a value`;
        }

        if ( argOption.hasValue && argParts.length == 1 ) {
            throw `Invalid option: --${argName} must have a value`;
        }

        if ( argOption.hasValue ) {
            result[argOption.name] = argParts[1];
            if ( argOption.handler ) {
                argOption.handler(argParts[1]);
            }
        }
        else {
            result[argOption.name] = true;
            if ( argOption.handler ) {
                argOption.handler();
            }
        }
    }

    on(...options) {
        this._allOptions = options;

        options.forEach(option => {
            option.patterns.forEach(pattern => {
                this.addOption(pattern, {
                    name: option.name,
                    hasValue: option.hasValue,
                    handler: option.handler
                });
            });
        });

        return this;
    }

    addOption(pattern, option) {
        if ( pattern.startsWith('--') ) {
            option.type = 'long';
            pattern = pattern.slice(2);
        }
        else if ( pattern.startsWith('-') ) {
            option.type = 'short';
            pattern = pattern.slice(1);
        }

        if ( !option.hasValue ) {
            option.hasValue = false;
        }

        switch ( option.type ) {
            case 'short':
                this._shortOptions[pattern] = option;
                break;
            case 'long':
                this._longOptions[pattern] = option;
                break;
        }
    }

    toHelpDoc() {
        return [
            this._doc.title,
            'Options',
            this._optionText()
        ].join('\n');
    }

    _optionText() {
        return this._allOptions.map(option => {
            return option.doc.contents.map(line => {
                return `  ${line}`;
            }).join('\n');
        }).join('\n\n');
    }
};

function getopt(options) {
    return new KOptionSuite(options);
}

module.exports = getopt;
