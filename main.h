enum BibleC_max {
	VERSE_LENGTH = 600,
	INDEX_MAX_LENGTH = 500
};

enum BibleC_error {
	FILE_NOT_FOUND = -1,
	BOOK_NOT_FOUND = -2,
	FILE_ERROR = -3,
	CHAPTER_TOO_BIG = -4
};

struct Reader {
	char *book;
	int chapter;
	int verse;
	int to;

	char result[VERSE_LENGTH];

	FILE *file;
	int linesRead;
};

struct Translation {
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

struct Reader reader_new(int *error, struct Translation *translation, char *book, int chapter, int verse, int to);
void parseIndexFile(int *error, struct Translation *translation, char *indexLocation);
int reader_next(struct Reader *reader);
