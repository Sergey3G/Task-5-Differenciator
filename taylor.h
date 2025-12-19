#ifndef TAYLOR_H
#define TAYLOR_H

#include "trees.h"

TreeNode* make_peano(char var, double a, int power);
TreeNode* taylor_peano(TreeNode* f, char var, double a, int order);

#endif
