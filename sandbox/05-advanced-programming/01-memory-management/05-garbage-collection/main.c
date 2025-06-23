#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ===== Reference Counting Example =====

// Object with reference count
typedef struct RefCountedObject {
    int ref_count;
    char *data;
} RefCountedObject;

// Create a new reference counted object
RefCountedObject* rc_create(const char *data) {
    RefCountedObject *obj = (RefCountedObject*)malloc(sizeof(RefCountedObject));
    if (!obj) return NULL;
    
    obj->data = strdup(data);
    if (!obj->data) {
        free(obj);
        return NULL;
    }
    
    obj->ref_count = 1;  // Initial reference count is 1
    printf("Created object '%s' with ref count %d\n", obj->data, obj->ref_count);
    return obj;
}

// Add a reference
void rc_add_ref(RefCountedObject *obj) {
    if (obj) {
        obj->ref_count++;
        printf("Increased ref count for '%s' to %d\n", obj->data, obj->ref_count);
    }
}

// Release a reference
void rc_release(RefCountedObject *obj) {
    if (!obj) return;
    
    obj->ref_count--;
    printf("Decreased ref count for '%s' to %d\n", obj->data, obj->ref_count);
    
    if (obj->ref_count == 0) {
        printf("Destroying object '%s'\n", obj->data);
        free(obj->data);
        free(obj);
    }
}

// ===== Mark and Sweep Example =====

// Simple object for mark-sweep demonstration
typedef struct Object {
    int id;
    char *name;
    struct Object **references;  // Objects this object refers to
    int ref_count;               // Number of references
    bool marked;                 // Used during mark phase
} Object;

// Array to hold all objects for demonstration
#define MAX_OBJECTS 100
Object *all_objects[MAX_OBJECTS];
int object_count = 0;

// Root objects are those directly accessible from the program
#define MAX_ROOTS 10
Object *root_objects[MAX_ROOTS];
int root_count = 0;

// Create an object for mark-sweep
Object* create_object(int id, const char *name) {
    if (object_count >= MAX_OBJECTS) return NULL;
    
    Object *obj = (Object*)malloc(sizeof(Object));
    if (!obj) return NULL;
    
    obj->id = id;
    obj->name = strdup(name);
    obj->references = (Object**)malloc(MAX_OBJECTS * sizeof(Object*));
    obj->ref_count = 0;
    obj->marked = false;
    
    all_objects[object_count++] = obj;
    printf("Created object %d: %s\n", id, name);
    return obj;
}

// Add a reference from one object to another
void add_reference(Object *from, Object *to) {
    if (!from || !to || from->ref_count >= MAX_OBJECTS) return;
    
    from->references[from->ref_count++] = to;
    printf("Added reference from '%s' to '%s'\n", from->name, to->name);
}

// Add an object to the root set
void add_root(Object *obj) {
    if (!obj || root_count >= MAX_ROOTS) return;
    
    root_objects[root_count++] = obj;
    printf("Added '%s' to root set\n", obj->name);
}

// Mark phase of mark-sweep algorithm
void mark_object(Object *obj) {
    if (!obj || obj->marked) return;
    
    obj->marked = true;
    printf("Marked object %d: %s\n", obj->id, obj->name);
    
    // Mark all referenced objects recursively
    for (int i = 0; i < obj->ref_count; i++) {
        mark_object(obj->references[i]);
    }
}

// Mark phase starting from roots
void mark_phase() {
    printf("\n--- Mark Phase ---\n");
    
    // Clear all marks
    for (int i = 0; i < object_count; i++) {
        all_objects[i]->marked = false;
    }
    
    // Mark from roots
    for (int i = 0; i < root_count; i++) {
        mark_object(root_objects[i]);
    }
}

// Sweep phase - free unmarked objects
void sweep_phase() {
    printf("\n--- Sweep Phase ---\n");
    
    int i = 0;
    while (i < object_count) {
        if (!all_objects[i]->marked) {
            printf("Sweeping (freeing) unmarked object %d: %s\n", all_objects[i]->id, all_objects[i]->name);
            
            // Free the object
            free(all_objects[i]->name);
            free(all_objects[i]->references);
            free(all_objects[i]);
            
            // Remove from array by replacing with last object
            all_objects[i] = all_objects[--object_count];
            all_objects[object_count] = NULL;
        } else {
            i++;
        }
    }
}

// Run mark and sweep garbage collection
void run_gc() {
    printf("\n=== Running Garbage Collection ===\n");
    mark_phase();
    sweep_phase();
    printf("Garbage collection complete. Remaining objects: %d\n", object_count);
}

// Clean up all objects
void cleanup_objects() {
    for (int i = 0; i < object_count; i++) {
        free(all_objects[i]->name);
        free(all_objects[i]->references);
        free(all_objects[i]);
    }
    object_count = 0;
    root_count = 0;
}

// Function to explain garbage collection concepts
void explain_gc_concepts() {
    printf("\n=== Garbage Collection Concepts ===\n");
    
    printf("1. Reference Counting\n");
    printf("   - Each object tracks how many references point to it\n");
    printf("   - Object is freed when reference count reaches zero\n");
    printf("   - Pros: Simple, incremental, immediate cleanup\n");
    printf("   - Cons: Can't handle circular references\n\n");
    
    printf("2. Mark and Sweep\n");
    printf("   - Two phases: Mark (trace from roots) and Sweep (free unmarked)\n");
    printf("   - Pros: Handles circular references, complete collection\n");
    printf("   - Cons: Pause during collection, memory fragmentation\n\n");
    
    printf("3. Copying Collection\n");
    printf("   - Memory divided into two regions\n");
    printf("   - Live objects copied from 'from-space' to 'to-space'\n");
    printf("   - Pros: No fragmentation, fast allocation\n");
    printf("   - Cons: Requires twice the memory, copying overhead\n\n");
    
    printf("4. Generational Collection\n");
    printf("   - Objects divided by age (young and old generations)\n");
    printf("   - Young generation collected more frequently\n");
    printf("   - Pros: Better performance, most objects die young\n");
    printf("   - Cons: More complex implementation\n\n");
    
    printf("5. Incremental and Concurrent Collection\n");
    printf("   - Collection work divided into small increments\n");
    printf("   - Can run concurrently with program execution\n");
    printf("   - Pros: Reduced pause times, better responsiveness\n");
    printf("   - Cons: Complex synchronization, overhead\n\n");
}

int main() {
    printf("==== GARBAGE COLLECTION CONCEPTS ====\n\n");
    
    // Reference counting example
    printf("=== Reference Counting Example ===\n");
    
    RefCountedObject *str1 = rc_create("Hello, world!");
    RefCountedObject *str2 = str1;  // Another reference to the same object
    rc_add_ref(str2);               // Increment reference count
    
    RefCountedObject *str3 = rc_create("Another string");
    
    printf("\nReleasing references...\n");
    rc_release(str1);  // Decrease count but not free yet
    rc_release(str3);  // This will be freed (ref count = 0)
    rc_release(str2);  // Now this will be freed (ref count = 0)
    
    printf("\n=== Mark-Sweep Example ===\n");
    
    // Create objects
    Object *obj1 = create_object(1, "Object 1");
    Object *obj2 = create_object(2, "Object 2");
    Object *obj3 = create_object(3, "Object 3");
    Object *obj4 = create_object(4, "Object 4");
    Object *obj5 = create_object(5, "Object 5");
    
    // Create object graph
    add_reference(obj1, obj2);
    add_reference(obj1, obj3);
    add_reference(obj2, obj4);
    add_reference(obj3, obj4);
    add_reference(obj4, obj5);
    
    // Set root objects (directly accessible)
    add_root(obj1);
    
    printf("\nObject graph created. Running garbage collection...\n");
    run_gc();
    
    printf("\nRemoving root reference to obj1 and adding reference to obj5...\n");
    root_count = 0;
    add_root(obj5);
    
    printf("\nRunning garbage collection again...\n");
    run_gc();
    
    // Clean up
    cleanup_objects();
    
    // Explain GC concepts
    explain_gc_concepts();
    
    return 0;
}