'use strict';

function replaceAll(str, cond, rep) {
    return str.split(cond).join(rep);
}

module.exports = {
    replaceAll
};
