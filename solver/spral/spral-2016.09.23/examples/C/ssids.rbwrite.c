/* examples/C/ssids.c - Example code for SPRAL_SSIDS package */
#include "spral.h"

#include <bebop/smc/csc_matrix.h>

#include <bebop/util/config.h>
#include <bebop/smc/sparse_matrix.h>
#include <bebop/smc/sparse_matrix_ops.h>

#include <bebop/util/get_options.h>
#include <bebop/util/init.h>
#include <bebop/util/log.h>
#include <bebop/util/malloc.h>
#include <bebop/util/timer.h>
#include <bebop/util/util.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int
matrix_properties(const struct csc_matrix_t* A);

int
writerb(const struct csc_matrix_t* A);

int main(int argc, char *argv[])
{
   /*Bebop load and convert matrix*/
    enum sparse_matrix_file_format_t informat = 0;
    const char *filename;
    double seconds = 0.0;
    int errcode = 0;
    enum sparse_matrix_storage_format_t output_format;

    if (argc < 2)
        {
                fprintf(stderr, "Usage: %s [matrix-market-input-filename]\n", argv[0]);
                return 1;
        }
    filename = argv[1];
    printf("filename: %s \n",filename);
    struct sparse_matrix_t* A = NULL;

    printf ("Loading sparse matrix...");
    informat = MATRIX_MARKET;
    seconds = get_seconds();
    A = load_sparse_matrix (informat, filename);
    seconds = get_seconds() - seconds;
    if (A == NULL)
      {
        fprintf (stderr, "*** Failed to load input matrix file \"%s\" ***\n",
               filename);
        return 1;
      }
    printf ("done, in %g seconds\n", seconds);

    printf ("Validating sparse matrix...");
    seconds = get_seconds();
    errcode = valid_sparse_matrix (A);
    seconds = get_seconds() - seconds;
    printf ("done, in %g seconds\n", seconds);

    if (valid_sparse_matrix (A))
      printf ("\n### Sparse matrix is valid ###\n\n");
    else
      {
        printf ("\n### Invalid sparse matrix! ###\n\n");
        return 1;
      }

    output_format = CSC;
    printf ("Converting COO to CSC format...");
    seconds = get_seconds();
    errcode = sparse_matrix_convert (A, output_format);
    if ( errcode !=0 )
      {
        fprintf (stderr, "*** Failed to convert COO to CSC format...");
        return 1;
      }
    seconds = get_seconds() - seconds;
    printf ("done, in %g seconds\n", seconds);

    matrix_properties(A->repr);

    printf ("Writing to RUTHERFORD_BOEING file");
    errcode = writerb(A->repr);
    if ( errcode !=0 )
      {
        fprintf (stderr, "*** Failed to write...\n");
        return 1;
      }
    
    return 0;

}

int
matrix_properties(const struct csc_matrix_t* A)
{
   printf("\n m, n, nnz: %d %d %d \n", A->m, A->n, A->nnz);
   return 0;
}

int 
writerb(const struct csc_matrix_t* A)
{
   /* Initialize options */
   struct spral_rb_write_options options;
   spral_rb_default_write_options(&options);

   int errcode;
   printf ("\n array base: %d\n", options.array_base);
   options.array_base=1;
   printf ("\n array base: %d\n", options.array_base);

    int n = A->n;
    int nnz = A->nnz;
    long *ptr;
    ptr = (long *) malloc((n+1) * sizeof(long));
    int *row;
    row = (int *) malloc(nnz * sizeof(int));
    //double *val;
    //val = (double *) malloc(nnz * sizeof(double));

    long int i;
    /* RHS and solution vectors. */
    double *x;
    x = (double *) malloc(n * sizeof(double));

    for ( i = 0; i < n; i++ )
    {
        x[i] = 1;
        ptr[i]=A->colptr[i]+1;
    }
    ptr[n] = nnz+1;

    //const double* const values = (const double* const) (A->values);
    double* val = (double*) (A->values);
    for ( i = 0; i < nnz; i++ )
    {
        row[i] = A->rowidx[i]+1;
    }
   
   /* Write matrix */
   double seconds = 0.0;
   seconds = get_seconds();
   errcode = spral_rb_write("matrix.rb", SPRAL_MATRIX_REAL_SYM_INDEF, n, n, ptr, row,
      val, &options, "SPRAL_RUTHERFORD_BOEING test matrix", NULL);
   seconds = get_seconds() - seconds;
   printf ("done, in %g seconds\n", seconds);

   if(errcode != 0) {
      return 1;
   }

   return 0;
}
