from matplotlib.markers import MarkerStyle
import matplotlib.pyplot as plt
import math
import os

fp = open(os.path.join("inserts.txt")).readlines()
sizes = [int(line.split()[0]) for line in fp]
fib = [float(line.split()[1]) for line in fp]
_bin = [float(line.split()[2]) for line in fp]
pair = [float(line.split()[3]) for line in fp]


plt.plot(sizes, fib)
plt.plot(sizes, _bin)
plt.plot(sizes, pair)

plt.title("insert operation")

plt.legend(["Fibonacci Heap", "Binary Heap", "Pairing Heap"])
plt.xlabel("Size of input")
plt.ylabel("Time")
plt.show()
