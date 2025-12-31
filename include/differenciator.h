#ifndef DIFF_H
#define DIFF_H

#include "trees.h"

NodeValue* copy_value(const NodeValue* src);
TreeNode* copy_tree(TreeNode* node);
TreeNode* differenciate_tree(TreeNode* node, char var_name);
TreeNode* find_tangent(TreeNode* node, char var_name, double a);

#endif
