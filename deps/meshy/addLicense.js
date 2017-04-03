#! /home/kinuxroot/Applications/node/current/bin/node

'use strict';

let fs = require('fs');

let license = fs.readFileSync('LICENSE');
let fileName = process.argv[2];

let fileContent = fs.readFileSync(fileName);
fileContent = license + '\n' + fileContent;

fs.writeFileSync(fileName, fileContent);
