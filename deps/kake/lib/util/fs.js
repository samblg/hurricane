'use strict';

let fs = require('fs');

function relativePath(originPath, basePath) {
    let absoluteOriginPath = fs.realpathSync(originPath);
    let absoluteBasePath = fs.realpathSync(basePath);

    absoluteOriginPath = replaceAll(absoluteOriginPath, '\\', '/');
    absoluteBasePath = replaceAll(absoluteBasePath, '\\', '/');

    let originPathParts = absoluteOriginPath.split('/');
    let originFileName = originPathParts[originPathParts.length - 1];
    originPathParts = originPathParts.slice(0, originPathParts.length - 1);
    let basePathParts = absoluteBasePath.split('/');

    let commonPrefixIndex = commonPathPrefix(originPathParts, basePathParts);
    let backTimes = basePathParts.length - commonPrefixIndex;
    let backPathParts = makeBackPathParts(backTimes);
    let remainingPathParts = originPathParts.slice(commonPrefixIndex);

    let finalPathParts = backPathParts.concat(remainingPathParts);
    finalPathParts.push(originFileName);

    let finalPath = finalPathParts.join('/');

    return finalPath;
}

function findFiles(options) {
    let foundFiles = [];

    let rootDir = fs.realpathSync(options.path);
    let exts = options.exts;

    let subItems = fs.readdirSync(rootDir);
    let subDirs = [];

    subItems.forEach(subItem => {
        let subItemPath = rootDir + '/' + subItem;

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
    relativePath,
    findFiles
};

//console.log(
//findFiles({
//    path: '../src/detector/caffe-fast-rcnn',
//    exts: ['cpp', 'c']
//}));
