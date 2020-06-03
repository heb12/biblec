const fs = require('fs');
var file, name;

// Simple CLI system
var param = process.argv;
var folder = "data"
if (param[2] == "-h") {
	console.log(`
		BibleC\n
		node compile.js <file> <output folder>\n
		file: Input file, Ex: ./jubl2000.json\n
		output folder: Folder to output to. Ex: data\n
	`);

	process.exit()
} else if (param.length == 2) {
	console.log("Not enough parameters.\nUse as `node main.js ./jubl2000.json`");
	process.exit()
} else if (param.length == 4) {
	file = param[2];
	name = param[3];
}

var bible = require(file).osis.osisText.div

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

struct Book {
	char *book;
	int start;
	int length;
	int chapters[151];
};

struct Translation {
	int length;
	struct Book book[66];
};

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

	init += `\n};
int main() {
	char book[] = "Heb";
	int chapter = 4;
	int verse = 12;

	// First, find the book
	int bookID = 0;
	for (int i = 0; i < ` + name + `.length; i++) {
		if (strcmp(book, ` + name + `.book[i].book) == 0) {
			bookID = i;
		}
	}

	// Next, find the chapter.
	int line = ` + name + `.book[bookID].start;
	for (int i = 0; i < chapter - 1; i++) {
		line += ` + name + `.book[bookID].chapters[i];
	}

	// Add the line over to the specific verse
	line += verse - 1;

	FILE *file = fopen("` + name + `.txt", "r");

	// Grab the specific line from the file
	char verseText[586];
	int i = 0;
	while (fgets(verseText, sizeof(verseText), file) != NULL) {
		if (i == line) {
			printf("%s\\n", verseText);
			break;
		} else {
			i++;
		}
	}

    fclose(file);
}`;
}

var error = [];

parseBible();
fs.writeFile("web/" + name + ".txt", verseFile, function (err) {error.push(err)});

parseBibleData();
fs.writeFile("web/" + name + ".c", init, function (err) {error.push(err)});

console.log("Done. Errors:", error)
