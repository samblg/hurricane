'use strict';

let fs = require('fs');

function readTemplateFile(templateName) {
    let templateDir = __dirname + '/../templates/';
    let templatePath = templateDir + templateName;

    return fs.readFileSync(templatePath, 'utf8');
}

module.exports = {
    readTemplateFile
};
