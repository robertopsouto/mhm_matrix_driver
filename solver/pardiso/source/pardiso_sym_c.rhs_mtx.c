/*******************************************************************************
* Copyright 2004-2015 Intel Corporation All Rights Reserved.
*
* The source code,  information  and material  ("Material") contained  herein is
* owned by Intel Corporation or its  suppliers or licensors,  and  title to such
* Material remains with Intel  Corporation or its  suppliers or  licensors.  The
* Material  contains  proprietary  information  of  Intel or  its suppliers  and
* licensors.  The Material is protected by  worldwide copyright  laws and treaty
* provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
* modified, published,  uploaded, posted, transmitted,  distributed or disclosed
* in any way without Intel's prior express written permission.  No license under
* any patent,  copyright or other  intellectual property rights  in the Material
* is granted to  or  conferred  upon  you,  either   expressly,  by implication,
* inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
* property rights must be express and approved by Intel in writing.
*
* Unless otherwise agreed by Intel in writing,  you may not remove or alter this
* notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
* suppliers or licensors in any way.
*******************************************************************************/

/*
*   Content : MKL PARDISO C example
*
********************************************************************************
*/
/* -------------------------------------------------------------------- */
/* Example program to show the use of the "PARDISO" routine */
/* on symmetric linear systems */
/* -------------------------------------------------------------------- */
/* This program can be downloaded from the following site: */
/* www.pardiso-project.org */
/* */
/* (C) Olaf Schenk, Department of Computer Science, */
/* University of Basel, Switzerland. */
/* Email: olaf.schenk@unibas.ch */
/* -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mmio.h"

#include "mkl_pardiso.h"
#include "mkl_types.h"

int main(int argc, char *argv[])
//MKL_INT main (void)
{
    /* Matrix Market reading */
    int ret_code;
    MM_typecode matcode, rhs_matcode;
    FILE *f, *frhs, *fo;
    MKL_INT m, n, nnz, nnzmax;
    MKL_INT i, *rowind, *colind;
    double *acoo;

    if (argc < 3)
        {
                fprintf(stderr, "Usage: %s [matrix-market-filename (.mtx)] [rhs-matrix-market-filename (.mtx)]\n", argv[0]);
                exit(1);
        }
    else
    {
        if ((f = fopen(argv[1], "r")) == NULL)
            exit(1); 
        if ((frhs = fopen(argv[2], "r")) == NULL)
            exit(1); 
    }

    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    if (mm_read_banner(frhs, &rhs_matcode) != 0)
    {
        printf("Could not process RHS Matrix Market banner.\n");
        exit(1);
    }

    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
            mm_is_sparse(matcode) )
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(f, &m, &n, &nnz)) !=0)
        exit(1);

    printf("m: %d\n", m);
    printf("n: %d\n", n);
    printf("nnz: %d\n", nnz);
    nnzmax = nnz*2-n;

    /* reserve memory for matrices */

    rowind = (MKL_INT *) malloc(nnz * sizeof(MKL_INT));
    colind = (MKL_INT *) malloc(nnz * sizeof(MKL_INT));
    acoo = (double *) malloc(nnz * sizeof(double));

    /* Matrix CSR indexes */
    MKL_INT *ia, *ja;
    ia = (MKL_INT *) malloc((n+1) * sizeof(MKL_INT));
    ja = (MKL_INT *) malloc((nnz) * sizeof(MKL_INT));
    double *acsr;
    acsr = (double *) malloc(nnz * sizeof(double));
    /*

    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

    for (i=0; i<nnz; i++)
    {
        fscanf(f, "%d %d %lg\n", &rowind[i], &colind[i], &acoo[i]);
        //rowind[i]--;  /* adjust from 1-based to 0-based */
        //colind[i]--;
        acsr[i]=acoo[i];
        ja[i]=colind[i];
    }

    if (f !=stdin) fclose(f);

    /*
    //MKL_INT n = 8;
    MKL_INT ia[9] = { 1, 5, 8, 10, 12, 15, 17, 18, 19};
    MKL_INT ja[18] = 
    { 1,    3,       6, 7,
         2, 3,    5,
            3,             8,
               4,       7,
                  5, 6, 7,
                     6,    8,
                        7,
                           8
    }; 
    double a[18] = 
    { 7.0,      1.0,           2.0, 7.0,
          -4.0, 8.0,      2.0,
                1.0,                     5.0,
                     7.0,           9.0,
                          5.0, 1.0, 5.0,
                              -1.0,      5.0,
                                   11.0,
                                         5.0
    };*/


//    mkl_dcsrcoo (const MKL_INT *job , const MKL_INT *n , double *acsr , MKL_INT *ja , MKL_INT *ia , MKL_INT *nnz , double *acoo , MKL_INT *rowind , MKL_INT *colind , MKL_INT *info );

   //     for coo to csr converter
   MKL_INT job[6] ,info;

   // ====== csr2coo  parameters =====
   job[0]=1;   //the matrix in the coordinate format is converted to the CSR format
   job[1]=1;   //one-based indexing for the matrix in CSR format is used;
   job[2]=1;   //one-based indexing for the matrix in coordinate format is used;
   job[4]=nnzmax; //maximum number of the non-zero elements allowed if job[0]=0
   job[5]=1;   //only array ia is filled in for the output storage
//   job[5]=0;   // all arrays acsr, ja, ia are filled in for the output storage

    mkl_dcsrcoo (job , &n , acsr , ja , ia , &nnz , acoo , rowind , colind , &info );
    printf("info: %d\n", info);
//    printf("nnz: %d\n", nnz);
    printf("ia(n+1): %d\n", ia[n]);
/*
    for ( i = 0; i <=n; i++ )
    {
        printf("%d\n", ia[i]);
    }
*/
/*
    for ( i = 0; i < nnz; i++ )
    {
       printf("%d   %d   %lg\n", i+1, ja[i], acsr[i]);
    }
*/
    MKL_INT mtype = -2;       /* real and symmetric positive definite */
    /* RHS and solution vectors. */
    double *b, *x;
    b = (double *) malloc(n * sizeof(double));
    x = (double *) malloc(n * sizeof(double));
    
    //double b[9], x[8];
    //
    MKL_INT nrhs = 1;     /* Number of right hand sides. */
    /* Internal solver memory pointer pt, */
    /* 32-bit: int pt[64]; 64-bit: long int pt[64] */
    /* or void *pt[64] should be OK on both architectures */
    void *pt[64];
    /* Pardiso control parameters. */
    MKL_INT iparm[64];
    MKL_INT maxfct, mnum, phase, error, msglvl;
    /* Auxiliary variables. */
    //MKL_INT i;
    double ddum;          /* Double dummy */
    MKL_INT idum;         /* Integer dummy. */
/* -------------------------------------------------------------------- */
/* .. Setup Pardiso control parameters. */
/* -------------------------------------------------------------------- */


    for ( i = 0; i < 64; i++ )
    {
        iparm[i] = 0;
    }
    iparm[0] = 0;         /* Solver default */
    iparm[1] = 2;         /* Fill-in reordering from METIS */

//    iparm[0] = 1;         /* No solver default */
//    iparm[1] = 2;         /* Fill-in reordering from METIS */
//    iparm[3] = 0;         /* No iterative-direct algorithm */
//   iparm[4] = 0;         /* No user fill-in reducing permutation */
//    iparm[5] = 0;         /* Write solution into x */
//    iparm[6] = 0;         /* Not in use */
//    iparm[7] = 2;         /* Max numbers of iterative refinement steps */
//    iparm[8] = 0;         /* Not in use */
//    iparm[9] = 8;        /* Perturb the pivot elements with 1E-13 */
//    iparm[10] = 0;        /* Use nonsymmetric permutation and scaling MPS */
//    iparm[11] = 0;        /* Not in use */
//    iparm[12] = 0;        /* Maximum weighted matching algorithm is switched-off (default for symmetric). Try iparm[12] = 1 in case of inappropriate accuracy */
//    iparm[13] = 0;        /* Output: Number of perturbed pivots */
//    iparm[14] = 0;        /* Not in use */
//    iparm[15] = 0;        /* Not in use */
//    iparm[16] = 0;        /* Not in use */
//    iparm[17] = -1;       /* Output: Number of nonzeros in the factor LU */
//    iparm[18] = -1;       /* Output: Mflops for LU factorization */
//    iparm[19] = 0;        /* Output: Numbers of CG Iterations */
//    iparm[34] = 0;        //Zero-based indexing: columns and rows indexing in arrays ia, ja, and perm starts from 0 

    maxfct = 1;           /* Maximum number of numerical factorizations. */
    mnum = 1;         /* Which factorization to use. */
    msglvl = 1;           /* Print statistical information in file */
    error = 0;            /* Initialize error flag */
/* -------------------------------------------------------------------- */
/* .. Initialize the internal solver memory pointer. This is only */
/* necessary for the FIRST call of the PARDISO solver. */
/* -------------------------------------------------------------------- */
    for ( i = 0; i < 64; i++ )
    {
        pt[i] = 0;
    }
/* -------------------------------------------------------------------- */
/* .. Reordering and Symbolic Factorization. This step also allocates */
/* all memory that is necessary for the factorization. */
/* -------------------------------------------------------------------- */
    phase = 11;
    PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
             &n, acsr, ia, ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
    if ( error != 0 )
    {
        printf ("\nERROR during symbolic factorization: %d", error);
        exit (1);
    }
    printf ("\nReordering completed ... ");
    printf ("\nNumber of nonzeros in factors = %d", iparm[17]);
    printf ("\nNumber of factorization MFLOPS = %d", iparm[18]);
/* -------------------------------------------------------------------- */
/* .. Numerical factorization. */
/* -------------------------------------------------------------------- */
/*    phase = 22;
    PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
             &n, acsr, ia, ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
    if ( error != 0 )
    {
        printf ("\nERROR during numerical factorization: %d", error);
        exit (2);
    }
    printf ("\nFactorization completed ... "); */
/* -------------------------------------------------------------------- */
/* .. Numerical factorization, back substitution and iterative refinement. */
/* -------------------------------------------------------------------- */
    phase = 23;
    iparm[7] = 2;         /* Max numbers of iterative refinement steps. */
    /* Set right hand side to one. */
    /*for ( i = 0; i < n; i++ )
    {
        b[i] = 1;
    }*/
    /* Set right hand side. */
    /* reading the second line of header from mtx file */
    fscanf(frhs, "%d %d %lg\n", &rowind[0], &colind[0], &b[0]);
    /* reading the values of rhs from mtx file */
    for ( i = 0; i < n; i++ )
    {
        fscanf(frhs, "%d %d %lg\n", &rowind[i], &colind[i], &b[i]);
    }

    PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
             &n, acsr, ia, ja, &idum, &nrhs, iparm, &msglvl, b, x, &error);
    if ( error != 0 )
    {
        printf ("\nERROR during solution: %d", error);
        exit (3);
    }
    printf ("\nSolve completed ... ");
    printf ("\nThe solution of the system is: ");

    fo = fopen("xpardiso.dat", "w");
    for ( i = 0; i < n; i++ )
    {
        fprintf (fo,"%f \n",x[i]);
    }
    fclose(fo);

    for ( i = 0; i < 10; i++ )
    {
        printf ("\n x [%d] = % f", i, x[i]);
    }

    printf ("\n");
/* -------------------------------------------------------------------- */
/* .. Termination and release of memory. */
/* -------------------------------------------------------------------- */
    phase = -1;           /* Release internal memory. */
    PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
             &n, &ddum, ia, ja, &idum, &nrhs,
             iparm, &msglvl, &ddum, &ddum, &error);
    return 0;
}
