clear all;
close all;
A=mmread('MatrixMarket_MHM_subproblem.mtx');
[n,m]=size(A);
rhs=ones(n,1);
x=A\rhs;
load('xpardiso.dat');
d=x-xpardiso;
d=d./x;
d=abs(d*100);
disp('max percentual difference:');
max(d)

%[xsort,id]=sort(x);
%[xpardisosort,id]=sort(xpardiso);
%d=x-xpardixo;
%disp('max absolute difference: x-xpardixo');
%max(abs(d));

%figure(1);
%plot(xsort)
%hold on;
%plot(xpardisosort,'r')
%figure(2);
%plot(x)
%hold on;
%plot(xpardiso,'r')
