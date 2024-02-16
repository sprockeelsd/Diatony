import matplotlib.pyplot as plt

with_restart = [6.639703, 7.130334, 7.141022, 6.865706, 5.670035, 7.503590, 0.943873, 0.597786, 0.910740, 0.770388, 0.748090, 0.834639, 0.522258,
                0.864266, 1.242898, 0.835493, 0.642486, 0.633051, 26.073171, 6.142389, 30.651358, 7.049500, 10.995183, 22.478803, 115.635702,
                85.096064, 107.758557, 92.780075, 77.790846, 119.866340, 21.252934, 216.704336, 23.583592, 161.187985, 133.389053,22.55206
                ]
without_res = [7.336618, 8.366203, 8.685200, 9.198524, 11.633881, 13.036704, 0.514980, 1.398208, 13.099731, 1.220693, 0.454456, 0.708182,
                    0.315143, 1.294082, 0.652869, 0.930813, 0.475396, 0.592219, 26.372885, 6.397904, 32.604014, 7.094531, 8.497084, 22.299900,
                    108.557939, 449.960955, 449.959911, 449.960406, 69.686376, 110.721763, 27.040256, 449.945861, 449.945228, 449.953803,
                    116.656015, 17.083386
                ]

without_restart = values = [min(val, 300) for val in without_res]

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