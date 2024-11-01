
//the code that originally worked

#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 1000
#define MAX_SUBJECTS 6
#define MAX_PREFERENCES 3 // Each student has a maximum of 3 preferred subjects

// Structure to store student data
typedef struct {
    int roll;
    char name[50];
    char preferredSubjects[MAX_PREFERENCES][20]; // List of preferred subjects with priorities
} Student;

// Structure to store subject data
typedef struct {
    char name[20];
    int availableSeats;
} Subject;

int getSubjectIndex(char *subject) {
    if (strcmp(subject, "Math") == 0) return 0;
    if (strcmp(subject, "Physics") == 0) return 1;
    if (strcmp(subject, "Chemistry") == 0) return 2;
    if (strcmp(subject, "Biology") == 0) return 3;
    if (strcmp(subject, "History") == 0) return 4;
    if (strcmp(subject, "Geography") == 0) return 5;
    return -1; // Return -1 if subject name is unrecognized
}


// Merge function to combine two halves for Merge Sort
void mergeStudents(Student students[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Student L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = students[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = students[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].roll <= R[j].roll) {
            students[k] = L[i];
            i++;
        } else {
            students[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        students[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        students[k] = R[j];
        j++;
        k++;
    }
}

// Merge Sort function
void mergeSortStudents(Student students[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortStudents(students, left, mid);
        mergeSortStudents(students, mid + 1, right);
        mergeStudents(students, left, mid, right);
    }
}


// Partition function for Quick Sort
int partitionSubjects(Subject subjects[], int low, int high) {
    int pivot = subjects[high].availableSeats;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (subjects[j].availableSeats >= pivot) { // Sorting in descending order
            i++;
            Subject temp = subjects[i];
            subjects[i] = subjects[j];
            subjects[j] = temp;
        }
    }

    Subject temp = subjects[i + 1];
    subjects[i + 1] = subjects[high];
    subjects[high] = temp;

    return (i + 1);
}

// Quick Sort function
void quickSortSubjects(Subject subjects[], int low, int high) {
    if (low < high) {
        int pi = partitionSubjects(subjects, low, high);

        quickSortSubjects(subjects, low, pi - 1);
        quickSortSubjects(subjects, pi + 1, high);
    }
}

// Function to allocate seats to students based on priority
void allocateSeats(Student students[], int n, Subject subjects[], int numSubjects) {
    for (int i = 0; i < n; i++) {
        int allocated = 0;

        // Try to allocate based on the student's priority list
        for (int k = 0; k < MAX_PREFERENCES; k++) {
            int subjectIndex = getSubjectIndex(students[i].preferredSubjects[k]);
            
            // Ensure subjectIndex is valid and seats are available
            if (subjectIndex != -1 && subjects[subjectIndex].availableSeats > 0) {
                subjects[subjectIndex].availableSeats--;
                printf("Allocated %s to %s (Roll No: %d)\n", subjects[subjectIndex].name, students[i].name, students[i].roll);
                allocated = 1;
                break;
            } else if (subjectIndex != -1) {
                printf("%s is full for %s (Roll No: %d)\n", subjects[subjectIndex].name, students[i].name, students[i].roll);
            }
        }

        // FCFS fallback in case all preferred subjects are full
        if (!allocated) {
            for (int j = 0; j < numSubjects; j++) {
                if (subjects[j].availableSeats > 0) {
                    subjects[j].availableSeats--;
                    printf("Allocated %s to %s (Roll No: %d) due to availability\n", subjects[j].name, students[i].name, students[i].roll);
                    allocated = 1;
                    break;
                }
            }
            if (!allocated) {
                printf("No seats available for %s (Roll No: %d)\n", students[i].name, students[i].roll);
            }
        }
    }
}



// Function to load students from a .dat file
int loadStudentsFromFile(Student students[], char filename[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return -1;
    }
    char buffer[100];
    int count = 0;
    // while (fscanf(file, "%d %s %s %s %s", &students[count].roll, students[count].name, students[count].preferredSubjects[0], students[count].preferredSubjects[1], students[count].preferredSubjects[2]) != EOF) {
    //     count++;
    // }
    // Example using buffered I/O for loading students
    while (fgets(buffer, sizeof(buffer), file)) {
    sscanf(buffer, "%d %s %s %s %s", &students[count].roll, students[count].name,
           students[count].preferredSubjects[0], students[count].preferredSubjects[1],
           students[count].preferredSubjects[2]);
    count++;
}


    fclose(file);
    return count;
}

int main() {
    // Define subjects and available seats
    Subject subjects[MAX_SUBJECTS] = {
        {"Math", 170},
        {"Physics", 164},
        {"Chemistry", 167},
        {"Biology", 165},
        {"History", 168},
        {"Geography", 166}
    };

    Student students[MAX_STUDENTS];
    int n;

    // Load students from a .dat file
    char filename[] = "students.dat";
    n = loadStudentsFromFile(students, filename);

    if (n == -1) {
        return 1;
    }

    printf("Loaded Student Records from %s:\n", filename);
    mergeSortStudents(students, 0, n - 1);
    quickSortSubjects(subjects, 0, MAX_SUBJECTS - 1);
    // Allocating seats to students based on their preferred subjects
    allocateSeats(students, n, subjects, MAX_SUBJECTS);

    return 0;
}