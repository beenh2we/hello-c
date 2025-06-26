#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 0. sum_n (n + n-1 + n-2 + ... + 2 + 1)
unsigned int sum_n(int n)
{
    if (n == 1) return 1;
    return n + sum_n(n - 1);
};

// 1. count_down
void count_down(int n)
{
    if (n == 0)
    {
        printf("0!\n");
        return;
    };

    printf("%d... \n", n);

    return count_down(n - 1);
}

// 2. sum_array
unsigned int sum_array(int *arr, size_t length)
{
    if (length == 1) return *arr;

    return *arr + sum_array(arr + 1, length - 1);
}

// 3. factorial
unsigned long long factorial(unsigned int n)
{
    if (n == 0 || n == 1)
    {
        return 1;
    }
    return n * factorial(n - 1);
}

// 4. reverse_string
void reverse_helper(char *str, int left, int right)
{
    if (left >= right) return;

    char temp = str[left];
    str[left] = str[right];
    str[right] = temp;

    reverse_helper(str, left + 1, right - 1);
}

char *reverse_string(const char *s)
{
    if (s == NULL) return NULL;

    int length = strlen(s);

    char *result = (char *) malloc(length + 1);
    strcpy(result, s);

    if (length > 1)
    {
        reverse_helper(result, 0, length - 1);
    }

    return result;
}

// 5. climb_stairs (fibonacci)
/**
 * Imagine you have a staircase with n steps.
 * Each time you can climb either 1 or 2 steps.
 * How many different ways can you climb to the top of the staircase?
 */
unsigned long climb_stairs(int n)
{
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    return climb_stairs(n - 1) + climb_stairs(n - 2);
}

int main(int argc, char *argv[])
{
    printf("Sum: 1 + ... + 5: %d\n", sum_n(5));

    printf("\nCount Down: \n");
    count_down(5);

    int nums[] = {3, 4, 5, 6, 10};
    printf("\nSum array: %d\n",
           sum_array(nums, sizeof(nums) / sizeof(nums[0])));

    printf("Factorial of 5: %llu\n", factorial(5));

    char str[] = "HELLO";
    char *reversed_str = reverse_string(str);
    printf("\nRevers String: %s\n", reversed_str);

    free(reversed_str);

    printf("\nClimb 10 Stairs: %lu\n", climb_stairs(10));
    return 0;
}
