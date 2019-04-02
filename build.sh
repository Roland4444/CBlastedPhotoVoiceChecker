gcc src/main.c -Iinclude -ldl -o obj/main; gcc -g -fPIC -ldl -Iinclude src/main.c -shared -o obj/libuk.so
