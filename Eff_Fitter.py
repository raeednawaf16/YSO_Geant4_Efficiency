import os
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# --- Model Definition ---
# Standard 3rd-order Log-Polynomial HPGe Efficiency Model
# def hpge_log_poly(E, a0, a1, a2, a3):
def hpge_log_poly(E, A, B, C, D, G):
    ln_E = np.log(E)
    # ln_eff = a0 + a1 * ln_E + a2 * (ln_E**2) + a3 * (ln_E**3)
    ln_eff = (((A + B(ln_E/100))**(-G)) + ((C + D(ln_E/1000))**(-G)))**(-1/G) 
    return np.exp(ln_eff)

# --- Data for Single Fit ---
energy = np.array([
    121.53,
    244.6,
    344.3,
    778.9,
    867.3,
    964,
    1085.9,
    1112,
    1408
])

eff = np.array([
    10.000000,
    6.997516,
    5.458623,
    2.688655,
    2.577766,
    2.300206,
    2.193434,
    2.029822,
    1.662159
])

# --- Fit Execution ---
p0 = [3, 2, 1, 0.8, 10] # Initial guess for parameters
popt, pcov = curve_fit(hpge_log_poly, energy, eff, p0=p0)
perr = np.sqrt(np.diag(pcov))

# --- Results Output ---
labels = ['A', 'B', 'C', 'D', 'G']
print("Fit Results:")
for i in range(len(labels)):
    print(f"{labels[i]:<10}: {popt[i]:.10f} +/- {perr[i]:.10f}")    

# plt.show()