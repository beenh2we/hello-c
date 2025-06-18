#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[30];
    float salary;
} Employee;

void printEmployee(Employee *emp);

int main(int argc, char *argv[]) {
    FILE *fp;
    Employee employees[3] = {
        {1001, "John Smith", 45000.01},
        {1002, "Lisa Wong", 25000.51},
        {1003, "Michael Brown", 47601.34}
    };
    Employee temp;

    fp = fopen("random_access.dat", "wb+");
    if (fp == NULL) {
        perror("Error opening file for writing ");
        return 1;
    }

    fwrite(employees, sizeof(Employee), 3, fp);

    // Random access - read second record
    fseek(fp, sizeof(Employee), SEEK_SET); // Position at second record
    fread(&temp, sizeof(Employee), 1, fp);
    printf("Second employee: ID: %d, Name: %s, Salary: %.2f\n", 
        temp.id, temp.name, temp.salary);

    // Modify record and write it back
    strcpy(temp.name, "Lisa Wang");
    temp.salary = 11111.11;

    // Position back to second record and write
    fseek(fp, sizeof(Employee), SEEK_SET);
    fwrite(&temp, sizeof(Employee), 1, fp);

    // Read all records to verify changes
    rewind(fp);
    printf("\nAll records after modification:\n");

    for (int i = 0; i < 3; i++) {
        fread(&temp, sizeof(Employee), 1, fp);
        printf("Employee %d: ID: %d, Name: %s, Salary: %.2f\n",
            i+1, temp.id, temp.name, temp.salary);
    }

    fclose(fp);
    return 0;
}

void printEmployee(Employee *emp) {
    printf("ID: %d, Name: %s, Salary: %.2f\n", emp -> id, emp -> name, emp -> salary);
}