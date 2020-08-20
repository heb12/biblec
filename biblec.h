int getLine(int *error, struct Translation *translation, char *book, int chapter, int verse);
void getVerses(int *error, char result[][VERSE_LENGTH], struct Translation *translation, char *book, int chapter, int verse, int to);
void parseIndexFile(int *error, struct Translation *translation, char *indexLocation, char *textLocation);
