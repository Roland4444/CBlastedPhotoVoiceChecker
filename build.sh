gcc src/main.c -Isrc -ldl -o obj/main; gcc -g -fPIC -ldl  -Isrc src/main.c -shared -o obj/universal.so
