gcc -w src/main.c -Isrc -ldl -o obj/main; gcc  -w -fPIC -ldl -Isrc src/main.c -shared -o obj/libnosess.so
