#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LENGTH 50

typedef struct {
    int rollNo;
    char name[NAME_LENGTH];
    float marks;
} Student;

Student students[MAX_STUDENTS];
int studentCount = 0;

void insertStudent();
void deleteStudent();
void updateStudent();
void displayStudents();

int linearSearch(int rollNo);
int binarySearch(int rollNo);
int interpolationSearch(int rollNo);

void bubbleSortByRoll();
void selectionSortByMarks();
void insertionSortByRoll();
void quickSortByMarks(int low, int high);
void mergeSortByRoll(int left, int right);

int partition(int low, int high);
void merge(int left, int mid, int right);

void filterByMarks(float threshold);

void swap(Student *a, Student *b);
void clearInputBuffer();
void pressEnterToContinue();

void menu();

int main() {
    menu();
    return 0;
}

void insertStudent() {
    if (studentCount == MAX_STUDENTS) {
        printf("Database full! Cannot insert more students.\n");
        return;
    }
    Student s;
    printf("Enter Roll Number: ");
    scanf("%d", &s.rollNo);
    clearInputBuffer();

    printf("Enter Name: ");
    fgets(s.name, NAME_LENGTH, stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    students[studentCount++] = s;
    printf("Student inserted successfully.\n");
}

void deleteStudent() {
    if (studentCount == 0) {
        printf("No students to delete.\n");
        return;
    }

    int rollNo;
    printf("Enter Roll Number to delete: ");
    scanf("%d", &rollNo);

    int index = linearSearch(rollNo);
    if (index == -1) {
        printf("Student with Roll No %d not found.\n", rollNo);
        return;
    }

    for (int i = index; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }
    studentCount--;
    printf("Student deleted successfully.\n");
}

void updateStudent() {
    if (studentCount == 0) {
        printf("No students to update.\n");
        return;
    }

    int rollNo;
    printf("Enter Roll Number to update: ");
    scanf("%d", &rollNo);
    clearInputBuffer();

    int index = linearSearch(rollNo);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("Enter new Name: ");
    fgets(students[index].name, NAME_LENGTH, stdin);
    students[index].name[strcspn(students[index].name, "\n")] = '\0';

    printf("Enter new Marks: ");
    scanf("%f", &students[index].marks);

    printf("Student record updated.\n");
}

void displayStudents() {
    if (studentCount == 0) {
        printf("No student records.\n");
        return;
    }

    printf("\n%-10s %-20s %-10s\n", "Roll No", "Name", "Marks");
    printf("------------------------------------------\n");
    for (int i = 0; i < studentCount; i++) {
        printf("%-10d %-20s %-10.2f\n", students[i].rollNo, students[i].name, students[i].marks);
    }
}

int linearSearch(int rollNo) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].rollNo == rollNo) return i;
    }
    return -1;
}

int binarySearch(int rollNo) {
    int low = 0, high = studentCount - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (students[mid].rollNo == rollNo) return mid;
        else if (students[mid].rollNo < rollNo) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int interpolationSearch(int rollNo) {
    int low = 0, high = studentCount - 1;
    while (low <= high && rollNo >= students[low].rollNo && rollNo <= students[high].rollNo) {
        if (low == high) {
            if (students[low].rollNo == rollNo) return low;
            return -1;
        }

        int pos = low + ((rollNo - students[low].rollNo) * (high - low)) / (students[high].rollNo - students[low].rollNo);

        if (students[pos].rollNo == rollNo) return pos;
        if (students[pos].rollNo < rollNo) low = pos + 1;
        else high = pos - 1;
    }
    return -1;
}

void swap(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSortByRoll() {
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            if (students[j].rollNo > students[j + 1].rollNo) {
                swap(&students[j], &students[j + 1]);
            }
        }
    }
    printf("Sorted by Roll No (Bubble Sort).\n");
}

void selectionSortByMarks() {
    for (int i = 0; i < studentCount - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < studentCount; j++) {
            if (students[j].marks > students[maxIndex].marks) maxIndex = j;
        }
        swap(&students[i], &students[maxIndex]);
    }
    printf("Sorted by Marks (Selection Sort, descending).\n");
}

void insertionSortByRoll() {
    for (int i = 1; i < studentCount; i++) {
        Student key = students[i];
        int j = i - 1;
        while (j >= 0 && students[j].rollNo > key.rollNo) {
            students[j + 1] = students[j];
            j--;
        }
        students[j + 1] = key;
    }
    printf("Sorted by Roll No (Insertion Sort).\n");
}

int partition(int low, int high) {
    float pivot = students[high].marks;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (students[j].marks > pivot) {
            i++;
            swap(&students[i], &students[j]);
        }
    }
    swap(&students[i + 1], &students[high]);
    return i + 1;
}

void quickSortByMarks(int low, int high) {
    if (low < high) {
        int pi = partition(low, high);
        quickSortByMarks(low, pi - 1);
        quickSortByMarks(pi + 1, high);
    }
}

void merge(int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    Student *L = malloc(n1 * sizeof(Student));
    Student *R = malloc(n2 * sizeof(Student));

    for (int i = 0; i < n1; i++) L[i] = students[left + i];
    for (int j = 0; j < n2; j++) R[j] = students[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].rollNo <= R[j].rollNo) students[k++] = L[i++];
        else students[k++] = R[j++];
    }
    while (i < n1) students[k++] = L[i++];
    while (j < n2) students[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSortByRoll(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortByRoll(left, mid);
        mergeSortByRoll(mid + 1, right);
        merge(left, mid, right);
    }
}

void filterByMarks(float threshold) {
    int found = 0;
    printf("\nStudents scoring above %.2f:\n", threshold);
    printf("%-10s %-20s %-10s\n", "Roll No", "Name", "Marks");
    printf("------------------------------------------\n");
    for (int i = 0; i < studentCount; i++) {
        if (students[i].marks > threshold) {
            printf("%-10d %-20s %-10.2f\n", students[i].rollNo, students[i].name, students[i].marks);
            found = 1;
        }
    }
    if (!found) printf("No students found with marks above %.2f.\n", threshold);
}

void clearInputBuffer() {
    while (getchar() != '\n');
}

void pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    getchar();
}

void menu() {
    int choice, rollNo, index;
    float threshold;

    while (1) {
        printf("\n==== Student Record Manager ====\n");
        printf("1. Insert Student\n2. Delete Student\n3. Update Student\n4. Display All Students\n");
        printf("5. Search Student (Linear)\n6. Search Student (Binary) - Sorted by Roll No\n7. Search Student (Interpolation) - Sorted by Roll No\n");
        printf("8. Sort (Bubble Sort by Roll No)\n9. Sort (Selection Sort by Marks)\n10. Sort (Insertion Sort by Roll No)\n");
        printf("11. Sort (Quick Sort by Marks)\n12. Sort (Merge Sort by Roll No)\n13. Filter by Marks Threshold\n0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: insertStudent(); break;
            case 2: deleteStudent(); break;
            case 3: updateStudent(); break;
            case 4: displayStudents(); break;
            case 5:
                printf("Enter Roll Number: ");
                scanf("%d", &rollNo);
                index = linearSearch(rollNo);
                if (index != -1) {
                    printf("Found: Roll No: %d, Name: %s, Marks: %.2f\n", students[index].rollNo, students[index].name, students[index].marks);
                } else printf("Student not found.\n");
                break;
            case 6:
                insertionSortByRoll();
                printf("Enter Roll Number: ");
                scanf("%d", &rollNo);
                index = binarySearch(rollNo);
                if (index != -1) {
                    printf("Found: Roll No: %d, Name: %s, Marks: %.2f\n", students[index].rollNo, students[index].name, students[index].marks);
                } else printf("Student not found.\n");
                break;
            case 7:
                insertionSortByRoll();
                printf("Enter Roll Number: ");
                scanf("%d", &rollNo);
                index = interpolationSearch(rollNo);
                if (index != -1) {
                    printf("Found: Roll No: %d, Name: %s, Marks: %.2f\n", students[index].rollNo, students[index].name, students[index].marks);
                } else printf("Student not found.\n");
                break;
            case 8: bubbleSortByRoll(); break;
            case 9: selectionSortByMarks(); break;
            case 10: insertionSortByRoll(); break;
            case 11:
                quickSortByMarks(0, studentCount - 1);
                printf("Sorted by Marks (Quick Sort).\n");
                break;
            case 12:
                mergeSortByRoll(0, studentCount - 1);
                printf("Sorted by Roll No (Merge Sort).\n");
                break;
            case 13:
                printf("Enter marks threshold: ");
                scanf("%f", &threshold);
                filterByMarks(threshold);
                break;
            case 0:
                printf("Exiting. Goodbye!\n");
                return;
            default:
                printf("Invalid choice! Try again.\n");
        }
        pressEnterToContinue();
    }
}
