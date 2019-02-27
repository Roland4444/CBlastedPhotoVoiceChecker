int checkin(char * filename)
{
  int res;
  Checker* initial = Checker__();
  res = initial ->checkFile(initial, filename);
  free(initial);
  return res;
  
}
