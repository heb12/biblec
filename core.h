struct Translation {
	int length;
	struct Book {
		char *book;
		int start;
		int length;
		int chapters[151];
	}book[66];
};
