int getLine(int *error, struct Translation *translation, char *book, int chapter, int verse);
struct Reader newReader(int *error, struct Translation *translation, char *book, int chapter, int verse, int to);
void parseIndexFile(int *error, struct Translation *translation, char *indexLocation, char *textLocation);
int readerNext(struct Reader *reader);
