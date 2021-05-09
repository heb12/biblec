#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

// Runtime struct
struct Biblec_translation translation;

void test(struct Biblec_reader *reader, char name[], int chapter, int from, int to) {
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
		biblec_close(reader);
		exit(1);
	}

	while (biblec_next(reader)) {
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
	
	test(&reader, "Gen", -1000, -1000, 16);
	test(&reader, "Gen", 2147483647, 2147483647, 2147483647);
}
