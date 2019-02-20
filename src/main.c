#include <def.h>
#define soundso "./libcv.so"
#define photoso "./libBKKAdaptor.so"
#define soundconfig "./cv_configuration.json"
#define photoconfig "./ci_configuration.json"
#define soundindex 0
#define photoindex 1


void * handles[2];
Session * sessions[2];
ContentInfo * loadContent(char * filename);
BKK v_check;
BKK i_check;

void initSessions();
int read_file_content(const char *file_path, uint8_t **content, size_t *content_size);
int checkFile(char * filename);
void foreach(char * filename);
void init();
void loadscheckers();
Session * initSession(void* handle, char * symbol, char * config);


int main(int argc, char *argv[])
{
  init();
  printf("%d\n",checkFile("./tested.wav"));
  for (int i=1; i<argc; i++)
    foreach(argv[i]);
}

Session * initSession(void* handle, char * symbol, char * config){
  Session* sess = (Session*)malloc(sizeof(Session));
  create_session load = (create_session)(dlsym(handle, symbol));
  if (!load){
    printf("error loading v_create_session\n\n");
    sess=NULL;
  }
  sess->id="my_session";
  if (!load(sess, config)){
    printf("error create session\n\n");
    sess=NULL;
    return sess;
  }
  printf("Session creates succesfully\n\n");
  return sess;
}

void init()
{
  printf("initialisation....\n\n");
  handles[soundindex] = dlopen(soundso, RTLD_LAZY);
  handles[photoindex] = dlopen(photoso, RTLD_LAZY);

  if (!handles[soundindex])
    printf("error loading sound so");
  else
    printf("Load success sound so");

  if (!handles[photoindex])
    printf("error loading photo so");
  else
    printf("Load success photo so");
  initSessions();
  loadscheckers();
}

ContentInfo * loadContent(char * filename)
{
  ContentInfo * ci=(ContentInfo*)malloc(sizeof(ContentInfo));
  read_file_content(filename, &ci ->content, &ci->size);
  return ci;
}

void initSessions()
{
  sessions[photoindex]=initSession(handles[photoindex], "i_create_session", photoconfig);
  sessions[soundindex]=initSession(handles[soundindex], "v_create_session", soundconfig);
  int i;
  for (i=0;i<2; i++){
    if (sessions[i]==NULL)
      printf("Error create session #%d\n", i);
  }
}

int read_file_content(const char *file_path, uint8_t **content, size_t *content_size) {
  FILE *fd = fopen(file_path, "rb");
  if (fd == NULL) {
    fprintf(stderr, "file \"%s\" not found\n", file_path);
    return 0;
  }
  fseek(fd, 0L, SEEK_END);
  (*content_size) = (size_t) ftell(fd);
  rewind(fd);
  (*content) = (uint8_t *) calloc(1, (*content_size));
  fread((*content), (*content_size), 1, fd);
  fclose(fd);
  return 1;
}

void foreach(char * filename){
    FILE* fp;
    char buf[1024];
    if ((fp = fopen(filename, "r")) == NULL)
    { /* Open source file. */
        perror("fopen source-file");
        return;
    }

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
        checkFile(buf);
    }
    fclose(fp);
    return ;
}

void loadscheckers(){
  i_check = (BKKCheck)(dlsym(handles[photoindex],"i_check_format"));
  v_check = (BKKCheck)(dlsym(handles[soundindex],"v_check"));
  if (!i_check)
    printf("error load i_check");
  if (!v_check)
    printf("error load v_check");

}

int checkFile(char * filename)
{
  BKKCheck check;
  ContentInfo * ci;
  if (loadContent(filename) == NULL)
    return -1;
  ci = loadContent(filename);
  if (strstr(filename, "wav")!=NULL){
    printf("CHECKING WAV FILE");
    check=v_check;
    if (!check(sessions[soundindex], ci ->content, ci ->size))
    {
	printf("Check failed!\n");
	return -3;
      }
    else printf("Checking passed\n");
    return 0;
  }
  printf("CHECKING photo FILE");
  check=i_check;
    if (!check(sessions[photoindex], ci ->content, ci ->size))
    {
	printf("Check failed!\n");
	return -3;
      }
  else printf("Checking passed\n");
  return 0;
}
