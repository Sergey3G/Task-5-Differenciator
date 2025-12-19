#include <stdio.h>
#include <math.h>

#include "simplifier.h"
#include "differenciator.h"

static TreeNode* make_const(double value)
{
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "%.17g", value);
    TreeNode* node = construct_node(CONST, buf);
    node->id = -1;
    return node;
}

static TreeNode* make_op(const char* name, TreeNode* left, TreeNode* right)
{
    TreeNode* node = construct_node(OP, name);
    node->id = -1;
    node->left = left;
    node->right = right;
    if (left)
        left->parent = node;
    if (right)
        right->parent = node;
    return node;
}

static TreeNode* make_exp(TreeNode* argument)
{
    TreeNode* node = construct_node(OP, "exp");
    node->id = -1;
    node->left = argument;
    argument->parent = node;
    return node;
}

static inline int is_equal(const TreeNode* n, double val)
{
    if (!n || n->value->type != CONST)
        return 0;

    return fabs(n->value->constant - val) < 1e-12;
}

static TreeNode* fold_constants(TreeNode* node)
{
    if (!node || node->value->type != OP)
        return node;

    TreeNode* L = node->left;
    TreeNode* R = node->right;

    if (!L || !R)
        return node;

    if (L->value->type != CONST || R->value->type != CONST)
        return node;

    double a = L->value->constant;
    double b = R->value->constant;
    double result = 0.0;

    switch (node->value->operation->code)
    {
        case ADD:
            result = a + b;
            break;
        case SUB:
            result = a - b;
            break;
        case MUL:
            result = a * b;
            break;
        case DIV:
            result = a / b;
            break;
        case POW:
            result = pow(a, b);
            break;
        default: return node;
    }
    return make_const(result);
}

static TreeNode* simplify_add(TreeNode* node)
{
    if (is_equal(node->left, 0)) return copy_tree(node->right);
    if (is_equal(node->right, 0)) return copy_tree(node->left);
    if ()
    return node;
}

static TreeNode* simplify_sub(TreeNode* node)
{
    if (is_equal(node->right, 0)) return copy_tree(node->left);
    return node;
}

static TreeNode* simplify_mul(TreeNode* node)
{
    node->left->value->type == CONST ? node->left->value->constant : -999;
    node->right->value->type == CONST ? node->right->value->constant : -999;

    if (is_equal(node->left, 0) || is_equal(node->right, 0))
        return make_const(0);
    if (is_equal(node->right, 1)) return copy_tree(node->left);
    if (is_equal(node->left, 1)) return copy_tree(node->right);
    return node;
}

static TreeNode* simplify_div(TreeNode* node)
{
    if (is_equal(node->left, 0)) return make_const(0);
    if (is_equal(node->right, 1)) return copy_tree(node->left);
    return node;
}

static TreeNode* simplify_pow(TreeNode* node)
{
    if (is_equal(node->right, 1)) return copy_tree(node->left);
    if (is_equal(node->left, 1)) return make_const(1);
    if (is_equal(node->right, 0)) return make_const(1);

    if ((node->left->value->type == CONST && is_equal(node->left, M_E))
                                          || node->left->value->type == VAR && node->left->value->var_name == 'e')
    {
        return make_op("exp", copy_tree(node->right), NULL);
    }
    return node;
}


static TreeNode* simplify_ln(TreeNode* node)
{
    TreeNode* L = node->left;

    if (is_equal(L, 1)) return make_const(0);
    if (L && L->value->type == OP && L->value->operation->code == EXP)
        return copy_tree(L->left);

    return node;
}


static TreeNode* simplify_exp(TreeNode* node)
{
    TreeNode* L = node->left;

    if (is_equal(L, 0))
        return make_const(1);

    if (L && L->value->type == OP && L->value->operation->code == LN)
        return copy_tree(L->left);

    return node;
}

TreeNode* simplify_tree(TreeNode* node)
{
    if (!node)
        return NULL;

    if (node->value->type == PEANO)
        return copy_tree(node);

    if (node->value->type != OP)
        return copy_tree(node);

    TreeNode* L = simplify_tree(node->left);
    TreeNode* R = simplify_tree(node->right);

    TreeNode* rebuilt = make_op(node->value->operation->name, L, R);

    TreeNode* folded = fold_constants(rebuilt);
    if (folded != rebuilt)
    {
        free_subtree(rebuilt);
        return folded;
    }

    TreeNode* simplified = NULL;

    if (rebuilt->value->operation->code == MUL)
    {
        if (rebuilt->left->value->type == OP &&
            rebuilt->left->value->operation->code == EXP &&
            is_equal(rebuilt->left->left, 0))
        {
            TreeNode* res = copy_tree(rebuilt->right);
            free_subtree(rebuilt);
            return res;
        }

        if (rebuilt->right->value->type == OP &&
            rebuilt->right->value->operation->code == EXP &&
            is_equal(rebuilt->right->left, 0))
        {
            TreeNode* res = copy_tree(rebuilt->left);
            free_subtree(rebuilt);
            return res;
        }
    }

    switch (rebuilt->value->operation->code)
    {
        case ADD: simplified = simplify_add(rebuilt); break;
        case SUB: simplified = simplify_sub(rebuilt); break;
        case MUL: simplified = simplify_mul(rebuilt); break;
        case DIV: simplified = simplify_div(rebuilt); break;
        case POW: simplified = simplify_pow(rebuilt); break;
        case LN:  simplified = simplify_ln(rebuilt);  break;
        case EXP: simplified = simplify_exp(rebuilt); break;
        default:  return rebuilt;
    }

    if (simplified != rebuilt)
        free_subtree(rebuilt);

    return simplified;
}

