'use strict';

const fs = require('fs');

class KakeCache {
    constructor(options) {
        if ( !options ) {
            this._options = {};
            return;
        }

        this._options = JSON.parse(JSON.stringify(options));
    }

    get options() {
        return this._options;
    }

    read(path) {
        const fileContent = fs.readFileSync(path, 'utf-8');
        this._options = JSON.parse(fileContent);
    }

    write(path) {
        const fileContent = JSON.stringify(this._options);
        fs.writeFileSync(path, fileContent);
    }
}

module.exports = KakeCache;
