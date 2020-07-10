#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblec.h"
//#include "bibles/web.h"

struct Translation loadedTranslations[10];

int main() {
	int tryFile;
	parseIndexFile(
		&tryFile,
		&loadedTranslations[0],
		"bibles/web.index"
	);

	int tryStruct;
	char result[30][600];
	getVerses(
		&tryStruct,
		result,
		loadedTranslations[0],
		"Gen",
		1,
		1,
		1
	);

	printf("%s\n", result[0]);
}
