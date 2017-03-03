clear all;
close all;
A=mmread('MatrixMarket_MHM_subproblem.mtx');
[n,m]=size(A);
%rhs=ones(n,1);
rhs=mmread('MatrixMarket_MHM_subproblem_rhs.mtx');
x=A\rhs;
load('xpardiso.dat');
d=x-xpardiso;
dp=d./x;
d=abs(d*100);
disp('max percentual difference:');
max(d)

[xsort,id]=sort(x);
[xpardisosort,id]=sort(xpardiso);
d=xsort-xpardisosort;
dp=d./x;
d=abs(d*100);
disp('new max percentual difference:');
max(d)

figure(1);
hold on;
plot(xpardisosort,'r')
plot(xsort,'g')
figure(2);
hold on;
plot(xpardiso,'r')
plot(x,'g')
