#include <def.h>
#define soundso "./libsqc.so"
#define photoso "./libBKKAdaptor.so"
#define soundconfig "./cv_configuration.json"
#define photoconfig "./ci_configuration.json"
#define soundindex 0
#define photoindex 1
#define LOGFILE "./log.txt"

Checker* CheckerPtr;

Session* initSession(void* handle, char* symbol, char* config);
void initSessions_(Checker* self);
void loadscheckers_(Checker* self);
int checkFile_(Checker* self, char* filename);
Checker* Checker__();
int checkin(char * filename);
void lastErroeresult(int result);
void freeMem(ContentInfo* ci);
void loadsresultsymbols_(Checker* self);
void readCongig(Session* sess);

void printResult(Checker* self, int sessionindex);
void atomic(SessionValue* sv, FILE* fp);

void patchePhotoSession(Checker* self);
void atomicPhotoPatch(SessionValue* sv, Checker* self, int currentPosition);

Session* initSession(void* handle, char* symbol, char* config)
{
  Session* sess = (Session*)malloc(sizeof(Session));
  create_session load = (create_session)(dlsym(handle, symbol));
  if (!load){
    printf("error loading %s\n\n\n", symbol);
    sess=NULL;
  }
  sess->id = "my_session";
  if (!load(sess, config)){
    printf("error create session\n\n");
    sess = NULL;
    return sess;
  }
  printf("Session creates succesfully\n\n");
  printf("\n\nVERSION=>>\nMajor %d, minor %d, build %d\n\n\n", sess->version.major, sess->version.minor, sess->version.build);
  return sess;
}

ContentInfo* loadContent(char* filename)
{
  ContentInfo* ci = (ContentInfo*)malloc(sizeof(ContentInfo));
  if (read_file_content(filename, &ci->content, &ci->size))
    return ci;
  freeMem(ci);
  return NULL;
}

bool read_file_content(const char* file_path, uint8_t** content, size_t* content_size)
{
  FILE* fd = fopen(file_path, "rb");
  if (fd == NULL){
    fprintf(stderr, "file \"%s\" not found\n", file_path);
    return 0;
  }
  fseek(fd, 0L, SEEK_END);
  (*content_size) = (size_t) ftell(fd);
  rewind(fd);
  (*content) = (uint8_t*) calloc(1, (*content_size));
  fread((*content), (*content_size), 1, fd);
  fclose(fd);
  return 1;
}

void foreach_(Checker* self, char* filename)
{
    FILE* fp;
    char buf[1024];
    if ((fp = fopen(filename, "r")) == NULL){
        perror("fopen source-file");
        return;
    }

    while (fgets(buf, sizeof(buf), fp) != NULL){
        buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
        self->checkFile(self, buf);
    }
    fclose(fp);
    return ;
}

void freeMem(ContentInfo* ci)
{
  free(ci->content);
  free(ci);
}

void parchResultSession(Session* sess)
{
  printf("\n\nPatching last Error!\n\n");
  sess->last_error=2020;
}

void lastErroeresult(int result)
{
  printf("LAST ERROR RESULT = %d", result);
};

int checkFile_(Checker* self, char* filename)
{
  ContentInfo* ci = self->loadContent(filename);
  if (ci == NULL)
    return -1;
  printf("SIZE CONTENT==%d\n\n\n", ci->size);
  self -> log = fopen(LOGFILE, "a");
  if (strstr(filename, ".wav")!=NULL){
    printf("CHECKING WAV FILE %s", filename);
    fprintf(self -> log, "CHECKING WAV FILE %s", filename);
    if (!self->v_check(self->sessions[soundindex], ci->content, ci->size))
      printf("Check failed!\n");
    else
      printf("Checking passed\n");
    printResult(self, soundindex);
    lastErroeresult(self->sessions[soundindex]->last_error);


    parchResultSession(self->sessions[soundindex]);////////////////////
    freeMem(ci);

    return self->sessions[soundindex]->last_error;
  }

  printf("CHECKING photo FILE %s", filename);
  fprintf(self -> log, "CHECKING photo FILE %s", filename);
  if (!self->i_check(self->sessions[photoindex], ci->content, ci->size))
    printf("Check failed!\n");
  else
    printf("Checking passed\n");
  printResult(self, photoindex);
  lastErroeresult(self->sessions[photoindex]->last_error);
  freeMem(ci);
  return self->sessions[photoindex]->last_error;
}

char* getVersion(Checker* self)
{
  v_session_configuration_version get_sess_version = (v_session_configuration_version)(dlsym(self->handles[soundindex],"v_session_configuration_version"));
  return get_sess_version(self->sessions[soundindex]);
}

void initSessions_(Checker* self)
{
  self->sessions[photoindex]=self->initSession(self->handles[photoindex], "i_create_session", photoconfig);
  self->sessions[soundindex]=self->initSession(self->handles[soundindex], "v_create_session", soundconfig);
  int i;
  for (i=0;i<2; i++){
    if (self->sessions[i] == NULL)
      printf("Error create session #%d\n", i);
  }
}

void loadcheckers_(Checker* self)
{
  self->i_check = (BKKCheck)(dlsym(self->handles[photoindex],"i_check_format"));
  self->v_check = (BKKCheck)(dlsym(self->handles[soundindex],"v_check"));
  if (!self->i_check)
    printf("error load i_check");
  if (!self->v_check)
    printf("error load v_check");
}

void loadsresultsymbols_(Checker* self)
{
  self->v_result = (result_session)(dlsym(self->handles[soundindex],"v_result_session"));
  self->i_result = (result_session)(dlsym(self->handles[photoindex],"i_result_session"));

  if (!self->i_result)
    printf("\n\nerror load i_result_session\n\n");
  else
    printf("\n\nsuccess load i_result_session\n\n");
  if (!self->v_result)
    printf("error load v_result_session");
  else
    printf("\n\nsuccess load v_result_session\n\n");
}

Checker* Checker__()
{
  Checker* res = (Checker*)malloc(sizeof(Checker));
  res->loadContent = loadContent;
  res->handles[soundindex] = dlopen(soundso, RTLD_LAZY);
  res->handles[photoindex] = dlopen(photoso, RTLD_LAZY);

  if (!res->handles[soundindex])
    printf("error loading sound so");
  else
    printf("Load success sound so");

  if (!res->handles[photoindex])
    printf("error loading photo so");
  else
    printf("Load success photo so");

  res->initSession = initSession;
  res->initSessions = initSessions_;
  res->loadcheckers = loadcheckers_;
  res->loadContent = loadContent;
  res->checkFile = checkFile_;
  res->loadresult = loadsresultsymbols_;
  res->foreach = foreach_;
  res->initSessions(res);
  res->loadcheckers(res);
  res->loadresult(res);

  readCongig(res->sessions[soundindex]);
  printf("\n\n\nVERSION===>\n\n%s\n\n\n\n",  getVersion(res));
  return res;
}

void foreachGlobal(char* filename)
{
  printf("CALL  GLOBAL FOREACH>>\n\n");
  foreach_(CheckerPtr, filename);
}

void initGlobal()
{
  CheckerPtr = Checker__();
}

int checkin(char* filename)
{
  int res;
  Checker* initial = Checker__();
  res = initial ->checkFile(initial, filename);
  free(initial);
  return res;
}

int checkFileGlobal(char* filename)
{
  return CheckerPtr->checkFile(CheckerPtr, filename);
}

int lets_check(char* filename){
  Checker* chk = Checker__();
  int result___ = checkFile_(chk, filename);
  free(chk);
  return result___;
};

void readCongig(Session* sess)
{
  struct PrivateSession* ps;
  ps = (struct PrivateSession *) sess->payment;
  float f = ps->overload_border.value;
  printf("OVERALOARD BORDER ==>%f", f);
}

void atomic(SessionValue* sv, FILE* fp)
{
  printf("NAME = %s\n", sv->name);
  fprintf(fp, "NAME = %s\n", sv->name);

  printf("ENABLE = %d\n", sv->enable);
  fprintf(fp,"ENABLE = %d\n", sv->enable);

  printf("SessionValueState = %d\n", sv->state);
  fprintf(fp, "SessionValueState = %d\n", sv->state);

  printf("value = %f\n", sv->value);
  fprintf(fp, "value = %f\n", sv->value);

  if (sv->next == NULL){
    fclose(fp);
    return;
  }
  atomic(sv->next, fp);
}


void atomicPhotoPatch(SessionValue* sv, Checker* self, int currentPosition)
{
  int pos = currentPosition + 1;
  if (sv->state == DEFAULT_ERROR_STATE){
    printf("\n\nFailed state found >>Position #%d, Name=%s\n\n", pos, sv->name);
    self->PhotoFailedPosition = pos;
    return;
  }
  if (sv->next == NULL)
    return;
  atomicPhotoPatch(sv->next, self, pos);
}

void patchePhotoSession(Checker* self)
{
  self->PhotoFailedPosition=0;

  SessionValue* sv = (SessionValue*)malloc(sizeof(SessionValue));
  self->i_result(self->sessions[photoindex], &sv);
  atomicPhotoPatch(sv, self, 0);
  self->sessions[photoindex]->last_error = self->PhotoFailedPosition;
}

void printResult(Checker* self, int sessionindex)
{
  printf("Printing Session Result");
  SessionValue* sv = (SessionValue*)malloc(sizeof(SessionValue));
  if  (sessionindex == soundindex)
    self->v_result(self->sessions[soundindex], &sv);
  if  (sessionindex == photoindex)
    self->i_result(self->sessions[photoindex], &sv);
  atomic(sv, self -> log);
}

int main(int argc, char* argv[])
{
  /// Checker* initial = Checker__();
  printf("USING GLOBAL OBJECT\n\n!!!");
  initGlobal();
  for (int i=1; i<argc; i++)
    checkFileGlobal(argv[i]);
   // foreachGlobal(argv[i]);
}
