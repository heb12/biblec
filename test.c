#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

// Runtime struct
struct Biblec_translation translation;

int test(struct Biblec_reader *reader, char name[], int chapter, int from, int to) {
	int tryReader = biblec_new(
		reader,
		&translation,
		name,
		chapter,
		from,
		to
	);

	if (tryReader) {
		puts("Verse error");
		return 1;
	}

	while (!biblec_next(reader)) {
		puts(reader->result);
	}

	biblec_close(reader);
}

int main() {
	int tryFile = biblec_parse(
		&translation,
		"../bibles/web.i"
	);

	if (tryFile) {
		puts("Index parsing error");
		return 1;
	}

	struct Biblec_reader reader;
	for (int i = 0; i < 100000; i++) {
		test(&reader, "Gen", 1, 1, 1);
	}
}
