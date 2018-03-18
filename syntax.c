#include "calc.h"

ast_node* parse_int();
ast_node* parse_bracket_expr();
ast_node* parse_term();
ast_node* parse_binop_rhs(ast_node* lhs, int prec);
ast_node* parse_expr();

#define new_node(x) x = (ast_node*)malloc(sizeof(ast_node));

long long eval_node(ast_node* node) {
  long long val1, val2;
  switch (node->t) {
  case ast_int:
    return node->val;
  case ast_binop:
    val1 = eval_node(node->lhs);
    val2 = eval_node(node->rhs);
    switch (node->op) {
    case t_plus: return val1 + val2;
    case t_minus: return val1 - val2;
    case t_star: return val1 * val2;
    case t_slash: return val1 / val2;
    }
  }
  // We should never get here
  printf("Invalid node type %d\n", node->t);
  return 0;
}

void free_node(ast_node* node) {
  switch (node->t) {
  case ast_int:
    break;
  case ast_binop:
    free_node(node->lhs);
    free_node(node->rhs);
    break;
  default:
    // We should never get here
    printf("Invalid node type %d\n", node->t);
    break;
  }
  free((void*)node);
}

static int precedence(token_t op) {
  switch (op) {
  case t_plus:		return 10;
  case t_minus:		return 10;
  case t_star:		return 20;
  case t_slash:		return 20;
  default:		return -1;
  }
}

static int isop(token_t tt) {
  switch (tt) {
  case t_plus:
  case t_minus:
  case t_star:
  case t_slash:
    return 1;
  default:
    return 0;
  }
}

ast_node* parse_int() {
  // We assume the curtok is already an integer
  ast_node* node;
  new_node(node);
  node->t = ast_int;
  sscanf(curtok, "%lld", &node->val);
  // skip the integer
  gettoken();
  return node;
}

ast_node* parse_bracket_expr() {
  // We assume the curtok is already '('
  ast_node* node;
  // skip '('
  gettoken();
  node = parse_expr();
  if (!node) return NULL;
  if (curtt != t_rbracket) {
    printf("Unexpected %s, Expecting )\n", curtok);
    free_node(node);
    return NULL;
  }
  // skip ')'
  gettoken();
  return node;
}

ast_node* parse_term() {
  ast_node* node;
  switch (curtt) {
  case t_int:
    node = parse_int();
    if (!node) return NULL;
    break;
  case t_lbracket:
    node = parse_bracket_expr();
    if (!node) return NULL;
    break;
  case t_plus:
  case t_minus:
    new_node(node);
    node->t = ast_int;
    node->val = 0;
    node = parse_binop_rhs(node, precedence(curtt));
    if (!node) return NULL;
    break;
  default:
    printf("Unexpected %s, expecting a term\n", curtok);
    return NULL;
  }
  return node;
}

ast_node* parse_binop_rhs(ast_node* lhs, int prec) {
  // We assume the curtok is already an operator
  token_t op;
  ast_node* rhs;
  ast_node* node;
  op = curtt;
  // skip the op
  gettoken();
  rhs = parse_term();
  if (!rhs) {
    free_node(lhs);
    return NULL;
  }
  if (isop(curtt) && precedence(curtt) > prec) {
    rhs = parse_binop_rhs(rhs, precedence(curtt));
    if (!rhs) {
      free_node(lhs);
      return NULL;
    }
  }
  new_node(node);
  node->t = ast_binop;
  node->op = op;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

ast_node* parse_expr() {
  ast_node* node;
  node = parse_term();
  if (!node) return NULL;
  while (isop(curtt)) {
    node = parse_binop_rhs(node, precedence(curtt));
    if (!node) return NULL;
  }
  return node;
}

ast_node* parse_all() {
  ast_node* node;
  gettoken();
  if (curtt == t_eol) {
    return NULL;
  }
  node = parse_expr();
  if (!node) return NULL;
  if (curtt != t_eol) {
    printf("Unexpected %s, expecting <EOL>\n", curtok);
  }
  return node;
}
