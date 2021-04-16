#include <stdio.h>
#include <string.h>
#include "main.h"

// Split chars and ints from string
int strToInt(char *buf) {
	int ret = 0;
	for (; *buf != '\0'; buf++) {
		ret *= 10;
		ret += *buf - '0';
	}

	return ret;
}

// Parse BibleC index file, see format.md
int parseIndexFile(struct Translation *translation, char *indexLocation) {
	FILE *index = fopen(indexLocation, "r");
	if (index == NULL) {
		return FILE_NOT_FOUND;
	}

	// If location is never filled in, then assume text
	// file location is in the same folder as the index file.
	strcpy(translation->location, indexLocation);
	translation->location[strlen(indexLocation) - 1] = 't';
	
	char line[INDEX_MAX_LENGTH];
	int book = 0;
	while (fgets(line, INDEX_MAX_LENGTH, index) != NULL) {
		// Remove trailing breakline
		strtok(line, "\n");

		// Pointer
		char *contents = line + 1;

		// Make a duplicate for manipulation
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

			if (!strcmp(afterFirst, "name")) {
				strcpy(translation->name, contents);
			} else if (!strcmp(afterFirst, "lang")) {
				strcpy(translation->lang, contents);
			} else if (!strcmp(afterFirst, "location")) {
				strcpy(translation->location, contents);
			} else if (!strcmp(afterFirst, "length")) {
				translation->length = strToInt(contents);
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

		// Any other line start chars will be skipped
	}

	fclose(index);
	return 0;
}

int getBookID(struct Translation *translation, char *book) {
	// First, find the book
	int bookID = BOOK_NOT_FOUND;
	for (int i = 0; i < translation->length; i++) {
		if (!strcmp(book, translation->book[i].name)) {
			bookID = i;
		}
	}

	return bookID;
}

int reader_next(struct Reader *reader) {
	// Reached end of requested verses
	if (reader->linesRead > reader->to) {
		return -1;
	}

	// End of file
	if (fgets(reader->result, VERSE_LENGTH, reader->file) == NULL) {
		return -1;
	}

	strtok(reader->result, "\n"); // Strip '\n'
	reader->linesRead++;
	return 0;
}

// Create a new reader structure
int reader_new(struct Reader *reader, struct Translation *translation, char *book, int chapter, int verse, int to) {
	int c;

	// Check book ID
	int bookID = getBookID(translation, book);
	if (bookID == BOOK_NOT_FOUND) {
		return BOOK_NOT_FOUND;
	}

	// Check if requested chapter is larger than book length
	if (translation->book[bookID].length < chapter) {
		return CHAPTER_TOO_BIG;
	}

	// Grab start line, and add until specified chapter is reached.
	int line = translation->book[bookID].start;
	for (c = 0; c < chapter - 1; c++) {
		line += translation->book[bookID].chapters[c];
	}

	// When 0 is passed for to, grab the entire chapter.
	if (to == 0) {
		to = translation->book[bookID].chapters[c] - 1;
	} else {
		// Else, "to" refers to how many verse to
		// count in the struct. Set it here.
		to = to - verse;
	}

	// Add the line over to the specific verse
	line += verse - 1;

	reader->book = book;
	reader->chapter = chapter;
	reader->verse = verse;
	reader->to = to;

	reader->linesRead = 0;
	reader->file = fopen(translation->location, "r");
	if (reader->file == NULL) {
		return FILE_ERROR;
	}

	// Loop through until it gets to the line
	int i = 0;
	char verseText[VERSE_LENGTH];
	while (1) {
		if (i == line) {break;}
		i++;

		if (fgets(verseText, VERSE_LENGTH, reader->file) == NULL) {
			return FILE_ERROR;
		}
	}

	return 0;
}
