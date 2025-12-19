#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "differenciator.h"
#include "simplifier.h"
#include "taylor.h"
#include "my_assert.h"

#define OP(name, L, R) make_op((name), (L), (R))
#define CONST(x) make_const((x))
#define VAR(name) make_var((name))

#define U (node->left)
#define V (node->right)
#define DU differenciate_tree(U, var_name)
#define DV differenciate_tree(V, var_name)
#define CU copy_tree(U)
#define CV copy_tree(V)
#define CN(n) make_const((n))

#define ADD_(a, b) OP("+", (a), (b))
#define SUB_(a, b) OP("-", (a), (b))
#define MUL_(a, b) OP("*", (a), (b))
#define DIV_(a, b) OP("/", (a), (b))
#define POW_(a, b) OP("^", (a), (b))

#define SQRT_(a) OP("sqrt", (a), NULL)
#define SIN_(a) OP("sin", (a), NULL)
#define COS_(a) OP("cos", (a), NULL)
#define LN_(a) OP("ln",  (a), NULL)
#define SH_(a) OP("sh",  (a), NULL)
#define CH_(a) OP("ch",  (a), NULL)
#define EXP_(a) OP("exp", (a), NULL)

NodeValue* copy_value(const NodeValue* src)
{
    if (!src)
        return NULL;

    NodeValue* value = (NodeValue*)calloc(1, sizeof(NodeValue));
    MY_ASSERT(value, "Error: memory allocation failed!\n");

    value->type = src->type;

    switch (src->type)
    {
        case VAR:
            value->var_name = src->var_name;
            break;
        case CONST:
            value->constant = src->constant;
            break;
        case OP:
            value->operation = (Operator*)calloc(1, sizeof(Operator));
            MY_ASSERT(value->operation, "Error: memory allocation failed!\n");
            value->operation->name = strdup(src->operation->name);
            value->operation->code = src->operation->code;
            break;
        case PEANO:
            value->peano = (Peano*)calloc(1, sizeof(Peano));
            MY_ASSERT(value->peano, "Error: memory allocation failed!\n");
            value->peano->a = src->peano->a;
            value->peano->var = src->peano->var;
            value->peano->power = src->peano->power;
            break;
        default:
            break;
    }

    return value;
}

TreeNode* copy_tree(TreeNode* node)
{
    if (!node)
        return NULL;

    TreeNode* new_node = (TreeNode*)calloc(1, sizeof(TreeNode));
    MY_ASSERT(new_node, "Error: memory allocation for new_node failed!\n");

    new_node->value = copy_value(node->value);
    new_node->id = -1;

    new_node->left = copy_tree(node->left);
    if (new_node->left)
        new_node->left->parent = new_node;

    new_node->right = copy_tree(node->right);
    if (new_node->right)
        new_node->right->parent = new_node;

    new_node->parent = NULL;
    return new_node;
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

TreeNode* differenciate_tree(TreeNode* node, char var_name)
{
    if (!node)
        return NULL;

    if (var_name == 'e')
    {
        printf("You cannot differenciate by constant!\n");
        return node;
    }

    NodeValue* value = node->value;

    if (value->type == CONST || (value->type == VAR && value->var_name != var_name))
        return CN(0);

    if (value->type == VAR && value->var_name == var_name)
        return CN(1);

    if (value->type == PEANO)
    {
        printf("Called make_peano\n");
        printf("%c", value->peano->var);
        return make_peano(value->peano->var, value->peano->a, value->peano->power);
    }

    OperatorCode op = value->operation->code;

    switch (op)
    {
        case ADD: return ADD_(DU, DV);
        case SUB: return SUB_(DU, DV);
        case MUL: return ADD_(MUL_(DU, CV), MUL_(CU, DV));
        case DIV: return DIV_(SUB_(MUL_(DU, CV), MUL_(CU, DV)), POW_(CV, CN(2)));
        case POW:
            if (V->value->type == CONST)
            {
                double c = V->value->constant;
                return MUL_(CN(c), MUL_(POW_(CU, CN(c - 1)), DU));
            }
            else
            {
                return MUL_(POW_(CU, CV), ADD_(MUL_(DV, LN_(CU)), MUL_(CV, DIV_(DU, CU))));
            }
        case SIN: return MUL_(COS_(CU), DU);
        case COS: return MUL_(CN(-1), MUL_(SIN_(CU), DU));
        case LN: return DIV_(DU, CU);
        case TG: return DIV_(DU, POW_(COS_(CU), CN(2)));
        case CTG: return MUL_(CN(-1), DIV_(DU, POW_(SIN_(CU), CN(2))));
        case SH: return MUL_(CH_(CU), DU);
        case CH: return MUL_(SH_(CU), DU);
        case ARCSIN: return DIV_(DU, SQRT_(SUB_(CN(1), POW_(CU, CN(2)))));
        case ARCCOS: return MUL_(CN(-1), DIV_(DU, SQRT_(SUB_(CN(1), POW_(CU, CN(2))))));
        case ARCTG: return DIV_(DU, ADD_(CN(1), POW_(CU, CN(2))));
        case ARCCTG: return MUL_(CN(-1), DIV_(DU, ADD_(CN(1), POW_(CU, CN(2)))));
        case EXP: return MUL_(EXP_(CU), DU);
        case SQRT: return DIV_(DU, MUL_(CN(2), SQRT_(CU)));
        default:
            printf("Unsupported operator in differenciator!\n");
            return NULL;
    }
}
