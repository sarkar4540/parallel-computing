import subprocess
from subprocess import Popen, PIPE
import numpy as np
import matplotlib.pyplot as plt

x=np.array([])
y_time_sparse=np.array([])
y_count_sparse=np.array([])
y_time_dense=np.array([])
y_count_dense=np.array([])

for sp in range(1, 50):

    cmd = ["./matmul", "-m", "100", "-n", "100", "-p", "100", "-spA", str(sp/100), "-spB", str(sp/100)]

    result = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    out = result.stdout.read().decode("utf-8").split(",")
    x=np.append(x,sp/100)
    y_time_sparse=np.append(y_time_sparse,int(out[0]))
    y_count_sparse=np.append(y_count_sparse,int(out[1]))
    y_time_dense=np.append(y_time_dense,int(out[2]))
    y_count_dense=np.append(y_count_dense,int(out[3]))
    print(sp/100,out)




plt.plot(x, y_time_dense, label ='Dense Matrix Format')
plt.plot(x, y_time_sparse, label ='Sparse Matrix Format')
plt.legend()
plt.xlabel('Sparsity')
plt.ylabel('Runtime (in ms)')
plt.show()

plt.plot(x, y_count_dense, label ='Dense Matrix Format')
plt.plot(x, y_count_sparse, label ='Sparse Matrix Format')
plt.legend()
plt.xlabel('Sparsity')
plt.ylabel('Total Operations')
plt.show()