'use strict';

let Fiber = require('fibers');
let child_process = require('child_process');

function execFile() {
    let args = Array.from(arguments);
    let result = {};
    let fiber = Fiber.current;

    args.push((error, stdout, stderr) => {
        result.error = error;
        result.stdout = stdout;
        result.stderr = stderr;

        fiber.run();
    });

    child_process.execFile.apply(null, args);
    Fiber.yield();

    return result;
}

module.exports = {
    execFile
};
