#ifndef TREES_H
#define TREES_H

typedef struct String
{
    char* ptr;
    size_t len;
} String;

typedef enum ValueType
{
    VAR = 1,
    CONST = 2,
    OP = 3,
    PEANO = 4
} ValueType;

typedef enum OperatorCode
{
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    POW = 5,
    SIN = 6,
    COS = 7,
    LN = 8,
    SQRT = 9,
    ARCSIN = 10,
    ARCCOS = 11,
    TG = 12,
    CTG = 13,
    ARCTG = 14,
    ARCCTG = 15,
    SH = 16,
    CH = 17,
    TH = 18,
    CTH = 19,
    EXP = 20,
    NULL_OP = 0
} OperatorCode;

typedef struct Operator
{
    char* name;
    OperatorCode code;
} Operator;

typedef struct Peano
{
    char var;
    double a;
    int power;
} Peano;

typedef struct NodeValue
{
    ValueType type;
    union
    {
        double constant;
        char var_name;
        Operator* operation;
        Peano* peano;
    };
} NodeValue;

typedef struct _node
{
    _node* parent;
    NodeValue* value;
    _node* left;
    _node* right;
    int id;
} TreeNode;

TreeNode* construct_node(ValueType type, const char* value_str);
int is_operator(const char* string);
OperatorCode get_operator_code(const char* string);
ValueType detect_type(const char* string);
TreeNode* parse_tree(char** p, char* end);
void free_subtree(TreeNode* node);
void free_node(TreeNode* node);
String* file_to_buffer(const char* filename);
double eval_tree(TreeNode* node, char var, double value);

#endif
