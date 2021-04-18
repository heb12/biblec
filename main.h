#ifndef __BIBLEC
#define __BIBLEC 1

enum Biblec_max {
	VERSE_LENGTH = 700,
	INDEX_MAX_LENGTH = 500
};

enum BibleC_error {
	FILE_NOT_FOUND = -1,
	BOOK_NOT_FOUND = -2,
	FILE_ERROR = -3,
	CHAPTER_TOO_BIG = -4
};

struct Biblec_reader {
	char *book;
	int chapter;
	int verse;
	int to;
	char result[VERSE_LENGTH];
	FILE *file;
	int linesRead;
};

struct Biblec_translation {
	char name[20];
	char lang[20];
	char location[50];
	int length;
	struct Book {
		char name[20];
		int start;
		int length;
		int chapters[151];
	}book[66];
};

int biblec_parse(struct Biblec_translation *translation, char *indexLocation);
int biblec_new(struct Biblec_reader *reader, struct Biblec_translation *translation, char *book, int chapter, int verse, int to);
int biblec_next(struct Biblec_reader *reader);

#endif
