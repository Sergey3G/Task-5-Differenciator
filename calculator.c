#include <stdio.h>

#include "calculator.h"

int get_p(char** p)
{
    if (**p == '(')
    {
        (*p)++;
        int val = get_e(p);
        if (**p == ')')
        {
            (*p)++;
        }
        else
            printf("syntax error! %d\n", __LINE__);
        return val;
    }
    else
    {
        return get_n(p);
    }
}

int get_g(char** p)
{
    int val = get_e(p);
    if (**p != '\0')
        printf("syntax error! %d\n", __LINE__);
    return val;
}

int get_n(char** p)
{
    int val = 0;
    while ('0' <= (**p) && **p <= '9')
    {
        val = val * 10 + (**p - '0');
        (*p)++;
    }
    return val;
}

int get_e(char** p)
{
    int val = get_t(p);
    while (**p == '+' || **p == '-')
    {
        char op = **p;
        (*p)++;
        int val2 = get_t(p);
        if (op == '+')
            val += val2;
        else
            val -= val2;
    }
    return val;
}

int get_t(char** p)
{
    int val = get_p(p);
    while (**p == '*' || **p == '/')
    {
        char op = **p;
        (*p)++;
        int val2 = get_p(p);
        if (op == '*')
            val *= val2;
        else
            val /= val2;
    }
    return val;
}
