#ifndef __BIBLEC
#define __BIBLEC 1

#define MAX_CHAPTERS 151
#define INDEX_MAX_LENGTH 500
#define MAX_BOOKS 66

#define VERSE_LENGTH 700
#define MAX_NAME 20
#define MAX_LANG 10
#define MAX_LOCATION 100
#define MAX_BOOK_NAME 30

enum Biblec_error {
	FILE_NOT_FOUND = -1,
	BOOK_NOT_FOUND = -2,
	FILE_OVERFLOW = -3,
	FILE_ERROR = -4,
	BAD_CHAPTER = -5,
	VERSE_ERROR = -6,
};

struct Biblec_reader {
	char *book;
	int chapter;
	int verse;
	int to;
	char result[VERSE_LENGTH];
	int linesRead;
	FILE *file;
};

struct Biblec_translation {
	char name[MAX_NAME];
	char lang[MAX_LANG];
	char location[MAX_LOCATION];
	int length;
	struct Book {
		char name[MAX_BOOK_NAME];
		int start;
		int length;
		int chapters[MAX_CHAPTERS];
	}books[MAX_BOOKS];
};

int biblec_parse(struct Biblec_translation *translation, char *indexLocation);
int biblec_next(struct Biblec_reader *reader);
void biblec_close(struct Biblec_reader *reader);

int biblec_new(
	struct Biblec_reader *reader,
	struct Biblec_translation *translation,
	char *book,
	int chapter,
	int verse,
	int to
);

#endif
