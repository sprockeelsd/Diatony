import matplotlib.pyplot as plt

filename = "out/search-stats-2024-01-30_09-58-52.csv"

data = []
#read file
with open(filename, 'r') as csvfile:
    for line in csvfile.readlines():
        #for each line, separate each element and return a list of lists
        line_data = [elem.strip() for elem in line.split(',')]
        data.append(line_data)

#remove first line
values = data[1:]

#isolate the data we want to plot
times = []
n4_note_dim_chords = []
n_3_note_chords = []
n_fund_chords_no_double_bass = []
n_incomplete_chords = []
n_common_notes_tenor = []
n_common_notes_alto = []
n_common_notes_soprano = []
melodic_intervals_costs = []

for val in values:
    curr_t = []
    curr_dim = []
    curr_3 = []
    curr_fund = []
    curr_inc = []
    curr_tenor = []
    curr_alto = []
    curr_soprano = []
    curr_mel = []
    temp = val[32:]
    i = 0
    while i + 10 < len(temp):
        curr_t.append(float(temp[i]))
        curr_dim.append(int(temp[i+1]))
        curr_3.append(int(temp[i+2]))
        curr_fund.append(int(temp[i+3]))
        curr_inc.append(int(temp[i+4]))
        curr_tenor.append(int(temp[i+5]))
        curr_alto.append(int(temp[i+6]))
        curr_soprano.append(int(temp[i+7]))
        curr_mel.append(int(temp[i+8]))
        i+= 10
    times.append(curr_t)
    n4_note_dim_chords.append(curr_dim)
    n_3_note_chords.append(curr_3)
    n_fund_chords_no_double_bass.append(curr_fund)
    n_incomplete_chords.append(curr_inc)
    n_common_notes_tenor.append(curr_tenor)
    n_common_notes_alto.append(curr_alto)
    n_common_notes_soprano.append(curr_soprano)
    melodic_intervals_costs.append(curr_mel)

max_t = max([item for sublist in times for item in sublist])
max_m = max([item for sublist in melodic_intervals_costs for item in sublist])

data = [n4_note_dim_chords, n_3_note_chords, n_fund_chords_no_double_bass, n_incomplete_chords, n_common_notes_tenor, n_common_notes_alto, n_common_notes_soprano, 
        melodic_intervals_costs]
labels = ['Number of 4-note diminished chords', 'Number of 3-note chords', 'Number of fundamental chords without double bass', 'Number of incomplete chords',
          'Number of common notes in the tenor', 'Number of common notes in the alto', 'Number of common notes in the soprano', 'Melodic intervals cost']
for i in range(len(data)):
    plt.plot(times[-1], data[i][-1], label = labels[i])

    # plt.xticks(range(round(times[-1][0]), 10, 1))
    # plt.yticks(range(0, max([abs(x) for x in data[i][-1]]) + 1, 1))

    plt.xlabel('Time')
    plt.ylabel('Cost Values')
    plt.title('Evolution of the quality of solutions over time')

    plt.legend()
    plt.show()

#plot the graph
# plt.scatter(times[-1], n4_note_dim_chords[-1], label = 'Number of 4-note diminished chords')
# plt.scatter(times[-1], n_3_note_chords[-1], label = 'Number of 3-note chords')
# plt.scatter(times[-1], n_fund_chords_no_double_bass[-1], label = 'Number of fundamental chords without double bass')
# plt.scatter(times[-1], n_incomplete_chords[-1], label = 'Number of incomplete chords')
# plt.scatter(times[-1], n_common_notes_tenor[-1], label = 'Number of common notes in the tenor')
# plt.scatter(times[-1], n_common_notes_alto[-1], label = 'Number of common notes in the alto')
# plt.scatter(times[-1], n_common_notes_soprano[-1], label = 'Number of common notes in the soprano')
# plt.scatter(times[-1], melodic_intervals_costs[-1], label = f'Melodic intervals cost')
# # for i in range(len(times)):
# #     plt.scatter(times[i], melodic_intervals_costs[i], label = f'Case {i+1}')
# #     plt.scatter(times[i][-1], melodic_intervals_costs[i][-1], color = 'black', marker = 'x', label = 'Melodic interval costs')

# plt.xticks(range(round(times[-1][0]), 10, 1))
# plt.yticks(range(0, max_m, 10))

# plt.xlabel('Time')
# plt.ylabel('Cost Values')
# plt.title('Evolution of the quality of solutions over time')

# plt.legend()
# plt.show()