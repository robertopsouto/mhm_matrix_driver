#!/bin/bash

gcc main.c -o main -I/home/rpsouto/programming/libraries/sparse/bebop/sparse_matrix_converter/include -I/home/rpsouto/programming/libraries/sparse/bebop/bebop_util/include -L/home/rpsouto/programming/libraries/sparse/bebop/bebop_util -lbebop_util -L/home/rpsouto/programming/libraries/sparse/bebop/sparse_matrix_converter -lsparse_matrix_converter
#icc main.c -o main -I/home/rpsouto/programming/libraries/sparse/bebop/sparse_matrix_converter/include -I/home/rpsouto/programming/libraries/sparse/bebop/bebop_util/include -L/home/rpsouto/programming/libraries/sparse/bebop/bebop_util -lbebop_util -L/home/rpsouto/programming/libraries/sparse/bebop/sparse_matrix_converter -lsparse_matrix_converter
