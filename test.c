#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

// Runtime struct
struct Translation loadedTranslations;

int main() {
	int tryFile = parseIndexFile(
		&loadedTranslations,
		"../bibles/web.i"
	);	

	if (tryFile) {
		printf("Index parsing error");
		return 0;
	}

	struct Reader reader;
	int tryReader = reader_new(
		&reader,
		&loadedTranslations,
		"John",
		3,
		1,
		0 // Get whole chapter
	);

	if (tryReader) {
		printf("Verse error");
		return 0;
	}

	int status = 0;
	while (1) {
		status = reader_next(&reader);
		if (status) {break;}
		printf("%s\n", reader.result);
	}

	fclose(reader.file);
}
