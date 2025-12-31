#ifndef PYTHON_GRAPH_H
#define PYTHON_GRAPH_H

#include "trees.h"

void print_into_python(const char* filename, TreeNode* original, TreeNode* diff, TreeNode* taylor);
void python_print_expression(FILE* file, TreeNode* node);

#endif
