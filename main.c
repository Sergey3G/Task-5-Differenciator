#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tex.h"
#include "trees.h"
#include "graph.h"
#include "calculator.h"
#include "simplifier.h"
#include "differenciator.h"
#include "taylor.h"
#include "my_assert.h"

int main(const int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("You didn't enter enough filenames!\n");
        return 1;
    }

    char* filename1 = argv[1];
    String* buffer1 = file_to_buffer(filename1);

    char* filename2 = argv[2];
    char* filename3 = argv[3];
    char* p = buffer1->ptr;
    char* end = buffer1->ptr + buffer1->len;

    TreeNode* original = parse_tree(&p, end);
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
    printf("%d", taylor->right->value->type);
    MY_ASSERT(taylor, "Error: taylor in main is nullptr!\n");
    MY_ASSERT(taylor->right, "Error: taylor->right (peano) in main is nullptr!\n");
    MY_ASSERT(taylor->right->value->peano, "Error: taylor->right->value->peano in main is nullptr!\n");

    tree_dump(taylor, filename2);
    tex_dump(filename3, original, work, taylor);

    free_subtree(original);
    free_subtree(work);
    free_subtree(taylor);
    free(buffer1->ptr);
    free(buffer1);
}
