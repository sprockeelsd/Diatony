import csv

# parses a csv file and returns each line as a list where the empty columns have been removed
def parse_csv(file_path):
    result = []

    with open(file_path, 'r') as csvfile:
        csv_reader = csv.reader(csvfile)

        for row in csv_reader:
            # Remove empty columns from the row
            filtered_row = [value for value in row if value.strip()]

            # Append the filtered row to the result list
            result.append(filtered_row)
    return result[1:]


file_path = '../out/search-stats-2024-01-11_14-13-21.csv'
data = parse_csv(file_path)

# Print the result
for d in data:
    print(d)
