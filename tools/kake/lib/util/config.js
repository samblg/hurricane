'use strict';

function combineConfig(config1, config2) {
    let finalConfig = Object.create(null);

    if ( config1 ) {
        Object.keys(config1).forEach(key => {
            finalConfig[key] = config1[key];
        });
    }

    if ( !config2 ) {
        return finalConfig;
    }

    Object.keys(config2).forEach(key => {
        if ( !finalConfig[key] ) {
            finalConfig[key] = config2[key];
            if ( finalConfig[key] instanceof Array ) {
                let newOption = config2[key].concat([]);

                let defaultConfigIndex = newOption.indexOf('$(Inherit)');
                if ( defaultConfigIndex != -1 ) {
                    finalConfig[key] = newOption.slice(0, defaultConfigIndex)
                        .concat(newOption.slice(defaultConfigIndex + 1));
                }

                return;
            }
        }

        if ( finalConfig[key] instanceof Array ) {
            let newOption = config2[key].concat([]);

            let defaultConfigIndex = newOption.indexOf('$(Inherit)');
            if ( defaultConfigIndex != -1 ) {
                if ( !config1[key] ) {
                    config1[key] = [];
                }

                newOption = newOption.slice(0, defaultConfigIndex)
                    .concat(config1[key])
                    .concat(newOption.slice(defaultConfigIndex + 1));
            }

            finalConfig[key] = newOption;
        }
        else {
            finalConfig[key] = config2[key];
            if ( finalConfig[key] === '$(Inherit)' ) {
                finalConfig[key] = config1[key];
            }
        }
    });

    return finalConfig;
}

module.exports = {
    combineConfig
};
