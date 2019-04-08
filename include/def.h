#include <common.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h> 

#define DEFAULT_ERROR_STATE 2



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

typedef int (*BKK)(Session*,  char*);
typedef int (*i_check_format)(Session*,  uint8_t*, uint64_t);
typedef char* (*v_session_configuration_version)(struct Session*);
typedef bool (*result_session)(Session*, SessionValue**);
struct SoundSessionResult {
  struct SessionValue length;
  struct SessionValue duration;
  struct SessionValue depth;
  struct SessionValue channels;
  struct SessionValue sample_rate;
  struct SessionValue telephony_border;
  struct SessionValue overload;
  struct SessionValue silent;
  struct SessionValue signal_noise;
};

struct PrivateSession {
  uint32_t magic;
  struct IntValue signal_noise;
  struct IntValue duration;
  struct IntValue sample_rate;
  struct IntValue channels;
  struct IntValue depth;
  struct IntValue length;
  struct IntValue frequency;
  struct SingleIntValue telephony_border;
  struct SingleFloatValue overload_border;
  char *processing_output_sound;
  struct SoundSessionResult result;
  char configuration_version[20];
};

struct Answer{
  uint8_t ProblemPos;
  int8_t value;
};

typedef struct Answer Answer;

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
  int PhotoFailedPosition;
  double WrongPhotoValue;
};

typedef struct Checker Checker;
