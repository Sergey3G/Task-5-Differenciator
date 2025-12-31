#include "stdio.h"

#include "../include/python_graph.h"
#include "../include/my_assert.h"
#include "../include/tex.h"

void print_into_python(const char* filename, TreeNode* original, TreeNode* tangent, TreeNode* taylor)
{
    MY_ASSERT(filename, "Error: filename is nullptr!\n");
    MY_ASSERT(original, "Error: original function is nullptr!\n");
    MY_ASSERT(taylor, "Error: taylor is nullptr!\n");
    MY_ASSERT(tangent, "Error: diff is nullptr!\n");

    MY_ASSERT(taylor->right || taylor->right->value->type == PEANO, "Error: Taylor has no Peano term!\n");

    FILE* file = fopen(filename, "w");
    MY_ASSERT(file, "Error: cannot open file!\n");

    double a = taylor->right->value->peano->a;
    char var = taylor->right->value->peano->var;

    fprintf(file, "import sys\nimport numpy as np\n"
                  "import matplotlib.pyplot as plt\n\n");
    fprintf(file, "%c = np.linspace(%lf, %lf, %d)\n", var, a - 3, a + 3, 2000);
    fprintf(file, "f = lambda %c: ", var);
    python_print_expression(file, original);
    fprintf(file, "\n");

    fprintf(file, "teyl = lambda %c: ", var);
    python_print_expression(file, taylor->left);
    fprintf(file, "\n");

    fprintf(file, "tan = lambda x: ");
    python_print_expression(file, tangent);
    fprintf(file, "\n\n");

    fprintf(file, "with np.errstate(divide='ignore', invalid='ignore'):\n"
                  "    f_f = f(%c)\n"
                  "    f_T = teyl(%c)\n"
                  "    f_tan = tan(%c)\n\n"
                  "plt.plot(x, f_f, label='f(%c)')\n"
                  "plt.plot(x, f_T, label='Taylor')\n"
                  "plt.plot(x, f_tan, label='Tangent')\n"
                  "plt.axvline(%lf, color='gray', alpha=0.5)\n"
                  "plt.legend()\n"
                  "plt.grid(True)\n"
                  "plt.savefig(sys.argv[1])\n"
                  "plt.show()\n",
                  var, var, var, var, a);

}

void python_print_expression(FILE* file, TreeNode* node)
{
    MY_ASSERT(file, "Error: file is nullptr!\n");
    MY_ASSERT(node, "Error: node is nullptr!\n");

    NodeValue* value = node->value;

    if (value->type == CONST)
    {
        fprintf(file, "%.5g", value->constant);
        return;
    }

    if (value->type == VAR)
    {
        fprintf(file, "%c", value->var_name);
        return;
    }

    if (value->type == PEANO)
    {
        fprintf(file, "0");
        return;
    }

    OperatorCode code = value->operation->code;

    if (!node->right)
    {
        switch(code)
        {
            case SIN: fprintf(file, "np.sin("); break;
            case COS: fprintf(file, "np.cos("); break;
            case TG: fprintf(file, "np.tan("); break;
            case CTG: fprintf(file, "(1/np.tan("); break;
            case ARCSIN: fprintf(file, "np.arcsin("); break;
            case ARCCOS: fprintf(file, "np.arccos("); break;
            case ARCTG: fprintf(file, "np.arctan("); break;
            case ARCCTG: fprintf(file, "(np.pi/2 - np.arctan("); break;
            case SH: fprintf(file, "np.sinh("); break;
            case CH: fprintf(file ,"np.cosh("); break;
            case TH: fprintf(file, "np.tanh("); break;
            case CTH: fprintf(file, "(1/np.tanh("); break;
            case LN: fprintf(file, "np.log("); break;
            case EXP: fprintf(file, "np.exp("); break;
            case SQRT: fprintf(file, "np.sqrt("); break;
            default:
                fprintf(stderr, "unknown operator!\n");
        }

        python_print_expression(file, node->left);

        if (code == CTG || code == ARCCTG || code == CTH)
        {
            fprintf(file, "))");
        }
        else
        {
            fprintf(file, ")");
        }

        return;
    }

    int need_l = node->left->value->type == OP &&
                        op_priority(node->left->value->operation->code) < op_priority(code);

    int need_r = node->right->value->type == OP &&
                        op_priority(node->right->value->operation->code) <= op_priority(code);

    if (need_l) fprintf(file, "(");
    python_print_expression(file, node->left);
    if (need_l) fprintf(file, ")");

    switch(code)
    {
        case ADD: fprintf(file, " + "); break;
        case SUB: fprintf(file, " - "); break;
        case MUL: fprintf(file, " * "); break;
        case DIV: fprintf(file, " / "); break;
        case POW: fprintf(file, " ** "); break;
        default:
            fprintf(stderr, "unknown binary operator!\n");
    }

    if (need_r) fprintf(file, "(");
    python_print_expression(file, node->right);
    if (need_r) fprintf(file, ")");
}
