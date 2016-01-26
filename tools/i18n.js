'use strict'
let fs = require('fs');

let sourceFile = 'translation.txt';  // utf-8
let enFile = "../Resources/i18n/en.json";
let zhFile = "../Resources/i18n/zh.json";
let headerFile = "../Classes/i18n.h";

let lines = fs.readFileSync('translation.txt', 'utf-8').split('\r\n');
let en = {},
    zh = {},
    keys = "";
lines.map(function(line){
    if(line.length == 0) return null;
    let words = line.split("#").map(s=>s.trim());
    let key = words[0];

    en[key] = words[1];
    zh[key] = words[2];
//    console.log(`static const char* ${key.toUpperCase()} = "${key}";`);
    keys += `\tstatic const char* ${key.toUpperCase()} = "${key}"; \r\n`;
});

let head =
`#pragma once
#include <string>

using namespace std;
namespace R {
	string getString(const char* key);\r\n\r\n`;
let tail =`\r\n};`;

console.log(keys);
fs.writeFile(headerFile, head+keys+tail, err=>{if(err)console.log(err);});
fs.writeFile(enFile, JSON.stringify(en, null, 4), err=>{if(err)console.log(err)});
fs.writeFile(zhFile, JSON.stringify(zh, null, 4), err=>{if(err)console.log(err)});
