'''
Plotting Program to complement qft.cpp
'''
import numpy as np
import matplotlib.pyplot as plt

def plot_with_grover_fit(probabilities):
    iters = np.arange(len(probabilities))
    y = np.array(probabilities)

    # Grover parameter for N = 8
    theta = np.arcsin(1 / np.sqrt(8))

    # Smooth curve matching grover's theory
    iters_smooth = np.linspace(0, len(probabilities) - 1, 500)
    y_smooth = np.sin((2 * iters_smooth + 1) * theta) ** 2

    plt.figure()
    plt.plot(iters_smooth, y_smooth, label="Grover Expectation")
    plt.plot(iters, y, 'o', label="Simulation Data")
    plt.xlabel("Iteration")
    plt.ylabel("Probability of Target State")
    plt.title("Grover's Algorithm Simulation for 3-Qubits Targeting |011>")
    plt.grid(True)
    plt.legend()
    plt.show()
    
    
if __name__ == "__main__":
    
    probabilities = [0.125,0.78125,0.945313,0.330078,0.012207,0.547974,0.999786,0.576973,0.0194569,0.302891,0.931266,0.804925,0.144965,0.106316,0.756614,0.957837,0.357846,0.0066241,0.51881,0.998078,0.605709,0.0283488,0.276378,0.915746,0.827558,0.166144,0.0889775,0.7311,0.968798,0.386099,0.00272716,0.489583,0.994669,0.634084,0.0388525,0.250629,0.898805,0.849072,0.188463,0.0730435]

    plot_with_grover_fit(probabilities)