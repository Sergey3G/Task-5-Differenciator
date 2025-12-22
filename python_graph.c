#include "stdio.h"

#include "python_graph.h"
#include "my_assert.h"




void print_into_python(FILE* file, TreeNode* original, TreeNode* taylor)
{
    MY_ASSERT(file, "Error: file is nullptr!\n");
    MY_ASSERT(original, "Error: original function is nullptr!\n");
    MY_ASSERT(taylor, "Error: taylor is nullptr!\n");

    fprintf(file, "import numpy as np\n"
                  "import matplotlib.pyplot as plt\n\n");
    fprintf(file, "x = np.linspace(%lf, %lf, %d)\n", taylor->right->value->peano->a - 0.5, taylor->right->value->peano->a + 0.5, 200);
    

}
