from pylab import *
import numpy as np

with open('silencer.log') as j:
    jacobi_times = j.readlines()
jacobi_times = [x.strip() for x in jacobi_times]

#with open('gauss_seidl_times') as g:
#    gauss_seidl_times = g.readlines()
#gauss_seidl_times = [x.strip() for x in gauss_seidl_times]

#with open('gaussLU_times') as lu:
#    gaussLU_times = lu.readlines()
#gaussLU_times = [x.strip() for x in gaussLU_times]


x = np.linspace(1, 4000, 100, endpoint=True)
plot(x, jacobi_times, label='Jacobi')
#plot(x, gauss_seidl_times, label='Gauss-Seidl')
#plot(x, gaussLU_times, label='GaussLU')
grid(True)
xlabel('liczba niewiadomych N')
ylabel('czas wykonywania [ms]')
legend()
show()
