#!/bin/bash

head -n 2 MatrixMarket_MHM_subproblem.txt0 > MatrixMarket_MHM_subproblem.mtx
awk 'NR==2{$2=0}1' MatrixMarket_MHM_subproblem.mtx | awk 'NR==2{$3=0}1' > header.txt

find . -iname "MatrixMarket_MHM_subproblem.txt?" | sort > arqs.txt

for arq in $(cat arqs.txt);do
   ./merge MatrixMarket_MHM_subproblem.mtx ${arq}
done

s1=`awk '{ if (NR == 2) print $0}' header.txt`
s2=`awk '{ if (NR == 2) print $0}' MatrixMarket_MHM_subproblem.mtx`

sed "s|${s2}|${s1}|" MatrixMarket_MHM_subproblem.mtx > temp.mtx
mv temp.mtx MatrixMarket_MHM_subproblem.mtx
