const fs = require('fs');
var file, name, folder;

// Simple CLI system
var param = process.argv;
if (param[2] == "-h") {
	console.log(`
		BibleC\n
		node compile.js <file> <name> <output folder>\n
		file: Input file, Ex: ./jubl2000.json\n
		name: web, kjv2000...\n
		output folder: Folder to output to. Ex: bibles/data\n
	`);

	process.exit()
} else if (param.length < 3) {
	console.log("Not enough parameters.\nUse as `node main.js ./jubl2000.json web bible/web`");
	process.exit();
} else if (param.length == 5) {
	file = param[2];
	name = param[3];
	folder = param[4];
}

var bible = require(file).osis.osisText.div;

// Use "global" variables, much more efficient than making
// a copy of a 4mb string 10 times.
var verseFile = ``;
var init = ``;

// First, turn the JSON into a readable struct that
// we will later turn into a C struct.
var bibleData = [];
var linesPassed = 0;
function parseBible() {
	for (var b = 0; b < bible.length; b++) {
		// Push a book object into bibleData
		bibleData.push({
			book: bible[b].osisID,
			start: linesPassed,
			end: 0,
			chapters: []
		});

		// In OSIS, "verses" is on the book key when there is 1 chapter.
		// Else, it is in "chapters"
		if (bible[b].chapter.length != undefined) {
			bibleData[bibleData.length - 1].end = bible[b].chapter.length;
			for (var c = 0; c < bible[b].chapter.length; c++) {
				const verses = bible[b].chapter[c].verse;

				// Set the last created object chapter length
				bibleData[bibleData.length - 1].chapters.push(verses.length);

				loopVerses(verses);
			}
		} else {
			bibleData[bibleData.length - 1].end = 1;
			bibleData[bibleData.length - 1].chapters.push(bible[b].chapter.verse.length);
			loopVerses(bible[b].chapter.verse);
		}
	}
}

function loopVerses(verses) {
	for (var v = 0; v < verses.length; v++) {
		verseFile += verses[v][1] + "\n";
		linesPassed++;
	}
}

function parseBibleData() {
	// Include main C structs.
	init = `
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../core.h"

struct Translation ` + name + ` = {
` + bible.length + `,
{`;

	// Initialize the C struct by generating C code
	for (var i = 0; i < bibleData.length; i++) {
		init += `	{`;
		init += `"` + bibleData[i].book + `", `;

		init += bibleData[i].start + `, `;
		init += bibleData[i].end + `, `;

		init += `{` + bibleData[i].chapters.join(", ") + `}`;
		init += `}`;

		if (i + 1 != bibleData.length) {
			init += `,\n`;
		} else {
			init += `\n}`;
		}
	}

	init += `\n};`;
}

var error = [];

parseBible();
fs.writeFile(
	folder + "/" + name + ".txt",
	verseFile,
	function (err) {error.push(err)}
);

parseBibleData();
fs.writeFile(
	folder + "/" + name + ".c",
	init,
	function (err) {error.push(err)}
);

// TODO: Replace 66 with real calculated number
fs.writeFile(
	folder + "/" + name + ".h",
	`extern struct Translation ` + name + `[66];`,
	function (err) {error.push(err)}
);


console.log("Done. Errors: ", error);
