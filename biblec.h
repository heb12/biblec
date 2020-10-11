struct Reader reader_new(int *error, struct Translation *translation, char *book, int chapter, int verse, int to);
void parseIndexFile(int *error, struct Translation *translation, char *indexLocation);
int reader_next(struct Reader *reader);
