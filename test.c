#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "biblec.h"

// Runtime struct
struct Translation loadedTranslations[10];

int main() {
	int tryFile = 0;
	parseIndexFile(
		&tryFile,
		&loadedTranslations[0],
		"bibles/web.i",
		"bibles/web.t"
	);

	if (tryFile) {
		printf("Index parsing error");
		return 0;
	}

	int tryReader;
	struct Reader reader = reader_new(
		&tryReader,
		&loadedTranslations[0],
		"Gen",
		1,
		2,
		5
	);

	if (tryReader) {
		printf("Verse error");
		return 0;
	}

	int status = 0;
	for (;;) {;;
		status = reader_next(&reader);
		if (status) {break;}
		printf("%s\n", reader.result);
	}

	fclose(reader.file);
}
