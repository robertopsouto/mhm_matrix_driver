from scipy.io import mminfo,mmread
A=mmread('MatrixMarket_MHM_subproblem.mtx')
import matplotlib
matplotlib.use('PDF')
import matplotlib.pyplot as plt
plt.spy(A,markersize=1)
plt.savefig('MatrixMarket_MHM_subproblem')
