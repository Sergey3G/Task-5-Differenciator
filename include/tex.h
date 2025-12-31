#ifndef TEX_H
#define TEX_H

#include "trees.h"

int op_priority(OperatorCode op);
void print_expr(TreeNode* node);
void tex_dump(const char* filename, TreeNode* original, TreeNode* diff, TreeNode* taylor);
void tex_print_expr(FILE* file, TreeNode* node);
void diff_each_node(TreeNode* node, char var, FILE* tex, int* step);

#endif
