#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readline();

typedef enum {
  t_unk = -1,
  t_eol = 0,
  t_int,
  t_plus,
  t_minus,
  t_star,
  t_slash,
  t_lbracket,
  t_rbracket
} token_t;

extern char curtok[];
extern token_t curtt;

void gettoken();

typedef enum {
  ast_int = 0,
  ast_binop
} ast_type;

typedef struct _ast_node {
  ast_type t;
  union {
    struct {
      long long val;
    }; // ast_int
    struct {
      token_t op;
      struct _ast_node* lhs;
      struct _ast_node* rhs;
    }; // ast_binop
  };
} ast_node;

ast_node* parse_all();
long long eval_node(ast_node* node);
void free_node(ast_node* node);

