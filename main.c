#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "bibles/web.h"

int getLine(char *book, int chapter, int verse) {
	// First, find the book
	int bookID = 0;
	for (int i = 0; i < web -> length; i++) {
		if (strcmp(book, web -> book[i].book) == 0) {
			bookID = i;
		}
	}

	// Next, find the chapter.
	int line = web -> book[bookID].start;
	for (int i = 0; i < chapter - 1; i++) {
		line += web -> book[bookID].chapters[i];
	}

	// Add the line over to the specific verse
	line += verse - 1;
	return line;
}

int main() {
	int line = getLine("Heb", 4, 12);
	int to = 4;

	FILE *file = fopen("bibles/web.txt", "r");

	// Grab the specific line from the file
	char verseText[600];
	int i = 0;
	while (1) {
		fgets(verseText, 600, file);
		if (i >= line + to) {
			break;
		} else if (i >= line) {
			printf("%s", verseText);
		}

		i++;
	}

    fclose(file);
}
