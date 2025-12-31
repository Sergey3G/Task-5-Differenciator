import sys
import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(-3.000000, 3.000000, 2000)
f = lambda x: np.sin(2 ** x)
teyl = lambda x: 0.84147 + 0.37451 * x + -0.072349 * x ** 2 + -0.14012 * x ** 3
tan = lambda x: 0.84147 + 0.37451 * (x - 0)

with np.errstate(divide='ignore', invalid='ignore'):
    f_f = f(x)
    f_T = teyl(x)
    f_tan = tan(x)

plt.plot(x, f_f, label='f(x)')
plt.plot(x, f_T, label='Taylor')
plt.plot(x, f_tan, label='Tangent')
plt.axvline(0.000000, color='gray', alpha=0.5)
plt.legend()
plt.grid(True)
plt.savefig(sys.argv[1])
plt.show()
