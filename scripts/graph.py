import sys
import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(-0.281718, 5.718282, 2000)
f = lambda x: 2.7183 ** np.log(x)
teyl = lambda x: 2.7183 + (x - 2.7183)
tan = lambda x: 2.7183 + 1 * (x - 2.7183)

with np.errstate(divide='ignore', invalid='ignore'):
    f_f = f(x)
    f_T = teyl(x)
    f_tan = tan(x)

plt.plot(x, f_f, label='f(x)')
plt.plot(x, f_T, label='Taylor')
plt.plot(x, f_tan, label='Tangent')
plt.axvline(2.718282, color='gray', alpha=0.5)
plt.legend()
plt.grid(True)
plt.savefig(sys.argv[1])
plt.show()
