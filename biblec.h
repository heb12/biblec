enum Error {
	DEFAULT=0,
	BOOK_NOT_FOUND=1,
	CHAPTER_TOO_HIGH=2,
	READING_OVERFLOW=3
};

struct Translation {
	char name[20];
	char lang[20];
	char location[20];
	int length;
	struct Book {
		char name[20];
		int start;
		int length;
		int chapters[151];
	}book[66];
};

int getLine(int *error, struct Translation translation, char *book, int chapter, int verse);
void getVerses(int *error, char result[][600], struct Translation translation, char *book, int chapter, int verse, int to);
void parseIndexFile(int *error, struct Translation *translation, char *indexLocation, char *textLocation);
