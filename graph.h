#ifndef GRAPH_H
#define GRAPH_H

#include "trees.h"

void tree_dump(const TreeNode* root, const char* filename);
void node_dump(const TreeNode* node, FILE* f, size_t* node_num);

#endif
