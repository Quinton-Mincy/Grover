import os
import matplotlib.pyplot as plt
import numpy as np

def grover(filename):
    iterations = []
    probabilities = []

    if not os.path.exists(filename):
        print(f"Error: {filename} not found.")
        return

    with open(filename, 'r') as f:
        args = next(f).split()
        (qbits,d0,dt,iters) = args
        for line in f:
            #defauts to whitepsace
            cols = line.split()
            if len(cols) == 2:
                iterations.append(float(cols[0]))
                probabilities.append(float(cols[1]))

    # 5. Create the plot
    plt.figure(figsize=(10, 6))
    plt.scatter(iterations, probabilities, marker='o', linestyle='-', color='r', label = "Simulation Data")
    
    theta = np.arcsin(1 / np.sqrt(2**int(qbits)))
    #theta = np.arcsin(1 / np.sqrt(8))
    iters_smooth = np.linspace(0, len(probabilities), 500)
    y_smooth = np.sin((2 * iters_smooth + 1) * theta) ** 2
    plt.plot(iters_smooth, y_smooth, label="Grover Expectation")

    plt.title(rf"Probability Oscillations"
              f"\n"
              rf"{qbits} Qubits. Input state: $\delta_{{{d0}}}$. Target state: $\delta_{{{dt}}}$. {iters} Iterations")
    plt.xlabel("Iterations")
    plt.ylabel("Probabilities")
    plt.grid(True)
    plt.legend(loc="lower right")
    plt.show()

if __name__ == "__main__":
    grover("data.txt")
