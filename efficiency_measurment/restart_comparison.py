import matplotlib.pyplot as plt

with_restart = [4.828066, 5.199222, 7.345299, 7.408935, 5.185392, 7.233157, 0.622676, 0.556763, 0.424870, 0.415148, 0.520834, 0.749219, 
                0.324900, 0.675077, 0.777038, 0.644770, 0.531998, 0.619174, 1.710342, 0.612256, 2.271011, 0.526332, 0.556473, 1.310051, 
                4.506562, 8.274826, 6.597391, 7.198861, 4.353386, 5.184589, 12.276587, 43.496361, 9.995491, 33.308981, 33.542040, 11.692890]

without_res = [4.440985, 8.377433, 6.522765, 6.223050, 4.210800, 7.260432, 0.438615, 0.418507, 1.087747, 0.916572, 0.238763, 0.349300, 
               0.193747, 0.936281, 0.666608, 0.713823, 0.283971, 0.322005, 1.854145, 0.570140, 2.009448, 0.608240, 0.816699, 1.442330, 
               4.597969, 6.661876, 315.137349, 206.028804, 2.583442, 2.879752, 9.941100, 46.721546, 450, 450, 32.711958, 10.326515]

without_restart = values = [min(val, 60) for val in without_res]

n = len(with_restart)
indices = range(1,n+1)
width = 0.35

bar1 = plt.bar(indices, with_restart, width, label='With restart', align = 'edge')
bar2 = plt.bar(indices, without_restart, -width, label='Without restart', align = 'edge')

plt.xlabel('Test cases',fontsize=30)
plt.ylabel('Time taken to prove optimality (s)',fontsize=30)
plt.title('Comparison of the time taken to prove optimality with and without restart',fontsize=30)
plt.xticks(indices, range(1, n+1))
plt.legend(fontsize=30)
plt.show()