# BibleC

A minimal Bible format designed for the C language.  
It uses a JS script to convert a OSIS JSON file (http://api.heb12.com/translations/json/)  
to a minimal format designed with minimalism in mind.

It creates an index file, and then a text file. The index file can be used to calculate  
what line a verse starts on in the text file, and it seeks to it.

## Usage:
Use the `build.sh` script when pulled for the first time.

-> "For the word of God is living, and active, and sharper than any two-edged sword, and piercing even to the dividing of soul and spirit, of both joints and marrow, and quick to discern the thoughts and intents of the heart."

## Features:
1. First calculates line of Bible verse, then grabs it.
2. Support for more than 1 verse (John 3 16-18)

## Future:
2. Write compiler in something lighter than NodeJS