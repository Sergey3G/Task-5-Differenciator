#include <stdio.h>
#include <stdlib.h>

#include "../include/taylor.h"
#include "../include/simplifier.h"
#include "../include/differenciator.h"
#include "../include/my_assert.h"

#define OP(name, L, R) make_op((name), (L), (R))
#define CONST(x) make_const((x))
#define VAR(name) make_var((name))

#define ADD_(a, b) OP("+", (a), (b))
#define SUB_(a, b) OP("-", (a), (b))
#define MUL_(a, b) OP("*", (a), (b))
#define DIV_(a, b) OP("/", (a), (b))
#define POW_(a, b) OP("^", (a), (b))

TreeNode* make_peano(char var, double a, int power)
{
    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
    MY_ASSERT(node, "Error: memory allocation for node failed!\n");
    node->value = (NodeValue*)calloc(1, sizeof(NodeValue));
    MY_ASSERT(node->value, "Error: memory allocation for node->value failed!\n");
    node->value->peano = (Peano*)calloc(1, sizeof(Peano));
    MY_ASSERT(node->value->peano, "Error: memory allocation for node->value->peano failed!\n");

    node->value->type = PEANO;
    node->value->peano->var = var;
    node->value->peano->a = a;
    node->value->peano->power = power;
    node->id = -1;

    return node;
}

static double factorial(int n)
{
    double res = 1.0;
    for (int i = 2; i <= n; i++)
        res *= i;
    return res;
}

static TreeNode* make_const(double value)
{
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "%.17g", value);
    TreeNode* node = construct_node(CONST, buf);
    node->id = -1;
    return node;
}

static TreeNode* make_var(char name)
{
    char buf[2] = {name, '\0'};
    TreeNode* node = construct_node(VAR, buf);
    node->id = -1;
    return node;
}

static TreeNode* make_op(const char* op, TreeNode* L, TreeNode* R)
{
    TreeNode* node = construct_node(OP, op);
    node->id = -1;
    node->left = L;
    node->right = R;

    if (L)
        L->parent = node;
    if (R)
        R->parent = node;

    return node;
}

TreeNode* taylor_peano(TreeNode* f, char var, double a, int order)
{
    TreeNode* result = NULL;
    TreeNode* deriv = copy_tree(f);

    printf("var = %c, a = %.5g, order = %d\n", var, a, order);

    for (int k = 0; k <= order; k++)
    {
        double value = eval_tree(deriv, var, a);
        printf("value = %.5g\n", value);
        double coeff = value / factorial(k);
        printf("coeff = %.5g\n", coeff);

        TreeNode* power = POW_(SUB_(VAR(var), CONST(a)), CONST(k));
        TreeNode* term  = MUL_(CONST(coeff), power);

        if (result)
            result = ADD_(result, term);
        else
            result = term;

        TreeNode* next = differenciate_tree(deriv, var);
        TreeNode* next_simple = simplify_tree(next);

        free_subtree(next);
        free_subtree(deriv);

        deriv = next_simple;
    }

    free_subtree(deriv);

    TreeNode* peano = make_peano(var, a, order);
    MY_ASSERT(peano, "Fail with peano!\n");
    MY_ASSERT(peano->value, "Fail with peano->value!\n");
    MY_ASSERT(peano->value->peano, "Fail with peano->value->peano\n");
    result = ADD_(result, peano);
    MY_ASSERT(result, "Error: result is nullptr!\n");

    TreeNode* simplified = simplify_tree(result);
    MY_ASSERT(simplified->right->value->peano, "Error: peano is nullptr after simplifying!\n");
    free_subtree(result);

    return simplified;
}

