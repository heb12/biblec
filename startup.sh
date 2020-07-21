# Use this script with a clean installation
mkdir bibles
wget http://api.heb12.com/translations/json/en/web.json
node compiler.js ./web.json web bibles
cc bibles/web.c biblec.c test.c -o test