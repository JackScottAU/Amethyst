
void* memset(void *b, int c, int len)
{
  int           i;
  unsigned char *p = (unsigned char*)b;
  i = 0;
  while(len > 0)
    {
      *p = c;
      p++;
      len--;
    }
  return(b);
}

void memzero(void* b, int len) {
    memset(b, 0, len);
}
