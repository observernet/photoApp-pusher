const appRoot = require('app-root-path');
const winston = require('winston');
const process = require('process');
const path = require('path');
 
const { combine, timestamp, label, printf } = winston.format;
 
const myFormat = printf(({ level, message, label, timestamp }) => {
  return `${timestamp} ${level}: ${message}`;
}); 
 
const options = {
  // log파일
  file: {
    level: 'info',
    filename: `${appRoot}/logs/${path.basename(process.argv[1])}.log`,
    handleExceptions: true,
    json: false,
    maxsize: 10485760,
    maxFiles: 20,
    colorize: false,
    format: combine(
      timestamp(),
      myFormat
    )
  },
  console: {
    level: 'debug',
    handleExceptions: true,
    json: false,
    colorize: true,
    format: combine(
      timestamp(),
      myFormat
    )
  }
}
 
let logger = new winston.createLogger({
  transports: [
    new winston.transports.File(options.file)
  ],
  exitOnError: false, 
});
 
if(process.env.NODE_ENV !== 'production'){
  //logger.add(new winston.transports.Console(options.console))
}
 
module.exports = logger;
