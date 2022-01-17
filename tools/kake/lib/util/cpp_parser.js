'use strict';

let fs = require('fs');

class CppParser {
    parseFile(filePath) {
        let fileContent = fs.readFileSync(filePath, 'utf8');

        let fileMode = this.getFileMode(fileContent);
        let lines = fileContent.split(fileMode.lineSeperator);

        this.parseLines(lines);
    }

    getFileMode(fileContent) {
        if ( fileContent.indexOf('\r\n') ) {
            return CppParser.FileMode.DosFile
        }

        return CppParser.FileMode.UnixFile;
    }

    parseLines(lines) {
        let lineCount = lines.length;

        for ( let lineIndex = 0; lineIndex != lineCount; ++ lineIndex ) {
            let line = lines[lineIndex];

            let lineLength = line.length;
            console.log(`${lineIndex}: ${line.length}`);
            for ( let charIndex = 0; charIndex != lineLength; ++ charIndex ) {
                let character = line[charIndex];
            }
        }
    }
}

CppParser.FileMode = {
    UnixFile: {
        type: 'UnixFile',
        lineSeperator: '\n'
    },
    DosFile: {
        type: 'DosFile',
        lineSeperator: '\r\n'
    }
};

CppParser.lexSyntax = [
    {
        name: 'string_concat',
        start: '##',
        pattern: /##/
    },
    {
        name: 'make_character',
        start: '#@',
        pattern: /#@/
    },
    {
        name: 'define',
        start: '#define',
        pattern: /\b#define\b/
    },
    {
        name: 'if',
        start: '#if',
        pattern: /\b#if\b/
    },
    {
        name: 'ifdef',
        start: '#ifdef',
        pattern: /\b#ifdef\b/
    },
    {
        name: 'ifndef',
        start: '#ifndef',
        pattern: /\b#ifndef\b/
    },
    {
        name: 'else',
        start: '#else',
        pattern: /\b#else\b/
    },
    {
        name: 'endif',
        start: '#endif',
        pattern: /\b#endif\b/
    },
    {
        name: 'error',
        start: '#error',
        pattern: /\b#error\b/
    },
    {
        name: 'make_string',
        start: '#',
        pattern: /#/
    },
    {
        name: 'string',
        start: '"',
        pattern: /"[^"]"/
    },
    {
        name: 'cpp_comment',
        start: '//',
        pattern: /\/\/.*$/
    },
    {
        name: 'c_comment_start',
        start: '/*',
        pattern: /\/\*/
    },
    {
        name: 'c_comment_end',
        start: '*/',
        pattern: /\*\//
    },
    {
        name: 'bracket_open',
        start: '(',
        pattern: /\(/
    },
    {
        name: 'bracket_close',
        start: ')',
        pattern: /\)/
    },
    {
        name: 'comma',
        start: ',',
        pattern: /,/
    },
    {
        name: 'plus',
        start: '+',
        pattern: /\+/
    },
    {
        name: 'minus',
        start: '-',
        pattern: /-/
    },
    {
        name: 'multiply',
        start: '*',
        pattern: /\*/
    },
    {
        name: 'devide',
        start: '/',
        pattern: /\//
    },
    {
        name: 'mod',
        start: '%',
        pattern: /%/
    },
    {
        name: 'logic_and',
        start: '&&',
        pattern: /&&/
    },
    {
        name: 'logic_or',
        start: '||',
        pattern: /||/
    },
    {
        name: 'logic_not',
        start: '!',
        pattern: /!/
    },
    {
        name: 'bit_and',
        start: '&',
        pattern: /&/
    },
    {
        name: 'bit_or',
        start: '|',
        pattern: /|/
    },
    {
        name: 'bit_not',
        start: '~',
        pattern: /~/
    },
    {
        name: 'bit_xor',
        start: '^',
        pattern: /\^/
    },
    {
        name: 'word',
        start: /[\w]/,
        pattern: /[\w][\w\d]*/
    }
];

let cppParser = new CppParser();
cppParser.parseFile('/home/kinuxroot/Projects/authen/AuthenCoreSDK/src/common/caffe-fast-rcnn/caffe/layer.cpp');
