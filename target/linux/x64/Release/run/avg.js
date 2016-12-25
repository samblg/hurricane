'use strict';

const fs = require('fs');

const timestampTexts = fs.readFileSync('timestamp.txt', 'utf8');
const timestamps = timestampTexts.split('\n').map(line => {
    line = line.trim();
    const parts = line.split(' ');

    return {
        start: Number(parts[0]),
        end: Number(parts[1])
    };
});

let totalResponse = 0;
let timestampCount = 0;
timestamps.forEach(timestamp => {
    if ( timestamp.start && timestamp.end ) {
        totalResponse += (timestamp.end - timestamp.start);
        timestampCount ++;
    }
});

console.log(totalResponse);

const avg = totalResponse / timestampCount;
console.log(avg);

// 133
