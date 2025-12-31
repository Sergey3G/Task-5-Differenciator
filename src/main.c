#include <stdio.h>
#include <stdlib.h>

#include "../include/tex.h"
#include "../include/trees.h"
#include "../include/graph.h"
// #include "calculator.h"
#include "../include/simplifier.h"
#include "../include/differenciator.h"
#include "../include/taylor.h"
#include "../include/my_assert.h"
#include "../include/python_graph.h"

int main(const int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("You didn't enter enough filenames!\n");
        return 1;
    }

    char* expression_filename = argv[1];
    String* buffer1 = file_to_buffer(expression_filename);

    char* graphviz_filename = argv[2];
    char* tex_filename = argv[3];
    char* python_filename = argv[4];
    char* p = buffer1->ptr;
    char* end = buffer1->ptr + buffer1->len;

    TreeNode* original = parse_tree(&p, end);
    MY_ASSERT(original, "Error: cannot read expression correctly!\n");
    TreeNode* work = copy_tree(original);

    TreeNode* simplified = simplify_tree(work);
    free_subtree(work);
    work = simplified;

    TreeNode* diff = differenciate_tree(work, 'x');
    free_subtree(work);
    work = diff;

    TreeNode* diff_simple = simplify_tree(work);
    free_subtree(work);
    work = diff_simple;

    TreeNode* taylor = taylor_peano(original, 'x', 0, 3);
    TreeNode* tangent = find_tangent(original, 'x', 0);

    tree_dump(taylor, graphviz_filename);
    tex_dump(tex_filename, original, work, taylor);
    print_into_python(python_filename, original, tangent, taylor);

    free_subtree(original);
    free_subtree(work);
    free_subtree(taylor);
    free_subtree(tangent);
    free(buffer1->ptr);
    free(buffer1);
}
