clear all;
close all;
A=mmread('MatrixMarket_MHM_subproblem.mtx');
[n,m]=size(A);
%rhs=ones(n,1);
rhs=mmread('MatrixMarket_MHM_subproblem_rhs.mtx');
x=A\rhs;
load('xpardiso.dat');
d=x-xpardiso;
d=d./x;
d=abs(d*100);
disp('max percentual difference:');
max(d)

b=load("b.out");
