#include <stdio.h>

#define CLEARTEXT 'A'
#define KEY 'X'

void printchar(char c, const char *s)
{
  printf("%s = '%c' (0x%x)\n", s, c, c);
  
  return;
}

int main(void)
{
  char cleartext = CLEARTEXT;
  printchar(cleartext, "cleartext");

  char key = KEY;
  printchar(key, "key");

  char cipher = cleartext ^ key; /* XOR */
  printchar(cipher, "cipher");

  char new = cipher ^ key;
  printchar(new, "new cleartext");

  return 0;
}
