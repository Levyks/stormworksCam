const express = require('express');
const cors = require('cors');
const imgStringfier = require('./build/Release/module');

var app=express();

app.use(
    cors({
        origin: 'https://lua.flaffipony.rocks',
        optionsSuccessStatus: 200
    })
)

app.get('/', function(req,res){
    counter+=1;
    res.end(pixelData);
});

let port = parseInt(process.argv[2]);
let width = parseInt(process.argv[3]);
let height = parseInt(process.argv[4]);

var server = app.listen(port,function() {});

console.log("Server running at port " + port);

let pixelData;
let counter = 0;


let total = width * height * 6;

imgStringfier.startProcessing("http://192.168.1.143:8080/video", width, height, (env, value) => { pixelData=value; });

let fpsChecker = setInterval(()=>{
    process.stdout.clearLine();
    process.stdout.cursorTo(0);
    process.stdout.write(counter + " FPS");
    counter = 0;
},1000)






