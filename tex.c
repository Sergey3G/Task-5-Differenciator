#include <stdio.h>

#include "tex.h"

void tex_dump(const char* filename, TreeNode* root)
{
    FILE* file = fopen(filename, "w");
    if (!file)
    {
        fprintf(stderr, "Error: cannot open file %s!\n", filename);
        return;
    }

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
                   "\\% \\usepackage{hyperref}\n"
                   "\\captionsetup[figure]{name=Рисунок}\n"
                   "\\captionsetup[table]{name=Таблица}\n\n"
                   "\\title\n\n"
                   "\\author{Сергей Коваленко}\n\n");
}
