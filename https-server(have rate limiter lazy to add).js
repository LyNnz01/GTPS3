"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    Object.defineProperty(o, k2, { enumerable: true, get: function() { return m[k]; } });
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
Object.defineProperty(exports, "__esModule", { value: true });
const https = require('https');
const url = require('url');
const path = require('path');
const fs = require('fs');
var pk = fs.readFileSync('./key.pem');
var pc = fs.readFileSync('./cert.pem');
var optss = { key: pk, cert: pc };
var blacklist = new Map();

const port = process.argv[2] || 443;
require('events').EventEmitter.prototype._maxListeners = 100;

var packet = `server|127.0.0.1\nport|4141\ntype|1\n#maint|Server is currently initializing or re-syncing with sub servers. Please try again in a minute.\n\n\nbeta_server|127.0.0.1\nbeta_port|17091\nbeta_type|1\nbeta2_server|127.0.0.1\nbeta2_port|17099\nbeta2_type|1\nmeta|${Math.floor(Date.now() / 1000)}\nRTENDMARKERBS1001`;

var server = https.createServer(optss,function (req, res) {
  console.log(`Connection from: ${req.connection.remoteAddress} Url: ${req.url} Method: ${req.method}` );
  const parsedUrl = url.parse(req.url);
  let pathname = `.${parsedUrl.pathname}`;
  const ext = path.parse(pathname).ext;
  const map = {
    '.ico': 'image/x-icon',
    '.html': 'text/html',
    '.js': 'text/javascript',
    '.json': 'application/json',
    '.css': 'text/css',
    '.png': 'image/png',
    '.jpg': 'image/jpeg',
    '.wav': 'audio/wav',
    '.mp3': 'audio/mpeg',
    '.svg': 'image/svg+xml',
    '.pdf': 'application/pdf',
    '.doc': 'application/msword'
  };

  if(req.url === "/growtopia/server_data.php")
  {
    res.write(packet, function (err) {
      if (err)
          console.log(err);      
  });
  res.end();
  res.destroy();
  }
  else
  {
    if( req.url.indexOf("/cache") !== -1)
    {
      console.log(`Connection from: ${req.connection.remoteAddress} Url is: ${req.url}`);
  fs.exists(pathname, function (exist) {
    if(!exist) {
      // if the file is not found, return 404
      res.statusCode = 301;
      res.writeHead(301, {
        Location: `https://ubistatic-a.akamaihd.net/0098/28378${req.url}`
      }).end();
            return;
    }

    // read file from file system
    fs.readFile(pathname, function(err, data){
      if(err){
        res.statusCode = 404;
        res.end(`error from loading`);
      } else {
        // if the file is found, set Content-type and send data
        res.setHeader('Content-type', map[ext] || 'text/plain' );
        res.end(data);
      }
    });
  });    

    }
    else if(req.url.indexOf("/0098") !== -1)
    {    
    console.log(`Connection from: ${req.connection.remoteAddress} Url is: ${req.url}`);

      fs.exists(pathname, function (exist) {
        if(!exist) {
          // if the file is not found, return 404
          res.statusCode = 301;
          res.writeHead(301, {
            Location: `https://ubistatic-a.akamaihd.net/0098/28378${req.url}`
          }).end();
              return;
        }
    
        // read file from file system
        fs.readFile(pathname, function(err, data){
          if(err){
            res.statusCode = 404;
            res.end(`error`);
          } else {
            // if the file is found, set Content-type and send data
            res.setHeader('Content-type', map[ext] || 'text/plain' );
            res.end(data);
          }
        });
      });  
    }
    else
    {
      res.statusCode = 301;
      res.end();

    }
  }

});
server.listen(parseInt(port));


server.on("listening", function () { return console.log("[+] started"); });

console.log(`Server listening on port ${port}`);
