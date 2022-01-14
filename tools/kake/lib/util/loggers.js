const log4js = require('log4js');
//const fs = require('fs');

//try {
//    fs.mkdirSync('logs');
//}
//catch (e) {
//}
//
//try {
//    fs.accessSync('logs', fs.F_OK);
//}
//catch (e) {
//    console.log('Cannot create logs directory, process exit.');
//    process.exit(1);
//}

log4js.configure({
    appenders: [
        {
            type: 'console',
            layout: {
                type: 'pattern',
                pattern: "[%[%p%]] %m"
            }
      
        },
        //{
        //    type: 'file',
        //    filename: 'logs/kake.log',
        //    maxLogSize: 1024 * 1024 * 16,
        //    backups: 3,
        //    category: 'kake'
        //}
    ]
});

var kakeLogger = log4js.getLogger('kake');
kakeLogger.setLevel('DEBUG');

module.exports = {
    kake: kakeLogger
};
