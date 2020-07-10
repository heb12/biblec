#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblec.h"
#include "bibles/web.h"

struct Translation loadedTranslations[10];

int main() {
	loadedTranslations[0] = web;

	int error;
	char result[30][600];
	getVerses(
		&error,
		result,
		loadedTranslations[0],
		"Heb",
		4,
		12,
		2
	);

	printf("%s\n", result[0]);
	printf("%s\n", result[1]);
}
