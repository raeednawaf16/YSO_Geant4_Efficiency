import os
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# --- Model Definition ---
# Standard 3rd-order Log-Polynomial HPGe Efficiency Model
# def hpge_log_poly(E, a0, a1, a2, a3):
def hpge_log_poly(E, a0, a1, a2, a3, a4):
    ln_E = np.log(E)
    ln_eff = a0 + a1 * ln_E + a2 * (ln_E**2) + a3 * (ln_E**3) + a4 * (ln_E**4)
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
p0 = [1.0, -1.0, -0.1, -0.01, -0.001] # Initial guess for parameters
popt, pcov = curve_fit(hpge_log_poly, energy, eff, p0=p0)
perr = np.sqrt(np.diag(pcov))

# --- Results Output ---
labels = ['a0', 'a1', 'a2', 'a3', 'a4']
print("Fit Results:")
for i in range(len(labels)):
    print(f"{labels[i]:<10}: {popt[i]:.10f} +/- {perr[i]:.10f}")    


# --- Generate Smooth Curve Data ---
# Generate 100 evenly spaced energy values between 100 and 1500 keV
smooth_energy = np.linspace(100, 1500, 100)

# Calculate the fitted efficiency using the optimized parameters (*popt unpacks the array)
smooth_eff = hpge_log_poly(smooth_energy, *popt)

print("\nGenerated Curve Data (Energy, Efficiency):")
for e, ev in zip(smooth_energy, smooth_eff):
    print(f"{e:.2f}\t{ev:.6f}")

# # --- Plotting ---
# plt.figure(figsize=(8, 5))

# # Plot original experimental data as scatter points
# plt.scatter(energy, eff, color='red', label='Experimental Data', zorder=5)

# # Plot the smooth fitted curve
# plt.plot(smooth_energy, smooth_eff, color='blue', label='4th-Order Log-Poly Fit')

# # Formatting
# plt.xlabel('Energy (keV)')
# plt.ylabel('Efficiency (%)')
# plt.title('HPGe Add-Back Efficiency Curve')
# plt.legend()
# plt.grid(True, linestyle='--', alpha=0.6)

# # Display the plot
# plt.show()
