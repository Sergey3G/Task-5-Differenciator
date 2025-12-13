#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tex.h"
#include "trees.h"
#include "graph.h"
#include "calculator.h"
#include "simplifier.h"
#include "differenciator.h"

int main(const int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("You didn't enter enough filenames!\n");
        return 1;
    }

    char* filename1 = argv[1];
    String* buffer1 = file_to_buffer(filename1);
    if (!buffer1)
    {
        fprintf(stderr, "Error in file_to_buffer!\n");
        return 1;
    }
    char* filename2 = argv[2];
    char* p = buffer1->ptr;
    char* end = buffer1->ptr + buffer1->len;

    TreeNode* tree = parse_tree(&p, end);

    TreeNode* simplified = simplify_tree(tree);
    free_subtree(tree);
    tree = simplified;

    TreeNode* diff = differenciate_tree(tree);
    free_subtree(tree);
    tree = diff;

    TreeNode* diff_simple = simplify_tree(tree);
    free_subtree(tree);
    tree = diff_simple;

    tree_dump(tree, filename2);
    free(buffer1->ptr);
    free(buffer1);
    free_subtree(tree);
}
