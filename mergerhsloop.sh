#!/bin/bash

head -n 2 MatrixMarket_MHM_subproblem.txt_rhs0 > MatrixMarket_MHM_subproblem_rhs.mtx
awk 'NR==2{$1=0}1' MatrixMarket_MHM_subproblem_rhs.mtx | awk 'NR==2{$3=0}1' > header.txt

find . -iname "MatrixMarket_MHM_subproblem.txt_rhs?" | sort > arqs.txt

for arq in $(cat arqs.txt);do
   ./mergerhs MatrixMarket_MHM_subproblem_rhs.mtx ${arq}
done

s1=`awk '{ if (NR == 2) print $0}' header.txt`
s2=`awk '{ if (NR == 2) print $0}' MatrixMarket_MHM_subproblem_rhs.mtx`

sed "s|${s2}|${s1}|" MatrixMarket_MHM_subproblem_rhs.mtx | sed "s/symmetric/general/" > temp.mtx
mv temp.mtx MatrixMarket_MHM_subproblem_rhs.mtx
