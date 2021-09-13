#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "biblec.h"

struct BiblecTranslation translation;

void test(char name[], int chapter, int from, int to) {
	printf("Parsing: %s %d %d:%d\n", name, chapter, from, to);

	struct BiblecReader reader;
	int tryReader = biblec_new(
		&reader,
		&translation,
		name,
		chapter,
		from,
		to
	);

	if (tryReader) {
		puts("BibleC error");
		biblec_close(&reader);
		return;
	}

	while (biblec_next(&reader)) {
		puts(reader.result);
	}

	biblec_close(&reader);
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

	struct BiblecReader reader;
	clock_t start_time = clock();
	for (int i = 0; i < 100; i++) {
		int tryReader = biblec_new(
			&reader,
			&translation,
			"Exod",
			1,
			1,
			1
		);

		if (tryReader) {
			biblec_close(&reader);
			exit(1);
		}

		while (biblec_next(&reader));
		biblec_close(&reader);
	}

	double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
	printf("Done in %f seconds\n", elapsed_time);

	test("Gen", -1000, -1000, 16);
	test("Gen", 2147483647, 2147483647, 2147483647);
	test("Gen", -10, 0, 0);
	test("Gen", 0, 0, -10);
}

