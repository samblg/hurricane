'use strict';

const logger = require('./loggers').kake;
const fs = require('fs');
const path = require('path');
const uuid = require('uuid');

function exists(filePath) {
    try {
        fs.accessSync(filePath, fs.F_OK);

        return true;
    }
    catch (e) {
        return false;
    }
}

function mkdir(directoryPath) {
    const pathParts = directoryPath.split(path.sep);

    const rootDir = path.parse(directoryPath).root;

    let currentPath = rootDir;
    pathParts.forEach(pathPart => {
        currentPath = path.join(currentPath, pathPart);
        if ( !exists(currentPath) ) {
            logger.info(`Create directory ${currentPath}`);
            fs.mkdirSync(currentPath);
        }
    });
}

function absolutePath(relPath) {
    try {
        return fs.realpathSync(relPath);
    }
    catch (e) {
        return null;
    }
}

function relativePath(originPath, basePath) {
    let absoluteOriginPath = fs.realpathSync(originPath);
    let absoluteBasePath = fs.realpathSync(basePath);

    return path.relative(basePath, originPath);
}

function findFilesToFilters(options) {
    let filterName = options.name;
    let basePath = options.basePath;
    let projectDir = options.projectDir;
    let files = options.files;
    let exts = options.exts;

    let filters = new Map();
    let scannedFile = new Set();
    filters.set(filterName, {
        name: filterName,
        include: filterName,
        files: [],
        exts: exts
    });

    let items = files
    .filter(file => {
        if ( scannedFile.has(file) ) {
            return false;
        }

        scannedFile.add(file);
        return true;
    })
    .map(file => {
        let relPath = relativePath(file, basePath);
        let fileProjRelPath = relativePath(file, projectDir);
        let parseResult = path.parse(relPath);

        let splittedDir = parseResult.dir.split(path.sep);
        {
            let filterDir = filterName;
            splittedDir.forEach(dirName => {
                filterDir = path.join(filterDir, dirName);
                if ( !filters.has(filterDir) ) {
                    filters.set(filterDir, {
                        include: filterDir,
                        files: []
                    });
                }
            });
        }

        let filterDir = path.join(filterName, parseResult.dir);
        if ( !filters.has(filterDir) ) {
            filters.set(filterDir, {
                include: filterDir,
                files: []
            });
        }

        let filterInfo = filters.get(filterDir);
        filterInfo.files.push(fileProjRelPath);
    });

    return filters;
}

function findFiles(options) {
    let foundFiles = [];

    let rootDir = fs.realpathSync(options.path);
    let exts = options.exts;

    let subItems = fs.readdirSync(rootDir);
    let subDirs = [];

    subItems.forEach(subItem => {
        let subItemPath = path.join(rootDir, subItem);

        let itemStatus = fs.statSync(subItemPath);
        if ( itemStatus.isFile() && endsWithExts(subItem, exts) ) {
            foundFiles.push(subItemPath);
        }
        else if ( itemStatus.isDirectory() ) {
            subDirs.push(subItemPath);
        }
    });

    subDirs.forEach(subDir => {
        foundFiles = foundFiles.concat(findFiles({
            path: subDir,
            exts: exts
        }));
    });

    return foundFiles;
}

function endsWithExts(fileName, exts) {
    exts = exts.map(ext => `.${ext}`);
    return exts.reduce((prev, ext) => {
        if ( !prev ) {
            return fileName.endsWith(ext);
        }

        return true;
    }, false);
}

function commonPathPrefix(pathParts1, pathParts2) {
    let minLength = min(pathParts1.length, pathParts2.length);

    for ( let partIndex = 0; partIndex < minLength; partIndex ++ ) {
        if ( pathParts1[partIndex] != pathParts2[partIndex] ) {
            return partIndex;
        }
    }

    return minLength;
}

function makeBackPathParts(backTimes) {
    let parts = [];
    for ( let backTime = 0; backTime < backTimes; backTime ++ ) {
        parts.push('..');
    }

    return parts;
}

function replaceAll(str, cond, rep) {
    let origin = str;
    while ( true ) {
        let result = origin.replace(cond, rep);
        if ( result == origin ) {
            return result;
        }

        origin = result;
    }
}

function min(a, b) {
    if ( a < b ) {
        return a;
    }

    return b;
}

module.exports = {
    absolutePath,
    relativePath,
    exists,
    mkdir,
    findFiles,
    findFilesToFilters
};

//console.log(
//findFiles({
//    path: '../src/detector/caffe-fast-rcnn',
//    exts: ['cpp', 'c']
//}));
