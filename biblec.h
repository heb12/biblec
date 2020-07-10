enum Error {
	DEFAULT = 0,
};

struct Translation {
	char *name;
	char *lang;
	char *location;
	int length;
	struct Book {
		char *book;
		int start;
		int length;
		int chapters[151];
	}book[66];
};

int getLine(int *error, struct Translation translation, char *book, int chapter, int verse);
void getVerses(int *error, char result[][600], struct Translation translation, char *book, int chapter, int verse, int to);
