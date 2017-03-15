#include <bebop/util/config.h>
#include <bebop/smc/sparse_matrix.h>
#include <bebop/smc/sparse_matrix_ops.h>
#include <bebop/smc/coo_matrix.h>
#include <bebop/smc/coord_elem.h>
#include <bebop/smc/csc_matrix.h>
#include <bebop/smc/csr_matrix.h>
#include <bebop/smc/mmio.h>
#include <bebop/smc/read_mm.h>

#include <bebop/util/get_options.h>
#include <bebop/util/init.h>
#include <bebop/util/log.h>
#include <bebop/util/malloc.h>
#include <bebop/util/timer.h>
#include <bebop/util/util.h>
#include <bebop/util/complex.h>
#include <bebop/util/merge_sort.h>
#include <bebop/util/log.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    const char *filename;
    int code=1;

    if (argc < 2)
        {
                fprintf(stderr, "Usage: %s [matrix-market-input-filename]\n", argv[0]);
                exit(1);
        }
    filename = argv[1];
    printf("filename: %s \n",filename);
    struct coo_matrix_t* A = bebop_malloc (sizeof (struct coo_matrix_t));
    printf("read code: %d \n",code);
    code = read_matrix_market_real_sparse ("MatrixMarket_MHM_subproblem.mtx", A);
    //code = read_matrix_market_real_sparse (filename, A);
    printf("read code: %d \n",code);
    if ( code != 0 )
       {
             fprintf(stderr, "Could not open Matrix Market file");
              exit(1);
       }

    return 0;
}

