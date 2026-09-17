import os
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# --- Model Definition ---
# Standard 3rd-order Log-Polynomial HPGe Efficiency Model
# def hpge_log_poly(E, a0, a1, a2, a3):
def hpge_log_poly(E, a0, a1, a2, a3):
    ln_E = np.log(E)
    ln_eff = a0 + a1 * ln_E + a2 * (ln_E**2) + a3 * (ln_E**3)
    # ln_eff = (((A + B(ln_E/100))**(-G)) + ((C + D(ln_E/1000))**(-G)))**(-1/G) 
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
    7.475484,
    6.489832,
    3.772965,
    3.568930,
    3.310685,
    3.360150,
    3.012904,
    2.495270
])

# --- Fit Execution ---
p0 = [1.0, -1.0, -0.1, -0.01] # Initial guess for parameters
popt, pcov = curve_fit(hpge_log_poly, energy, eff, p0=p0)
perr = np.sqrt(np.diag(pcov))

# --- Results Output ---
labels = ['a0', 'a1', 'a2', 'a3']
print("Fit Results:")
for i in range(len(labels)):
    print(f"{labels[i]:<10}: {popt[i]:.10f} +/- {perr[i]:.10f}")    

# plt.show()