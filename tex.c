#include <stdio.h>
#include <math.h>

#include "tex.h"
#include "simplifier.h"
#include "differenciator.h"
#include "my_assert.h"

static void tex_print_derivative(FILE* file, TreeNode* node)
{
    MY_ASSERT(file, "Error: given nullptr to file!\n");
    MY_ASSERT(node, "Error: given nullptr to node!\n");
    fprintf(file, "\\frac{d}{dx}\\left(");
    tex_print_expr(file, node);
    fprintf(file, "\\right)");
}

int op_priority(OperatorCode op)
{
    switch(op)
    {
        case ADD:
        case SUB: return 1;
        case MUL:
        case DIV: return 2;
        case POW: return 3;
        case SIN:
        case COS:
        case LN:
        case TG:
        case CTG:
        case SH:
        case CH:
        case EXP:
        case SQRT:
        case ARCSIN:
        case ARCCOS:
        case ARCTG:
        case ARCCTG:
            return 4;
        default:
            return 0;
    }
}

void print_expr(TreeNode* node)
{
    MY_ASSERT(node, "Error: given nullptr to node!\n");

    NodeValue* value = node->value;

    if (value->type == CONST)
    {
        printf("%.10g", value->constant);
        return;
    }

    if (value->type == VAR)
    {
        printf("%c", value->var_name);
        return;
    }

    if (value->type == PEANO)
    {
        MY_ASSERT(value->peano, "Error: in peano node value is nullptr!\n");
        if (fabs(value->peano->a) < 1e-12)
            printf("o(%c^%d)", value->peano->var, value->peano->power);
        else
            printf("o((%c - %.5g)^%d)", value->peano->var, value->peano->a, value->peano->power);
        return;
    }

    OperatorCode code = value->operation->code;

    if (!node->right)
    {
        printf("%s(", value->operation->name);
        print_expr(node->left);
        printf(")");
        return;
    }

    int need_l = node->left->value->type == OP &&
                        op_priority(node->left->value->operation->code) < op_priority(code);

    int need_r = node->right->value->type == OP &&
                        op_priority(node->right->value->operation->code) <= op_priority(code);

    if (need_l) printf("(");
    print_expr(node->left);
    if (need_l) printf(")");

    printf(" %s ", value->operation->name);

    if (need_r) printf("(");
    print_expr(node->right);
    if (need_r) printf(")");
}

void tex_dump(const char* filename, TreeNode* original, TreeNode* diff, TreeNode* taylor)
{
    FILE* file = fopen(filename, "w");
    MY_ASSERT(file, "Error: cannot open file by filename!\n");
    MY_ASSERT(original, "Error: given nullptr to original!\n");
    MY_ASSERT(diff, "Error: given nullptr to diff!\n");
    MY_ASSERT(taylor, "Error: given nullptr to taylor!\n");

    print_expr(original);
    printf("\n");
    print_expr(diff);
    printf("\n");
    print_expr(taylor);
    printf("\n");
    fprintf(file, "\\documentclass[a4paper,12pt]{article}\n"
                  "\\usepackage[utf8]{inputenc}\n"
                  "\\usepackage[russian,english]{babel}\n"
                  "\\usepackage[T2A]{fontenc}\n"
                  "\\usepackage[left=20mm, top=30mm, right=20mm, bottom=30mm, footskip=13mm]{geometry}\n"
                  "\\usepackage{indentfirst}\n"
                  "\\usepackage{amsmath,amssymb}\n"
                  "\\usepackage{graphicx}\n"
                  "\\usepackage[italicdiff]{physics}\n"
                  "\\usepackage{caption}\n"
                  "\\usepackage{array}\n"
                  "\\usepackage{float}\n"
                  "\\captionsetup[figure]{name=Рисунок}\n"
                  "\\captionsetup[table]{name=Таблица}\n\n"
                  "\\title{Дифференциатор}\n\n"
                  "\\author{Сергей Коваленко, Б01-504}\n\n"
                  "\\begin{document}\n\n"
                  "\\maketitle\n\n"
                  "\\newpage\n\n");
    fprintf(file, "\\section{Результат дифференцирования}\n\n");
    fprintf(file, "\\[\n");
    tex_print_expr(file, diff);
    fprintf(file, "\n\\]\n\n");
    printf("Ended first derivative\n");
    fprintf(file, "\\section{Пошаговое дифференцирование}\n\n");
    int step = 1;
    diff_each_node(original, 'x', file, &step);
    printf("Ended stepped differenciation\n");
    fprintf(file, "\\section{Разложение по Тейлору с членом в форме Пеано}\n");
    fprintf(file, "\\[\n");
    tex_print_expr(file, taylor);
    printf("Ended Taylor\n");
    fprintf(file, "\n\\]\n\n");
    fprintf(file, "\\end{document}");
}

void tex_print_expr(FILE* file, TreeNode* node)
{
    MY_ASSERT(file, "Error: given nullptr to file!\n");
    MY_ASSERT(file, "Error: given nullptr to node!\n");

    NodeValue* v = node->value;

    if (v->type == CONST)
    {
        fprintf(file, "%.5g", v->constant);
        return;
    }

    if (v->type == VAR)
    {
        fprintf(file, "%c", v->var_name);
        return;
    }

    if (v->type == PEANO)
    {
        if (fabs(v->peano->a) < 1e-12)\
            fprintf(file, "o\\left(%c^{%d}\\right)", v->peano->var, v->peano->power);
        else
            fprintf(file, "o\\left((%c - %.5g)^{%d}\\right)", v->peano->var, v->peano->a, v->peano->power);
        return;
    }

    OperatorCode op = v->operation->code;

    switch (op)
    {
        case ADD:
        case SUB:
            fprintf(file, "{");
            tex_print_expr(file, node->left);
            fprintf(file, " %s ", v->operation->name);
            tex_print_expr(file, node->right);
            fprintf(file, "}");
            break;
        case MUL:
            if (node->left->value->type == CONST &&
                fabs(node->left->value->constant + 1) < 1e-12)
            {
                fprintf(file, "-");
                tex_print_expr(file, node->right);
                break;
            }

            if (node->right->value->type == CONST &&
                fabs(node->right->value->constant + 1) < 1e-12)
            {
                fprintf(file, "-");
                tex_print_expr(file, node->left);
                break;
            }

            if (node->left->value->type == CONST &&
                fabs(node->left->value->constant - 1) < 1e-12)
            {
                tex_print_expr(file, node->right);
                break;
            }

            if (node->right->value->type == CONST &&
                fabs(node->right->value->constant - 1) < 1e-12)
            {
                tex_print_expr(file, node->left);
                break;
            }

            fprintf(file, "{");
            tex_print_expr(file, node->left);
            fprintf(file, "}\\cdot{");
            tex_print_expr(file, node->right);
            fprintf(file, "}");
            break;
        case DIV:
            fprintf(file, "\\frac{");
            tex_print_expr(file, node->left);
            fprintf(file, "}{");
            tex_print_expr(file, node->right);
            fprintf(file, "}");
            break;

        case POW:
            if (node->right->value->type == CONST &&
                fabs(node->right->value->constant - 1) < 1e-12)
            {
                tex_print_expr(file, node->left);
                break;
            }

            fprintf(file, "\\left(");
            tex_print_expr(file, node->left);
            fprintf(file, "\\right)^{");
            tex_print_expr(file, node->right);
            fprintf(file, "}");
            break;

        case SIN:
        case COS:
        case LN:
        case TG:
        case CTG:
        case SH:
        case CH:
        case TH:
        case CTH:
        case EXP:
        case ARCSIN:
        case ARCCOS:
        case ARCTG:
        case ARCCTG:
            fprintf(file, "\\%s\\left(", v->operation->name);
            tex_print_expr(file, node->left);
            fprintf(file, "\\right)");
            break;

        case SQRT:
            fprintf(file, "\\sqrt{");
            tex_print_expr(file, node->left);
            fprintf(file, "}");
            break;

        default:
            fprintf(stderr, "Unknown operator in LaTeX output\n");
    }
}

void diff_each_node(TreeNode* node, char var, FILE* tex, int* step)
{
    if (!node)
        return;

    if (node->left && node->left->id >= 0)
        diff_each_node(node->left, var, tex, step);

    if (node->right && node->right->id >= 0)
        diff_each_node(node->right, var, tex, step);

    if (node->id < 0 || node->value->type != OP)
        return;

    fprintf(tex,
        "\\[\n"
        "\\begin{aligned}\n"
        "%d.\\quad &",
        (*step)++
    );

    tex_print_derivative(tex, node);

    fprintf(tex, " \\\\\n&= ");

    TreeNode* diff = differenciate_tree(node, var);
    TreeNode* simple = simplify_tree(diff);

    free_subtree(diff);

    tex_print_expr(tex, simple);

    fprintf(tex,
        "\n\\end{aligned}\n"
        "\\]\n\n"
    );

    free_subtree(simple);
}


