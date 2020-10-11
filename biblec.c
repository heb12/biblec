#include <stdio.h>
#include <string.h>
#include "main.h"

// Simple+fast minimal function for atoi
int strToInt(char *buf) {
    int ret = 0;
    for (; *buf != '\0'; buf++) {
        ret *= 10;
        ret += *buf - '0';
    }

	return ret;
}

// Parse BibleC index file, see format.md
void parseIndexFile(int *error, struct Translation *translation, char *indexLocation) {
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
	int bookID = BOOK_NOT_FOUND;
	for (int i = 0; i < translation->length; i++) {
		if (!strcmp(book, translation->book[i].name)) {
			bookID = i;
		}
	}

	if (bookID == BOOK_NOT_FOUND) {
	 	return bookID;
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

// Get verses into array
struct Reader reader_new(int *error, struct Translation *translation, char *book, int chapter, int verse, int to) {
	*error = 0;
	struct Reader reader;

    // Check book ID
	int bookID = getBookID(translation, book);
	if (bookID == BOOK_NOT_FOUND) {
		*error = bookID;
		return reader;
	}

	// Check if requested chapter is larger than book length
	if (translation->book[bookID].length < chapter) {
		*error = CHAPTER_TOO_BIG;
		return reader;
	}

	// Grab start line, and add until specified chapter is reached.
    int c = 0;
	int line = translation->book[bookID].start;
	for (; c < chapter - 1; c++) {
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

	reader.book = book;
	reader.chapter = chapter;
	reader.verse = verse;
	reader.to = to;

	reader.linesRead = 0;
    reader.file = fopen(translation->location, "r");

    // Loop through until it gets to the line
	int i = 0;
	char verseText[VERSE_LENGTH];
	while (1) {
        if (i == line) {break;}
		i++;

		if (fgets(verseText, VERSE_LENGTH, reader.file) == NULL) {
            *error = FILE_ERROR;
			return reader;
		}
	}

    return reader;
}
