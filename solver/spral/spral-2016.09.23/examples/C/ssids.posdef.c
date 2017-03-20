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
solve(const struct csc_matrix_t* A);

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

    errcode = solve(A->repr);
    if ( errcode !=0 )
      {
        fprintf (stderr, "*** Failed to solve...\n");
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
solve(const struct csc_matrix_t* A)
{
   /* Derived types */
   void *akeep, *fkeep;
   struct spral_ssids_options options;
   struct spral_ssids_inform inform;

   /* Initialize derived types */
   akeep = NULL; fkeep = NULL; /* Important that these are NULL to start with */
   spral_ssids_default_options(&options);

   bool posdef = true;

   /* Data for matrix:
    * ( 2  1         )
    * ( 1  4  1    1 )
    * (    1  3  2   )
    * (       2 -1   )
    * (    1       2 ) */
   /*
   bool posdef = false;
   int n = 5;
   long ptr[]   = { 1,        3,             6,         8,   9,  10 };
   int row[]    = { 1,   2,   2,   3,   5,   3,   4,    4,   5   };
   double val[] = { 2.0, 1.0, 4.0, 1.0, 1.0, 3.0, 2.0, -1.0, 2.0 };
   */

   /* The right-hand side with solution (1.0, 2.0, 3.0, 4.0, 5.0) */
   //double x[] = { 4.0, 17.0, 19.0, 2.0, 12.0 };

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
   
   

   /* Perform analyse and factorise with data checking */
   bool check = true;
   printf("ptr[0]: %li \n",ptr[0]);

   printf ("Analysing ...");
   double seconds = 0.0;
   seconds = get_seconds();
   spral_ssids_analyse(check, n, NULL, ptr, row, NULL, &akeep, &options,
         &inform);
   seconds = get_seconds() - seconds;
   printf ("done, in %g seconds\n", seconds);

   if(inform.flag<0) {
      spral_ssids_free(&akeep, &fkeep);
      return 1;
   }

   printf ("Factoring ...");
   seconds = get_seconds();
   spral_ssids_factor(posdef, NULL, NULL, val, NULL, akeep, &fkeep, &options,
         &inform);
   if(inform.flag<0) {
      spral_ssids_free(&akeep, &fkeep);
      return 1;
   }
   seconds = get_seconds() - seconds;
   printf ("done, in %g seconds\n", seconds);

   /* Solve */
   printf ("Solving ...");
   seconds = get_seconds();
   spral_ssids_solve1(0, x, akeep, fkeep, &options, &inform);
   if(inform.flag<0) {
      spral_ssids_free(&akeep, &fkeep);
      return 1;
   }
   seconds = get_seconds() - seconds;
   printf ("done, in %g seconds\n", seconds);


   FILE *ofp;
   ofp = fopen("resultgpu.dat", "w");

   printf("The computed solution is:\n");
   for(int i=0; i<n; i++) fprintf(ofp," %18.10e \n", x[i]);
   printf("\n");

   fclose(ofp);

   /* Determine and print the pivot order */
   int piv_order[n];
   //spral_ssids_enquire_indef(akeep, fkeep, &options, &inform, piv_order, NULL);
   //spral_ssids_enquire_posdef(akeep, fkeep, &options, &inform, d);
   //printf("Pivot order:");
   //for(int i=0; i<n; i++) printf(" %5d", piv_order[i]);
   printf("\n");

   int cuda_error = spral_ssids_free(&akeep, &fkeep);
   if(cuda_error!=0) return 1;
}
