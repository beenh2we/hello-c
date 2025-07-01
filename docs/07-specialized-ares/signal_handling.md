# Unix/Linux Signal Handling Explained in Detail

## Table of Contents

1. [Signal Basic Concepts](#signal-basic-concepts)
2. [Common Signal Types](#common-signal-types)
3. [Basic Signal Handling](#basic-signal-handling)
4. [Advanced Signal Handling](#advanced-signal-handling)
5. [Signal Masking Mechanism](#signal-masking-mechanism)
6. [Signal Handling Pitfalls and Best Practices](#signal-handling-pitfalls-and-best-practices)
7. [Practical Application Scenarios](#practical-application-scenarios)

## Signal Basic Concepts

Signals are a simple mechanism for inter-process communication in Unix/Linux systems, which can be viewed as software interrupts sent to processes. Signals are typically used to notify a process that a certain event or exception has occurred.

**Key characteristics of signals**:

- Asynchronous - signals can arrive at any time
- Simple - only the signal type is transmitted, usually without additional data
- Unreliable - traditional Unix signals may be lost
- Limited - the system defines a finite number of signal types

**Signal sources**:

- Kernel (such as detecting memory access errors)
- Users (such as pressing Ctrl+C)
- Other processes (through the kill command or functions)
- The process itself (using the raise function)

## Common Signal Types

Common signals in Unix/Linux systems:

| Signal Name | Value | Default Behavior | Cause                                                  |
| ----------- | ----- | ---------------- | ------------------------------------------------------ |
| SIGINT      | 2     | Terminate        | Terminal interrupt key (typically Ctrl+C)              |
| SIGQUIT     | 3     | Terminate+core   | Terminal quit key (typically Ctrl+\\)                  |
| SIGKILL     | 9     | Terminate        | Force terminate a process, cannot be caught or ignored |
| SIGSEGV     | 11    | Terminate+core   | Segmentation fault (invalid memory reference)          |
| SIGTERM     | 15    | Terminate        | Terminate program (default signal for kill command)    |
| SIGUSR1     | 10    | Terminate        | User-defined signal 1                                  |
| SIGUSR2     | 12    | Terminate        | User-defined signal 2                                  |

## Basic Signal Handling

### Using the signal() Function

The simplest way to handle signals is to register a handler function using `signal()`:

```c
#include <signal.h>

void sigint_handler(int signum) {
    printf("Received signal %d (SIGINT)\n", signum);
}

int main() {
    // Register SIGINT signal handler
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Failed to register handler");
        return 1;
    }

    // ... main program body ...
}
```

In this example, the `sigint_handler()` function will be called when the program receives a SIGINT signal (typically when the user presses Ctrl+C).

### Implementing Graceful Exit

The following example shows how to implement a graceful exit using signals:

```c
volatile sig_atomic_t termination_requested = 0;

void sigint_handler(int signum) {
    sigint_count++;

    // Exit after pressing Ctrl+C three times
    if (sigint_count >= 3) {
        termination_requested = 1;
    }

    printf("\nPress Ctrl+C %d more times to exit.\n", 3 - sigint_count);
}

void basic_signal_demo() {
    signal(SIGINT, sigint_handler);

    while (!termination_requested) {
        // Normal operation...
        sleep(1);
    }

    printf("\nShutting down gracefully...\n");
    // Perform cleanup operations...
}
```

### Important Concept: sig_atomic_t

`sig_atomic_t` is an integer type that guarantees atomic read and write operations in signal handlers. Variables modified in signal handlers should use this type and be declared as `volatile`:

```c
volatile sig_atomic_t termination_requested = 0;
```

## Advanced Signal Handling

### Using the sigaction() Function

`sigaction()` is more powerful and reliable than `signal()`, providing more fine-grained control:

```c
void sigaction_info_handler(int signum, siginfo_t *info, void *context) {
    printf("Received signal %d from PID %d\n", signum, info->si_pid);
}

void setup_handler() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    // Use extended information handler function
    sa.sa_sigaction = sigaction_info_handler;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;

    // Block SIGTERM while the handler is executing
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGTERM);

    // Register the handler
    sigaction(SIGUSR1, &sa, NULL);
}
```

### Important Flags

The `sigaction` structure supports various flags, including:

- `SA_SIGINFO`: Use an extended signal handler function that receives additional information
- `SA_RESTART`: System calls interrupted by signals will automatically restart
- `SA_NODEFER`: Don't block the same signal during handler execution
- `SA_RESETHAND`: Reset the signal behavior to default after handler execution

### Receiving Detailed Signal Information

Use the `siginfo_t` structure to get more details about signals:

```c
void sigaction_info_handler(int signum, siginfo_t *info, void *context) {
    printf("Received signal %d from PID %d\n", signum, info->si_pid);

    if (info->si_code == SI_USER) {
        printf("Signal sent by kill()\n");
    } else if (info->si_code == SI_KERNEL) {
        printf("Signal sent by the kernel\n");
    }

    // SIGFPE specific information
    if (signum == SIGFPE) {
        switch (info->si_code) {
            case FPE_INTDIV: printf("Integer divide by zero\n"); break;
            case FPE_INTOVF: printf("Integer overflow\n"); break;
            // Other error types...
        }
    }
}
```

## Signal Masking Mechanism

### Why Signal Masking is Needed

Signal masking allows programs to temporarily block the delivery of certain signals, which is useful in the following situations:

- Protecting critical code sections from signal interruption
- Preventing specific signals from arriving at inappropriate times
- Implementing synchronized signal handling logic

### Setting Signal Masks

```c
void signal_mask_demo() {
    // Create signal set
    sigset_t mask, old_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    // Block SIGUSR1
    printf("Blocking SIGUSR1 for 5 seconds...\n");
    sigprocmask(SIG_BLOCK, &mask, &old_mask);

    // During this period, SIGUSR1 signals will be pending
    sleep(5);

    // Check if there are pending signals
    sigset_t pending;
    sigpending(&pending);
    if (sigismember(&pending, SIGUSR1)) {
        printf("SIGUSR1 is pending!\n");
    }

    // Unblock, allowing pending signals to be delivered
    printf("Unblocking SIGUSR1...\n");
    sigprocmask(SIG_SETMASK, &old_mask, NULL);
}
```

### Signal Set Operations

Main functions for handling signal sets:

- `sigemptyset()` - Create an empty signal set
- `sigfillset()` - Create a set containing all signals
- `sigaddset()` - Add a signal to a set
- `sigdelset()` - Remove a signal from a set
- `sigismember()` - Check if a signal is in a set

## Signal Handling Pitfalls and Best Practices

### Unsafe Practices Example

The following example shows practices that should be avoided in signal handling:

```c
void unsafe_signal_handler(int signum) {
    // Unsafe: Reentrancy problem
    if (in_signal_handler) {
        printf("WARNING: Signal received while already handling a signal!\n");
        return;
    }

    in_signal_handler = 1;

    // Unsafe: Using printf in a signal handler
    printf("Handling signal %d...\n", signum);

    // Unsafe: Performing time-consuming operations in a signal handler
    sleep(3);

    in_signal_handler = 0;
}
```

### Safe Practices Example

Correct signal handling approach:

```c
volatile sig_atomic_t cleanup_needed = 0;

void cleanup_handler(int signum) {
    // Only set a flag, don't perform actual cleanup
    cleanup_needed = 1;
}

int main() {
    signal(SIGINT, cleanup_handler);
    signal(SIGTERM, cleanup_handler);

    while (!cleanup_needed) {
        // Normal operation...
        sleep(1);
    }

    // Perform actual cleanup in the main program, not in the signal handler
    printf("Performing cleanup...\n");
    // Close files, free resources, save state, etc.

    return 0;
}
```

### Best Practices for Signal Handling

1. **Keep it simple**: Signal handler functions should be as simple as possible
2. **Use sig_atomic_t**: Use `volatile sig_atomic_t` for variables modified in signal handlers
3. **Use signal-safe functions**: Only use async-signal-safe functions in signal handlers
4. **Set flags**: Set flags in signal handlers and perform the actual work in the main program
5. **Use sigaction**: Prefer `sigaction()` over `signal()`
6. **Block signals**: Block signals during critical code sections
7. **Verify return values**: Always check the return values of signal-related functions

### Async-Signal-Safe Functions

Here are some functions that can be safely used in signal handlers:

- `_exit()`
- `write()` (for small data blocks)
- `sigaction()`, `signal()`
- `alarm()`, `sleep()`
- `kill()`, `raise()`

Unsafe functions include but are not limited to: `printf()`, `malloc()`, `free()`, and most library functions.

## Practical Application Scenarios

### 1. Graceful Shutdown

Safely terminate a program when receiving SIGINT or SIGTERM:

```c
volatile sig_atomic_t shutdown_requested = 0;

void termination_handler(int sig) {
    shutdown_requested = 1;
}

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = termination_handler;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    while (!shutdown_requested) {
        // Main program logic
    }

    // Clean up resources and exit gracefully
    return 0;
}
```

### 2. Configuration Reload

Trigger configuration reload with SIGHUP signal:

```c
volatile sig_atomic_t reload_config = 0;

void sighup_handler(int sig) {
    reload_config = 1;
}

int main() {
    signal(SIGHUP, sighup_handler);

    while (1) {
        if (reload_config) {
            reload_config = 0;
            // Reload configuration...
            printf("Configuration reloaded\n");
        }

        // Normal processing...
        sleep(1);
    }

    return 0;
}
```

### 3. Timeout Handling

Use SIGALRM to implement operation timeouts:

```c
void timeout_handler(int sig) {
    printf("Operation timed out!\n");
    // Set a flag or take other action
}

void perform_with_timeout(void (*operation)(), int seconds) {
    signal(SIGALRM, timeout_handler);
    alarm(seconds);

    operation();  // Perform potentially time-consuming operation

    alarm(0);  // Cancel the alarm
}
```

## Summary

Signal handling is a fundamental skill in Unix/Linux programming, and proper implementation can significantly improve program robustness. Understanding the signal mechanism and mastering safe handling techniques will enable your programs to behave more elegantly and reliably in exceptional situations.

Key points:

- Keep signal handler functions simple and safe
- Only set flags in signal handlers; perform actual work in the main program
- Use `sigaction()` for more control and information
- Block signals appropriately during critical code sections
- Always follow best practices for signal handling
