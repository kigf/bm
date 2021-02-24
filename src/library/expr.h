#ifndef EXPR_H_
#define EXPR_H_

#include <stdio.h>
#include "sv.h"
#include "arena.h"
#include "tokenizer.h"

typedef enum {
    EXPR_KIND_BINDING,
    EXPR_KIND_LIT_INT,
    EXPR_KIND_LIT_FLOAT,
    EXPR_KIND_LIT_CHAR,
    EXPR_KIND_LIT_STR,
    EXPR_KIND_BINARY_OP,
    EXPR_KIND_FUNCALL,
} Expr_Kind;

typedef struct Binary_Op Binary_Op;
typedef struct Funcall Funcall;
typedef struct Funcall_Arg Funcall_Arg;

size_t funcall_args_len(Funcall_Arg *args);

typedef union {
    String_View as_binding;
    uint64_t as_lit_int;
    double as_lit_float;
    char as_lit_char;
    String_View as_lit_str;
    Binary_Op *as_binary_op;
    Funcall *as_funcall;
} Expr_Value;

typedef struct {
    Expr_Kind kind;
    Expr_Value value;
} Expr;

void dump_expr(FILE *stream, Expr expr, int level);
void dump_expr_as_dot(FILE *stream, Expr expr);

typedef enum {
    BINARY_OP_PLUS,
    BINARY_OP_MULT,
    BINARY_OP_GT
} Binary_Op_Kind;

#define MAX_PRECEDENCE 2

size_t binary_op_kind_precedence(Binary_Op_Kind kind);
bool token_kind_as_binary_op_kind(Token_Kind token_kind, Binary_Op_Kind *binary_op_kind);
const char *binary_op_kind_name(Binary_Op_Kind kind);
void dump_binary_op(FILE *stream, Binary_Op binary_op, int level);

struct Binary_Op {
    Binary_Op_Kind kind;
    Expr left;
    Expr right;
};

struct Funcall_Arg {
    Funcall_Arg *next;
    Expr value;
};

struct Funcall {
    String_View name;
    Funcall_Arg *args;
};

void dump_funcall_args(FILE *stream, Funcall_Arg *args, int level);
Funcall_Arg *parse_funcall_args(Arena *arena, Tokenizer *tokenizer, File_Location location);
Expr parse_binary_op_from_tokens(Arena *arena, Tokenizer *tokenizer, File_Location location, size_t precedence);
Expr parse_primary_from_tokens(Arena *arena, Tokenizer *tokenizer, File_Location location);
Expr parse_expr_from_tokens(Arena *arena, Tokenizer *tokenizer, File_Location location);
Expr parse_expr_from_sv(Arena *arena, String_View source, File_Location location);

#endif // EXPR_H_
