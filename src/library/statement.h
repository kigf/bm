#ifndef STATEMENT_H_
#define STATEMENT_H_

#include "./bm.h"
#include "./expr.h"
#include "./linizer.h"

typedef struct Statement Statement;
typedef struct Block Block;

typedef enum {
    STATEMENT_KIND_EMIT_INST,
    STATEMENT_KIND_BIND_LABEL,
    STATEMENT_KIND_BIND_CONST,
    STATEMENT_KIND_BIND_NATIVE,
    STATEMENT_KIND_INCLUDE,
    STATEMENT_KIND_ASSERT,
    STATEMENT_KIND_ENTRY,
    STATEMENT_KIND_BLOCK,
} Statement_Kind;

typedef struct {
    Inst_Type type;
    Expr operand;
} Emit_Inst;

typedef struct {
    String_View name;
} Bind_Label;

typedef struct {
    String_View name;
    Expr value;
} Bind_Const;

typedef struct {
    String_View name;
    Expr value;
} Bind_Native;

typedef struct {
    String_View path;
} Include;

typedef struct {
    Expr condition;
} Assert;

typedef struct {
    Expr value;
} Entry;

typedef union {
    Emit_Inst as_emit_inst;
    Bind_Label as_bind_label;
    Bind_Const as_bind_const;
    Bind_Native as_bind_native;
    Include as_include;
    Assert as_assert;
    Entry as_entry;
    Block *as_block;
} Statement_Value;

struct Statement {
    Statement_Kind kind;
    Statement_Value value;
    File_Location location;
};

struct Block {
    Statement statement;
    Block *next;
};

typedef struct {
    Block *begin;
    Block *end;
} Block_List;

void block_list_push(Arena *arena, Block_List *list, Statement statement);

void dump_block(FILE *stream, Block *block, int level);
void dump_statement(FILE *stream, Statement statement, int level);
int dump_statement_as_dot_edges(FILE *stream, Statement statement, int *counter);
void dump_statement_as_dot(FILE *stream, Statement statement);

void parse_directive_from_line(Arena *arena, Linizer *linizer, Block_List *output);
Block *parse_block_from_lines(Arena *arena, Linizer *linizer);

#endif // STATEMENT_H_
