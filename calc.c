#include "calc.h"

const char banner[] = 
"Simple Integer Arithmetic Calculator\n"
"Valid chars: 0123456789+-*/()\n"
"Use CTRL+D to quit\n"
;

void mainloop() {
  ast_node* node;
  while (1) {
    printf(">");
    if (!readline()) break;
    node = parse_all();
    if (node) {
      printf("%lld\n", eval_node(node));
      free_node(node);
    }
  }
  printf("Bye\n");
}

int main() {
  printf(banner);
  mainloop();
  return 0;
}
