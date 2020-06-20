# Use this script with a clean installation
mkdir bibles
wget http://api.heb12.com/translations/json/en/web.json
node main.js ./web.json web bibles
cc bibles/web.c main.c -o main
