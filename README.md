# BibleC

A minimal Bible format designed for the C language.  
It uses a JS script to convert a OSIS JSON file (http://api.heb12.com/translations/json/)  
to a minimal format designed with minimalism in mind.

It creates an index file, and then a text file. The index file can be used to calculate  
what line a verse starts on in the text file, and it seeks to it.

## Usage:
```
mkdir bibles
wget http://api.heb12.com/translations/json/en/web.json
node compiler.js ./web.json web bibles
tcc biblec.c test.c
./a.out
```
*Note: Any C99 compiler should work.

## Features:
1. First calculates line of Bible verse, then grabs it
2. Support for more than 1 verse (John 3 16-18)
3. Support for Getting entire chapter
4. Load an index file or header file

## API:
```
// Load header translation
struct Biblec_reader reader;
int tryReader = biblec_new(
	&reader,
	&myTranslation,
	"John",
	3,
	16,
	20
);

while (biblec_next(reader)) {
	puts(reader.result);
}
```

```
// Load via index file
// (see test.c)
```
