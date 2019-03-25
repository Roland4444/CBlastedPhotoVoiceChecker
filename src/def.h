#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h> 

typedef struct
{
  int major;
  int minor;
  int build;
} Version;

typedef struct
{
  Version version;
  char* id;
  void* payment;
  int last_error;
} Session;

typedef struct
{
  uint8_t* content;
  uint64_t size;
}   ContentInfo;

typedef int (*create_session)(struct Session*  , char*);
typedef int (*BKKCheck)(Session*,  uint8_t* , uint64_t);

typedef int (*BKK)(Session *,  char *path_to_config);
typedef int (*i_check_format)(Session *,  uint8_t * , uint64_t);

typedef struct
{
  void(*Checker)(void);    
  void* handles[2];
  Session* sessions[2];
  BKKCheck v_check;
  BKKCheck i_check;
  ContentInfo*(*loadContent)(char * filename);
  void (*initSessions)(struct Checker*);
  Session* (*initSession)(void*, char*, char*);
  void (*loadcheckers)(struct Checker*);
  int (*checkFile)(struct Checker*, char*);
  void (*foreach)(struct Checker*, char*);
} Checker;
