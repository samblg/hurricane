/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

'use strict'

const config = require('../conf');
const path = require('path');
const util = require('util');
const child_process = require('child_process');

function main() {
    console.log('Starting president ...')
    loadPresident();

    console.log('Starting manager ...')
    loadManagers();
}

function loadPresident() {
    const paths = config.paths;
    const president = config.president;

    const presidentFilePath = getFilePath(paths.president);
    const configFilePath = getFilePath(president.configFile);

    const command = util.format('%s %s', presidentFilePath, configFilePath);
    child_process.exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(error);

            return;
        }

        console.log('President stdout:', stdout);
        console.log('President stderr:', stderr);
    });
}

function loadManagers() {
    let managers = config.managers;
    managers.forEach(manager => {
        loadManager(manager);
    });
}

function loadManager(managerConfig) {
    const paths = config.paths;
    const managerFilePath = getFilePath(paths.manager);
    const configFilePath = getFilePath(managerConfig.configFile);

    const command = util.format('%s %s', managerFilePath, configFilePath);
    child_process.exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(error);

            return;
        }

        console.log('Manager stdout:', stdout);
        console.log('Manager stderr:', stderr);
    });
}

function getFilePath(inputPath) {
    return path.isAbsolute(inputPath) 
        ? inputPath
        : path.join(__dirname, inputPath);
}

main();