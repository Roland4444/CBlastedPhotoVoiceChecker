#include <common.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h> 


typedef struct Version Version;

typedef struct Session Session; 

typedef struct SessionValue SessionValue;


struct ContentInfo
{
  uint8_t* content;
  uint64_t size;
};
typedef struct ContentInfo ContentInfo; 

typedef int (*create_session)(struct Session*, char*);
typedef int (*BKKCheck)(Session*,  uint8_t*, uint64_t);

typedef int (*BKK)(Session*,  char *path_to_config);
typedef int (*i_check_format)(Session*,  uint8_t*, uint64_t);
typedef char* (*v_session_configuration_version)(struct Session*);
typedef bool (*result_session)(Session*, SessionValue**);


struct Checker
{
  void(*Checker)(void);    
  void* handles[2];
  Session* sessions[2];
  FILE* log;
  BKKCheck v_check;
  BKKCheck i_check;
  result_session v_result;
  result_session i_result;
  ContentInfo*(*loadContent)(char* filename);
  void (*initSessions)(struct Checker*);
  Session* (*initSession)(void*, char*, char*);
  void (*loadcheckers)(struct Checker*);
  void (*loadresult)(struct Checker*);
  void (*printresult)(Session*);
  int (*checkFile)(struct Checker*, char*);
  void (*foreach)(struct Checker*, char*);
};

typedef struct Checker Checker;
