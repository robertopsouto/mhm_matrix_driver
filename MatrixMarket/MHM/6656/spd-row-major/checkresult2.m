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

[xsort,id]=sort(x);
[xpardisosort,id]=sort(xpardiso);
d=xsort-xpardisosort;
d=d./x;
d=abs(d*100);
disp('new max percentual difference:');
max(d)

figure(1);
%plot(xsort)
hold on;
plot(xpardisosort,'r')
figure(2);
%plot(x)
hold on;
plot(xpardiso,'r')
