#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h> 

typedef struct{
	int major;
	int minor;
	int build;
} Version;

typedef struct{
	Version version;
	char *id;
	void *payment;
	int last_error;
} Session;

typedef struct{
    uint8_t* content;
    uint64_t size;
}   ContentInfo;

typedef int (*v_create_session)(struct Session*  , char *);
typedef int (*create_session)(struct Session*  , char *);
typedef int (*BKKCheck)(Session *,  uint8_t * , uint64_t);
typedef int (*BKK)(Session *,  uint8_t * , uint64_t);
