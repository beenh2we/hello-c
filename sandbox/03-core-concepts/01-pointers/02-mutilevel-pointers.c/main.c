#include <stdio.h>

int main() {
    printf("\n--------------- MULTI-LEVEL POINTERS & POINTER ARRAYS ---------------\n\n");
    
    /*** 1. POINTER TO POINTER (DOUBLE POINTERS) ***/
    printf("===== POINTER TO POINTER =====\n");
    
    int num = 1988;
    printf("num: %d\n", num);
    printf("&num (address of num): %p\n", &num);
    
    printf("\nSingle-level pointer:\n");
    int *p = &num;
    printf("p (value of p -> address of num): %p\n", p);
    printf("*p (dereferencing -> value of num): %d\n", *p);
    printf("&p (address of p itself): %p\n", &p);
    
    printf("\nDouble-level pointer:\n");
    int **pp = &p;
    printf("pp (value of pp -> address of p): %p\n", pp);
    printf("*pp (first dereference -> value of p -> address of num): %p\n", *pp);
    printf("**pp (second dereference -> value of num): %d\n", **pp);
    
    /*
     * Memory visualization:
     *         +------+          +------+          +------+
     *  pp --->| &p   |    p --->| &num |   num-->| 1988 |
     *         +------+          +------+          +------+
     *            ^                 ^                 ^
     *            |                 |                 |
     *         **pp-->*pp---------->*p--------------->
     */
    
    /*** 2. ARRAY OF POINTERS ***/
    printf("\n===== ARRAY OF POINTERS =====\n");
    
    int a = 10, b = 20, c = 30;
    int *p_arr[] = {&a, &b, &c};  // Array of 3 int pointers
    
    printf("Original variables:\n");
    printf("a = %d at address %p\n", a, &a);
    printf("b = %d at address %p\n", b, &b);
    printf("c = %d at address %p\n", c, &c);
    
    printf("\nArray of pointers p_arr at address %p\n\n", p_arr);
    
    printf("Accessing first element:\n");
    printf("p_arr[0]: %p (address of a)\n", p_arr[0]);
    printf("*p_arr: %p (same as above, using pointer arithmetic)\n", *p_arr);
    printf("*p_arr[0] or **p_arr: %d (value of a)\n\n", **p_arr);
    
    printf("Accessing second element:\n");
    printf("p_arr[1]: %p (address of b)\n", p_arr[1]);
    printf("*(p_arr + 1): %p (same as above, using pointer arithmetic)\n", *(p_arr + 1));
    printf("*p_arr[1] or *(*(p_arr + 1)): %d (value of b)\n\n", *(*(p_arr + 1)));
    
    printf("Accessing third element:\n");
    printf("p_arr[2]: %p (address of c)\n", p_arr[2]);
    printf("*(p_arr + 2): %p (same as above, using pointer arithmetic)\n", *(p_arr + 2));
    printf("*p_arr[2] or *(*(p_arr + 2)): %d (value of c)\n", *(*(p_arr + 2)));
    
    /*
     * Memory layout for array of pointers:
     *           +------+------+------+
     * p_arr --->| &a   | &b   | &c   |  Array of pointers
     *           +------+------+------+
     *              |      |      |
     *              v      v      v
     *           +----+ +----+ +----+
     *           | 10 | | 20 | | 30 |  Original variables
     *           +----+ +----+ +----+
     */
    
    printf("\n-----------------------------------------------------------------\n");
    printf("Key concepts:\n");
    printf("• Double pointer (**pp): Pointer to another pointer\n");
    printf("• Array of pointers (p_arr): Collection of pointers in contiguous memory\n");
    printf("• Both provide indirect access requiring multiple dereferences\n");
    printf("-----------------------------------------------------------------\n");
    
    return 0;
}