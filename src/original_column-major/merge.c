/* 
*   Matrix Market I/O example program
*
*   Read a real (non-complex) sparse matrix from a Matrix Market (v. 2.0) file.
*   and copies it to stdout.  This porgram does nothing useful, but
*   illustrates common usage of the Matrix Matrix I/O routines.
*   (See http://math.nist.gov/MatrixMarket for details.)
*
*   Usage:  a.out [filename] > output
*
*       
*   NOTES:
*
*   1) Matrix Market files are always 1-based, i.e. the index of the first
*      element of a matrix is (1,1), not (0,0) as in C.  ADJUST THESE
*      OFFSETS ACCORDINGLY offsets accordingly when reading and writing 
*      to files.
*
*   2) ANSI C requires one to use the "l" format modifier when reading
*      double precision floating point numbers in scanf() and
*      its variants.  For example, use "%lf", "%lg", or "%le"
*      when reading doubles, otherwise errors will occur.
*/

#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"

int main(int argc, char *argv[])
{
    int ret_code;
    MM_typecode matcode;
    FILE *f1, *f2, *fh;
    int Mh, Nh, nzh;   
    int M2, N2, nz2;   
    int i, *I, *J;
    double *val;

    if (argc < 3)
	{
		fprintf(stderr, "Usage: %s [matrix-market-input-filename] [matrix-market-output-filename]\n", argv[0]);
		exit(1);
	}
    else    
    { 
        if ((f1 = fopen(argv[1], "a")) == NULL) 
            exit(1);
        if ((f2 = fopen(argv[2], "r")) == NULL) 
            exit(1);
        if ((fh = fopen("header.txt", "r")) == NULL) 
            exit(1);
    }

    if (mm_read_banner(fh, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        //exit(1);
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

    /* find out size of sparse matrix .... */
    if ((ret_code = mm_read_mtx_crd_size(f2, &M2, &N2, &nz2)) !=0)
        exit(1);
    /* reserve memory for matrices */
    I = (int *) malloc(nz2 * sizeof(int));
    J = (int *) malloc(nz2 * sizeof(int));
    val = (double *) malloc(nz2 * sizeof(double));

    /* find out size of sparse matrix .... */
    if ((ret_code = mm_read_mtx_crd_size(fh, &Mh, &Nh, &nzh)) !=0)
        exit(1);

    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

    for (i=0; i<nz2; i++)
    {
        fscanf(f2, "%d %d %lg\n", &I[i], &J[i], &val[i]);
        J[i]= J[i] + Nh;
    }

    if (f2 !=stdin) fclose(f2);
    if (fh !=stdin) fclose(fh);

    /************************/
    /* now write out matrix */
    /************************/

    if ((fh = fopen("header.txt", "w")) == NULL) 
            exit(1);
    mm_write_banner(fh, matcode);
    mm_write_mtx_crd_size(fh, Mh, Nh+N2, nzh+nz2);
    for (i=0; i<nz2; i++)
        fprintf(f1, "%d %d %lg\n", I[i], J[i], val[i]);
        //fprintf(f1, "%d %d %12.6e\n", I[i], J[i], val[i]);
        //fprintf(f1, "%d %d %10.6g\n", I[i], J[i], val[i]);

    if (f1 !=stdin) fclose(f1);
    if (fh !=stdin) fclose(fh);

	return 0;
}

