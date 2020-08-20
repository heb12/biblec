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

	int tryVerses;
	char result[30][VERSE_LENGTH];
	getVerses(
		&tryVerses,
		result,
		&loadedTranslations[0],
		"Gen",
		1,
		1,
		5
	);

	if (tryVerses) {
		printf("Verse error");
		return 0;
	}

	for (int i = 0; i < 5; i++) {
		printf("%d. %s\n", i + 1, result[i]);
	}
}
