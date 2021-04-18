#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

// Runtime struct
struct Biblec_translation loadedTranslation;

int main() {
	int tryFile = biblec_parse(
		&loadedTranslation,
		"../bibles/web.i"
	);	

	if (tryFile) {
		puts("Index parsing error");
		return 0;
	}

	struct Biblec_reader reader;
	int tryReader = biblec_new(
		&reader,
		&loadedTranslation,
		"John",
		3,
		1,
		2
	);

	if (tryReader) {
		puts("Verse error");
		return 0;
	}

	while (!biblec_next(&reader)) {
		printf("%s\n", reader.result);
	}

	fclose(reader.file);
}
