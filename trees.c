#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

#include "trees.h"

static const Operator OPERATORS[] = {{"+", ADD},
                                     {"-", SUB},
                                     {"*", MUL},
                                     {"/", DIV},
                                     {"^", POW},
                                     {"sin", SIN},
                                     {"cos", COS},
                                     {"ln", LN},
                                     {"sqrt", SQRT},
                                     {"arcsin", ARCSIN},
                                     {"arccos", ARCCOS},
                                     {"tg", TG},
                                     {"ctg", CTG},
                                     {"arctg", ARCTG},
                                     {"arcctg", ARCCTG},
                                     {"sh", SH},
                                     {"ch", CH},
                                     {"th", TH},
                                     {"cth", CTH},
                                     {"exp", EXP},
                                     {NULL, NULL_OP}};

static int GLOBAL_NODE_ID = 0;

TreeNode* construct_node(ValueType type, const char* value_str)
{
    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
    if (!node)
    {
        fprintf(stderr, "Error: memory allocation for node failed!\n");
        return NULL;
    }

    node->value = (NodeValue*)calloc(1, sizeof(NodeValue));
    if (!node->value)
    {
        fprintf(stderr, "Error: memory allocation for node value failed!\n");
        return NULL;
    }

    node->parent = NULL;
    node->left = node->right = NULL;

    node->value->type = type;
    char* endptr = NULL;

    switch (type)
    {
        case VAR:
            node->value->var_name = value_str[0];
            break;

        case CONST:
            node->value->constant = strtod(value_str, &endptr);

            if (endptr == value_str)
            {
                fprintf(stderr, "Error: %s is not a number!\n", value_str);
                free(node->value);
                free(node);
                return NULL;
            }
            break;
        case OP:
            node->value->operation = (Operator*)calloc(1, sizeof(Operator));
            if (!node->value->operation)
            {
                fprintf(stderr, "Error: memory allocation for operation failed!\n");
                free(node->value);
                free(node);
                return NULL;
            }
            node->value->operation->name = strdup(value_str);
            node->value->operation->code = get_operator_code(value_str);
            break;
        default:
            break;
    }

    return node;
}

static void skip_spaces(char** p)
{
    while (**p == '\t' || **p == ' ' || **p == '\n')
        (*p)++;
}

int is_operator(const char* string)
{
    for (int i = 0; OPERATORS[i].name; i++)
    {
        if (!strcmp(string, OPERATORS[i].name))
            return 1;
    }
    return 0;
}

OperatorCode get_operator_code(const char* string)
{
    for (int i = 0; OPERATORS[i].name; i++)
    {
        if (!strcmp(string, OPERATORS[i].name))
            return OPERATORS[i].code;
    }
    return NULL_OP;
}

ValueType detect_type(const char* string)
{
    if (is_operator(string))
        return OP;

    char* end = NULL;
    strtod(string, &end);
    if (*end == '\0')
        return CONST;

    return VAR;
}

TreeNode* parse_tree(char** p, char* end)
{
    skip_spaces(p);

    if (*p >= end)
        return NULL;

    if (!strncmp(*p, "nil", 3))
    {
        *p += 3;
        return NULL;
    }

    if (**p != '(')
    {
        fprintf(stderr, "Parse error: expected '('\n");
        return NULL;
    }

    (*p)++;
    skip_spaces(p);

    char val[64] = {0};
    int i = 0;

    while (**p && !isspace(**p) && **p != '(' && **p != ')' && i < 63)
    {
        val[i++] = **p;
        (*p)++;
    }
    val[i] = '\0';

    ValueType type = detect_type(val);
    TreeNode* node = construct_node(type, val);
    node->id = GLOBAL_NODE_ID++;
    skip_spaces(p);

    node->left = parse_tree(p, end);
    if (node->left)
        node->left->parent = node;

    skip_spaces(p);

    node->right = parse_tree(p, end);
    if (node->right)
        node->right->parent = node;

    skip_spaces(p);

    if (*p < end && **p == ')')
        (*p)++;

    return node;
}

void free_subtree(TreeNode* node)
{
    if (!node)
        return;

    free_subtree(node->left);
    free_subtree(node->right);

    node->parent = NULL;
    if (node->value)
    {
        if (node->value->type == OP)
        {
            if (node->value->operation)
            {
                free(node->value->operation->name);
                free(node->value->operation);
            }
        }
        free(node->value);
    }
    free(node);
}

void free_node(TreeNode* node)
{
    if (!node)
        return;

    if (node->value->type == OP)
    {
        free(node->value->operation->name);
        free(node->value->operation);
    }
    free(node->value);
    free(node);
}

String* file_to_buffer(const char* filename)
{
    struct stat st;
    if (stat(filename, &st) != 0)
    {
        perror("stat failed\n");
        return NULL;
    }

    size_t file_size = (size_t)st.st_size;
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: cannot open file %s!\n", filename);
        return NULL;
    }

    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    if (!buffer)
    {
        printf("Error: memory allocarion failed!\n");
        return NULL;
    }

    size_t read_size = fread(buffer, sizeof(char), file_size, file);
    fclose(file);
    if (read_size != file_size)
    {
        printf("Error: cannot read file content!\n");
        free(buffer);
        return NULL;
    }

    buffer[file_size] = '\0';

    String* string = (String*)calloc(1, sizeof(String));
    string->ptr = buffer;
    string->len = read_size + 1;
    return string;
}
