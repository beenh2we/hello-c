#include <stdio.h>
#include <string.h>

typedef struct
{
    int id;
    char name[30];
    float salary;
} Employee;

void printEmployee(Employee *emp);

int main(int argc, char *argv[])
{
    FILE *fp;
    Employee employees[3] = {{1001, "John Smith", 45000.01},
                             {1002, "Lisa Wong", 25000.51},
                             {1003, "Michael Brown", 47601.34}};
    Employee read_emp;

    fp = fopen("employees.dat", "wb");
    if (fp == NULL)
    {
        perror("Error opening file for writing ");
        return 1;
    }

    for (int i = 0; i < 3; i++)
    {
        if (fwrite(&employees[i], sizeof(Employee), 1, fp) != 1)
        {
            perror("Error writing employee data");
        }
    }

    fclose(fp);

    fp = fopen("employees.dat", "rb");
    if (fp == NULL)
    {
        perror("Error opening file for reading ");
        return 1;
    }

    printf("Employee records from file:\n");
    while (fread(&read_emp, sizeof(Employee), 1, fp) == 1)
    {
        printEmployee(&read_emp);
    }

    fclose(fp);
    return 0;
}

void printEmployee(Employee *emp)
{
    printf("ID: %d, Name: %s, Salary: %.2f\n", emp->id, emp->name, emp->salary);
}
