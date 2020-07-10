#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblec.h"
//#include "bibles/web.h"

void parseIndexFile(int *error, struct Translation *translation, char *indexLocation) {
	*error = 0;

	char line[600];
	FILE *file = fopen(indexLocation, "r");
	int book = 0;
	while (fgets(line, 600, file) != NULL) {
		// Remove trailing breakline
		strtok(line, "\n");

		// Plus 1 char (remove @/#/!)
		char *contents = line + 1;

		// Duplication for editing (after first char)
		char afterFirst[strlen(contents)];
		strcpy(afterFirst, contents);

		// Tell the difference between # and @
		if (line[0] == '#') {
			strtok(afterFirst, ":");
			char *text = contents + strlen(afterFirst) + 1;

			if (strcmp(afterFirst, "name") == 0) {
				strcpy(translation->name, text);
			} else if (strcmp(afterFirst, "lang") == 0) {
				strcpy(translation->lang, text);
			} else if (strcmp(afterFirst, "location") == 0) {
				strcpy(translation->location, text);
			} else if (strcmp(afterFirst, "length") == 0) {
				translation->length = atoi(text); // TODO: Fix
			}
		} else if (line[0] == '@') {
			// The the reading string is reused for this, as it won't be
			// used anymore.
			strtok(afterFirst, " ");
			strcpy(translation->book[book].name, afterFirst);
			strtok(NULL, " ");
			translation->book[book].start = atoi(afterFirst);
			strtok(NULL, " ");
			translation->book[book].length = atoi(afterFirst);

			// Create another duplicate for content after the |
			char afterFirst2[strlen(contents)];
			strcpy(afterFirst2, contents);
		} else if (line[0] == '!') {
			// Loop through chapters and set them in the struct
			int currentChapter = 0;
			char *chapter = strtok(afterFirst, " ");
			while (chapter != NULL) {
				translation->book[book].chapters[currentChapter] = atoi(chapter);
				chapter = strtok(NULL, " ");
				currentChapter++;
			}

			book++;
		}
	}

	fclose(file);
}

int getLine(int *error, struct Translation translation, char *book, int chapter, int verse) {
	*error = 0;

	// First, find the book
	int bookID = -1;
	for (int i = 0; i < translation.length; i++) {
		if (strcmp(book, translation.book[i].name) == 0) {
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
		if (fgets(verseText, 600, file) == NULL) {
			break;
		};

		if (i >= line + to) {
			break;
		} else if (i >= line) {
			// Remove trailing breakline
			strtok(verseText, "\n");
			
			strcpy(result[versesAdded], verseText);
			versesAdded++;
		}

		i++;
	}

    fclose(file);
}
