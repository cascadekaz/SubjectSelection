import random

# Subject list to randomize preferences
subjects = ["Math", "Physics", "Chemistry", "Biology", "History", "Geography"]

# Read names from the names.txt file
with open("names.txt", "r") as name_file:
    names = [line.strip() for line in name_file.readlines()]

# Check if there are enough names
if len(names) < 1000:
    raise ValueError("There are less than 1000 names in names.txt")

# Open the file to write the student data
with open("students.dat", "w") as file:
    for roll in range(1, 1001):
        # Get the corresponding name from the list
        name = names[roll - 1]
        
        # Randomly choose 3 unique subjects
        preferences = random.sample(subjects, 3)
        
        # Write the roll, name, and subject preferences to the file
        file.write(f"{roll} {name} {preferences[0]} {preferences[1]} {preferences[2]}\n")

print("Student data has been written to students.dat.")

