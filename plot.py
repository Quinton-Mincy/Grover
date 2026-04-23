import subprocess
import os
import matplotlib.pyplot as plt
import sys

def grover():


    if sys.platform.startswith('win32'):
        executable = "qft.exe"
        run_cmd = [executable]
    else:
        executable = "qft"
        run_cmd = ["./" + executable]
       
    compile_cmd = ["g++", "-o",executable, "qft.cpp", "-Ieigen", "-lm"]
    
    print("Compiling...")
    compilation = subprocess.run(compile_cmd, capture_output=True, text=True)
    
    if compilation.returncode != 0:
        print("Compilation Failed.")
        return

    execution = subprocess.run(run_cmd)

    #returns 1 if invalid input/target index is given
    if execution.returncode == 0:
        plot_data("data.txt")

    else:
        return

def plot_data(filename):
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
    plt.plot(iterations, probabilities, marker='o', linestyle='-', color='b')
    plt.title(rf"Probability Oscillations"
              f"\n"
              rf"{qbits} Qubits. Input state: $\delta_{{{d0}}}$. Target state: $\delta_{{{dt}}}$. {iters} Iterations")
    plt.xlabel("Iterations")
    plt.ylabel("Probabilities")
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    grover()
