import sys
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
    print("Wrong number of arguments")
    exit()

filename = sys.argv[1]

test_cases_strings = ["I5-V5-I5-V65-I6-II6-V64-V7+-I5",
                      "I5-IV5-VII6-I6-III5-VI5-II6-V7+-I5",
                      "I5-V6-VI5-V5-IV5-I6-II5-V5-I5-V6-VI5-V5-IV5-V7+-I5",
                      "I5-V5-VI5-I5-III5-VI5-II5-I5-V5",
                      "I5-V5-VI5-I5-VI5-VII6-III5-VI5-II6-V7+-I5",
                      "I5-II6-IV5-V+6-I6-III5-VI5-V65/-I5-IV5-I64-V7+-I5"]

data = []
#read file
with open(filename, 'r') as csvfile:
    for line in csvfile.readlines():
        #for each line, separate each element and return a list of lists
        line_data = [elem.strip() for elem in line.split(',')]
        #print(line_data,'\n')
        data.append(line_data)

#remove first line with titles
test_cases = data[1:]

#isolate the data we want to plot
chord_progressions = []
tonality = []
opt_sol_time = []
times = []
n4_note_dim_chords = []
n_3_note_chords = []
n_incomplete_chords = []
n_common_notes_same_voice = []
melodic_intervals_costs = []

for case in test_cases:
    #initialize lists for each case
    curr_t = []
    curr_dim = []
    curr_3 = []
    curr_inc = []
    curr_same = []
    curr_mel = []

    #data specific for the test case
    chord_progressions.append(case[0])
    tonality.append(case[1])
    optimal_sol_time = float(case[3])
    opt_sol_time.append(optimal_sol_time)
    opt_sol_inc = int(case[13])
    opt_sol_dim = int(case[14])
    opt_sol_3 = int(case[15])
    opt_sol_mel = int(case[16])
    opt_sol_same = int(case[17])

    # start at 30 to get each successive found solution
    temp = case[27:]
    i = 0
    while i + 7 < len(temp) and temp[i] != '':
        curr_t.append(float(temp[i]))
        curr_inc.append(int(temp[i+1]))
        curr_dim.append(int(temp[i+2]))
        curr_3.append(int(temp[i+3]))
        curr_mel.append(int(temp[i+4]))
        curr_same.append(-int(temp[i+5]))
        i+= 7 # move to the next solution
        
    #gather the information for this test case
    curr_t.append(optimal_sol_time)
    times.append(curr_t)

    curr_dim.append(opt_sol_dim)
    n4_note_dim_chords.append(curr_dim)

    curr_3.append(opt_sol_3)
    n_3_note_chords.append(curr_3)

    curr_inc.append(opt_sol_inc)
    n_incomplete_chords.append(curr_inc)

    curr_same.append(-opt_sol_same)
    n_common_notes_same_voice.append(curr_same)

    curr_mel.append(opt_sol_mel)
    melodic_intervals_costs.append(curr_mel)

min_t = min([item for sublist in times for item in sublist])
max_t = max([item for sublist in times for item in sublist])

costs = [n_incomplete_chords, n4_note_dim_chords, n_3_note_chords, melodic_intervals_costs, n_common_notes_same_voice]
costs_labels = ['Number of incomplete chords', 'Number of 4-note diminished chords', 'Number of 3-note chords', 
                'Melodic intervals cost', 'Number of common notes in the same voice']

# Plot cost evolution over time for each cost
for i in range(len(costs)):
    # for each test case
    for j in range(len(chord_progressions)):
        a = 0
        #plot the cost graph for the test case with the appropriate label
        plt.plot(times[j], costs[i][j], label = f'{chord_progressions[j]} - {tonality[j]}')

    plt.xlabel('Time (s)', fontsize=30)
    plt.ylabel(costs_labels[i], fontsize=30)
    plt.title(f'Evolution of the {costs_labels[i]} over time', fontsize=30)

    #plt.legend()
    plt.show()

# plot time required by instance to be solved (tonality and chord progression)
test_cases_literals = [str(test_cases_strings.index(cs)) + " " + t for cs, t in zip(chord_progressions, tonality)]
plt.bar(test_cases_literals, opt_sol_time)
plt.xlabel('Instance name', fontsize = 15)
plt.xticks(rotation = 90)
plt.ylabel('Time taken to find optimal solution', fontsize = 15)
plt.title('Time taken to find optimal solution for each instance')
plt.show()