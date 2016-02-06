'use strict'

let fs = require('fs');
let path = require('path');

const dir = '../cocosstudio/';
const hFile = '../Classes/AnimationConfig.h';
const cppFile = '../Classes/AnimationConfig.cpp';


let files = fs.readdirSync(dir);
let csdNames = files.filter( x => x.indexOf('.csd') > -1 );
let csds = csdNames.map( str => fs.readFileSync(path.join(dir, str), 'utf-8') );

let head = `#pragma once

#include <string>

struct AnimationConfig{
    std::string fileName;
    int idleEndFrame;
    int attackStartFrame;
    int attackEndFrame;
    int freezedStartFrame;
    int freezedEndFrame;

`;
let tail = `
};
`

let cppHeader = `
#include "AnimationConfig.h"


`


csds.forEach(function(csd, fileIndex){
    let lines = csd.split("\r\n").filter( line => line.indexOf("EventFrame")>-1 );

    const eventName = /Value="(.*?)"/
    const eventIndex = /Index="(.*?)"/

    let names = {}

    lines.forEach( function (line) {
        let name = line.match(eventName)[1];
        if(name.length!=0 && name!== "attack" && name!=="idleStartFrame"){
            names[name] = line.match(eventIndex)[1];
        }
    } );

    let configName = csdNames[fileIndex].split('.')[0];
    head += `    static const AnimationConfig ${configName};\r\n`;

    cppHeader += `const AnimationConfig AnimationConfig::${configName} = {
    "res/${configName}.csb", ${names.idleEndFrame}, ${names.attackStartFrame}, ${names.attackEndFrame}, ${names.freezedStartFrame}, ${names.freezedEndFrame}
};\r\n`;
    // console.log(names);
    // console.log(`${names.idleEndFrame}`);
})

fs.writeFile(hFile, head+tail, err=>{if(err)console.log(err);});
fs.writeFile(cppFile, cppHeader, err=>{if(err)console.log(err);});
