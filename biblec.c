#include <stdio.h>
#include <string.h>
#include "main.h"

// Simple fast minimal function for atoi
int strToInt(char *buf) {
    int ret = 0;
    for (; *buf != '\0'; buf++) {
        ret *= 10;
        ret += *buf - '0';
    }

	return ret;
}

// Parse BibleC index file, see format.md
void parseIndexFile(int *error, struct Translation *translation, char *indexLocation, char *textLocation) {

	// Set text location
	strcpy(translation->location, textLocation);

	FILE *index = fopen(indexLocation, "r");
	if (index == NULL) {
		*error = FILE_NOT_FOUND;
		return;
	}

	char line[INDEX_MAX_LENGTH];
	int book = 0;
	while (fgets(line, INDEX_MAX_LENGTH, index) != NULL) {
		// Remove trailing breakline
		strtok(line, "\n");

		// Pointer
		char *contents = line + 1;

		// General purpose dupilcation for manipulation
		char afterFirst[INDEX_MAX_LENGTH];
		strcpy(afterFirst, contents);

		// Tell the difference between # and @
		if (line[0] == '#') {
			// Copy before ':' to afterFirst
			int c = 0;
			while (*contents != ':') {
				afterFirst[c] = *contents;
				contents++;
				c++;
			}

			afterFirst[c] = '\0';
			contents++; // Increment to skip ':'

			if (strcmp(afterFirst, "name") == 0) {
				strcpy(translation->name, contents);
			} else if (strcmp(afterFirst, "lang") == 0) {
				strcpy(translation->lang, contents);
			} else if (strcmp(afterFirst, "length") == 0) {
				translation->length = strToInt(contents); // TODO: Fix
			}
		} else if (line[0] == '@') {
			char *bookInfo = strtok(afterFirst, " ");
			strcpy(translation->book[book].name, afterFirst);
			bookInfo = strtok(NULL, " ");
			translation->book[book].start = strToInt(bookInfo);
			bookInfo = strtok(NULL, " ");
			translation->book[book].length = strToInt(bookInfo);
		} else if (line[0] == '!') {
			// Loop through chapters and set them in the struct
			int currentChapter = 0;
			char *chapter = strtok(afterFirst, " ");
			while (chapter != NULL) {
				translation->book[book].chapters[currentChapter] = strToInt(chapter);
				chapter = strtok(NULL, " ");
				currentChapter++;
			}

			book++;
		}
	}

	fclose(index);
}

int getBookID(struct Translation *translation, char *book) {
	// First, find the book
	int bookID = -1;
	for (int i = 0; i < translation->length; i++) {
		if (!strcmp(book, translation->book[i].name)) {
			bookID = i;
		}
	}

	// Book int not found
	if (bookID == -1) {
	 	return BOOK_NOT_FOUND;
	}

	return bookID;
}

// Get line from Book, chapter, verse
int getLine(int *error, struct Translation *translation, char *book, int chapter, int verse) {
	*error = 0;

	int bookID = getBookID(translation, book);
	if (bookID == BOOK_NOT_FOUND) {
		*error = bookID;
		return 0;
	}

	// Requested chapter is larger than book legth
	if (translation->book[bookID].length < chapter) {
		*error = -2;
		return 0;
	}

	// Next, find the chapter.
	int line = translation->book[bookID].start;
	for (int i = 0; i < chapter - 1; i++) {
		line += translation->book[bookID].chapters[i];
	}

	// Add the line over to the specific verse
	line += verse - 1;
	return line;
}

// Get verses into array
void getVerses(int *error, char result[][VERSE_LENGTH], struct Translation *translation, char *book, int chapter, int verse, int to) {
	*error = 0;
	int tryLine;
	int line = getLine(&tryLine, translation, book, chapter, verse);

	if (tryLine) {
		*error = tryLine;
		return;
	}

	FILE *file = fopen(translation->location, "r");

	// Grab the specific line from the file
	char verseText[600];
	int i = 0;
	int versesAdded = 0;
	while (1) {
		if (fgets(verseText, 600, file) == NULL) {
			// TODO: Add reading overflow error
			break;
		}

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
