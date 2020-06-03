# BibleC

A minimal Bible format designed for the C language.  
It uses a JS script to convert a OSIS JSON file (http://api.heb12.com/translations-json/)  
to a C struct and a TXT file. No dependencies, just a simple C  
struct.  

This is not in production yet, so mainly a test here.

Usage:
```
# Get the WEB translation
wget http://api.heb12.com/translations-json/en/web.json

# Compile
mkdir web
node main.js ./web.json web

# Test it out
cd web
tcc en_web.c -o en_web
./en_web
```
-> "For the word of God is living, and active, and sharper than any two-edged sword, and piercing even to the dividing of soul and spirit, of both joints and marrow, and quick to discern the thoughts and intents of the heart."