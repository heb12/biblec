enum Max {
	VERSE_LENGTH = 600
};

enum BibleC_error {
	FILE_NOT_FOUND = -1,
	BOOK_NOT_FOUND = -1,
	INDEX_MAX_LENGTH = 600
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
