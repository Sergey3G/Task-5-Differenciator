#include <stdio.h>
#include "../include/trees.h"

static void format_node_value(const NodeValue* val, char* val_buf, size_t val_size, char* type_buf, size_t type_size)
{
    if (!val || !val_buf || !type_buf) return;

    switch (val->type)
    {
        case CONST:
            snprintf(val_buf, val_size, "%g", val->constant);
            snprintf(type_buf, type_size, "CONST");
            break;

        case VAR:
            snprintf(val_buf, val_size, "%c", val->var_name);
            snprintf(type_buf, type_size, "VARIABLE");
            break;

        case OP:
            snprintf(val_buf, val_size, "%s", val->operation->name);
            snprintf(type_buf, type_size, "OPERATION");
            break;

        case PEANO:
            snprintf(val_buf, val_size, "var = %c; a = %.5g; power = %d", val->peano->var, val->peano->a, val->peano->power);
            snprintf(type_buf, type_size, "PEANO");
            break;

        default:
            snprintf(val_buf, val_size, "?");
            break;
    }
}

void node_dump(const TreeNode* node, FILE* f, size_t* node_num)
{
    if (!f || !node || !node_num)
        return;

    size_t current_num = (*node_num)++;

    char value_buf[64] = {0};
    char type_buf[32] = {0};
    format_node_value(node->value, value_buf, sizeof(value_buf), type_buf, sizeof(type_buf));

    if (!node->left && !node->right)
    {
        fprintf(f, "  node%zu [label=\"{ %s | %s | {NIL | NIL}}\"];\n",
                current_num, value_buf, type_buf);
    }
    else if (!node->right)
    {
        fprintf(f, "  node%zu [label=\"{ %s | %s | {<left>%p | NIL}}\"];\n",
                current_num, value_buf, type_buf, node->left);
    }
    else
    {
        fprintf(f, "  node%zu [label=\"{ %s | %s | {<left> %p | <right> %p}}\"];\n",
                current_num, value_buf, type_buf, node->left, node->right);
    }
    if (node->left)
    {
        size_t left_num = *node_num;
        node_dump(node->left, f, node_num);
        fprintf(f, "  node%zu:left -> node%zu;\n", current_num, left_num);
    }

    if (node->right)
    {
        size_t right_num = *node_num;
        node_dump(node->right, f, node_num);
        fprintf(f, "  node%zu:right -> node%zu;\n", current_num, right_num);
    }
}

void tree_dump(const TreeNode* root, const char* filename)
{
    if (!root || !filename)
        return;

    FILE* f = fopen(filename, "w");
    if (!f)
    {
        printf("Error: cannot open file '%s'\n", filename);
        return;
    }

    size_t node_num = 0;

    fprintf(f, "digraph G {\n");
    fprintf(f, "  bgcolor=\"#ffffffff\";\n");
    fprintf(f, "  node [shape=record, fontname=\"Moscow Sans\", fontsize=10, "
                "style=filled, fillcolor=\"#ded1d1ff\", fontcolor=\"#c20c0cff\", color=gray];\n");
    fprintf(f, "  edge [arrowsize=0.7, color=gray];\n\n");

    node_dump(root, f, &node_num);

    fprintf(f, "}\n");
    fclose(f);
}

