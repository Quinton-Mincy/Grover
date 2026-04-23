import subprocess
import os
import matplotlib.pyplot as plt
import sys
import numpy as np

#def grover():


   # if sys.platform.startswith('win32'):
   #     executable = "qft.exe"
   #     run_cmd = [executable]
   # else:
   #     executable = "qft"
   #     run_cmd = ["./" + executable]
   #    
   # compile_cmd = ["g++", "-o",executable, "qft.cpp", "-Ieigen", "-lm"]
   # 
   # print("Compiling...")
   # compilation = subprocess.run(compile_cmd, capture_output=True, text=True)
   # 
   # if compilation.returncode != 0:
   #     print("Compilation Failed.")
   #     return

   # execution = subprocess.run(run_cmd)

   # #returns 1 if invalid input/target index is given
   # if execution.returncode == 0:
   #     plot_data("data.txt")

   # else:
   #     return
#import matplotlib.pyplot as plt
#
#def plot_with_grover_fit(probabilities):
#    iters = np.arange(len(probabilities))
#    y = np.array(probabilities)
#
#    # Grover parameter for N = 8
#    theta = np.arcsin(1 / np.sqrt(8))
#
#    # Smooth curve matching grover's theory
#    iters_smooth = np.linspace(0, len(probabilities) - 1, 500)
#    y_smooth = np.sin((2 * iters_smooth + 1) * theta) ** 2
#
#    plt.plot(iters_smooth, y_smooth, label="Grover Expectation")
#
#    plt.legend()
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
    
    theta = np.arcsin(1 / np.sqrt(8))
    iters_smooth = np.linspace(0, len(probabilities), 500)
    y_smooth = np.sin((2 * iters_smooth + 1) * theta) ** 2
    plt.plot(iters_smooth, y_smooth, label="Grover Expectation")
    plt.scatter(iterations, probabilities, marker='o', linestyle='-', color='r', label = "Simulation Data")

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
