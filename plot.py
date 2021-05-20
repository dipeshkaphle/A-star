from matplotlib.markers import MarkerStyle
import matplotlib.pyplot as plt
import math
import os


binheap = open(os.path.join("Bin.txt")).readlines()
fibheap = open(os.path.join("Fibo.txt")).readlines()
pairingheap = open(os.path.join("Pair.txt")).readlines()

n_values = [int(line.split(':')[0]) for line in binheap]
time_bin = [float(line.split(':')[1]) for line in binheap]
time_fib = [float(line.split(':')[1]) for line in fibheap]
time_pairing = [float(line.split(':')[1]) for line in pairingheap]

plt.plot(n_values, time_fib, marker='+')
plt.plot(n_values, time_bin, marker='*')
plt.plot(n_values, time_pairing, marker='*')
plt.ylabel("Times")
plt.xlabel("V")
plt.title("A* implementation with diff heaps")
plt.legend(["Fibonacci Heap", "Binary Heap", "Pairing Heap"])
plt.xlabel("No of nodes in graph")
plt.ylabel("Time")
plt.show()
