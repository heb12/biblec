#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "biblec.h"

// Runtime struct
struct Translation loadedTranslations;

int main() {
	int tryFile = 0;
	parseIndexFile(
		&tryFile,
		&loadedTranslations,
		"bibles/web.i"
	);	

	if (tryFile) {
		printf("Index parsing error");
		return 0;
	}

	int tryReader;
	struct Reader reader = reader_new(
		&tryReader,
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

