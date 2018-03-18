#include "calc.h"

#define TOKEN_BUFSIZE 128

char buffer[1024], *ptr;
char curtok[TOKEN_BUFSIZE], ch;
token_t curtt;

int readline() {
  int ret;
  ch = 0;
  ptr = buffer;
  ret = fgets(buffer, sizeof(buffer), stdin) != NULL;
  if (ret > sizeof(buffer))
    printf("warning: input truncated\n");
  buffer[sizeof(buffer)-1] = '\n';
  return ret;
}

// In the first call to gettoken we get the first char into ch
// Afterwards we assume ch is the current char

//TODO: check curtok boundary

void gettoken() {
  char* tok;
  tok = curtok;
  if (!ch) ch = *ptr++;
  while (ch == ' ') ch = *ptr++;
  if (ch == '\n') {
    *tok++ = '<';
    *tok++ = 'E';
    *tok++ = 'O';
    *tok++ = 'L';
    *tok++ = '>';
    ch = 0;
    curtt = t_eol;
  }
  else if (ch >= '0' && ch <= '9') {
    while (ch >= '0' && ch <= '9') {
      *tok++ = (char)ch;
      ch = *ptr++;
    }
    curtt = t_int;
  }
  else if (ch == '+') {
    *tok++ = (char)ch;
    ch = *ptr++;
    curtt = t_plus;
  }
  else if (ch == '-') {
    *tok++ = (char)ch;
    ch = *ptr++;
    curtt = t_minus;
  }
  else if (ch == '*') {
    *tok++ = (char)ch;
    ch = *ptr++;
    curtt = t_star;
  }
  else if (ch == '/') {
    *tok++ = (char)ch;
    ch = *ptr++;
    curtt = t_slash;
  }
  else if (ch == '(') {
    *tok++ = (char)ch;
    ch = *ptr++;
    curtt = t_lbracket;
  }
  else if (ch == ')') {
    *tok++ = (char)ch;
    ch = *ptr++;
    curtt = t_rbracket;
  }
  else {
    while (ch != '\n') {
      *tok++ = (char)ch;
      ch = *ptr++;
    }
    ch = 0;
    curtt = t_unk;
  }
  *tok = 0;
  //printf("token %d\t%s\n", curtt, curtok);
}
