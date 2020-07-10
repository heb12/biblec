#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblec.h"
#include "bibles/web.h"

int getLine(int *error, struct Translation translation, char *book, int chapter, int verse) {
	*error = 0;

	// First, find the book
	int bookID = -1;
	for (int i = 0; i < translation.length; i++) {
		if (strcmp(book, translation.book[i].book) == 0) {
			bookID = i;
		}
	}

	// Book int was not changed and not found
	if (bookID == -1) {
		*error = -1;
		return 1;
	}

	// Next, find the chapter.
	int line = translation.book[bookID].start;
	for (int i = 0; i < chapter - 1; i++) {
		line += translation.book[bookID].chapters[i];
	}

	// Add the line over to the specific verse
	line += verse - 1;
	return line;
}

void getVerses(int *error, char result[][600], struct Translation translation, char *book, int chapter, int verse, int to) {
	*error = 0;
	int tryLine;
	int line = getLine(&tryLine, translation, book, chapter, verse);
	if (tryLine) {
		*error = tryLine;
		return;
	}

	FILE *file = fopen(translation.location, "r");

	// Grab the specific line from the file
	char verseText[600];
	int i = 0;
	int versesAdded = 0;
	while (1) {
		fgets(verseText, 600, file);
		if (i >= line + to) {
			break;
		} else if (i >= line) {
			strcpy(result[versesAdded], verseText);
			versesAdded++;
		}

		i++;
	}

    fclose(file);
}
