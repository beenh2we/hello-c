#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>

// Flag for graceful shutdown
volatile sig_atomic_t termination_requested = 0;

// Counter for signal occurrences
volatile sig_atomic_t sigint_count = 0;
volatile sig_atomic_t sigusr1_count = 0;
volatile sig_atomic_t sigusr2_count = 0;

// Flag to demonstrate signal re-entrancy issues
volatile int in_signal_handler = 0;

// Simple signal handler for SIGINT
void sigint_handler(int signum) {
    sigint_count++;
    
    // This is signal-safe - only modifies sig_atomic_t variables
    if (sigint_count >= 3) {
        termination_requested = 1;
    }
    
    // UNSAFE: printf is not async-signal-safe
    // Only used for demonstration purposes
    printf("\nSIGINT received (%d/3). ", sigint_count);
    if (termination_requested) {
        printf("Preparing for shutdown...\n");
    } else {
        printf("Press Ctrl+C %d more times to exit.\n", 3 - sigint_count);
    }
}

// Handler that demonstrates unsafe signal handling
void unsafe_signal_handler(int signum) {
    // Demonstrate re-entrancy problems
    if (in_signal_handler) {
        // We're already in the signal handler - this could cause problems!
        printf("WARNING: Signal %d received while already handling a signal!\n", signum);
        return;
    }
    
    in_signal_handler = 1;
    
    printf("\nHandling signal %d...\n", signum);
    
    // UNSAFE: sleep is not async-signal-safe
    // Only used for demonstration purposes
    printf("Performing slow operation in signal handler (BAD PRACTICE)...\n");
    sleep(3);
    
    printf("Slow operation completed.\n");
    
    in_signal_handler = 0;
}

// Handler for USR1 and USR2 signals
void usr_signal_handler(int signum) {
    if (signum == SIGUSR1) {
        sigusr1_count++;
    } else if (signum == SIGUSR2) {
        sigusr2_count++;
    }
    
    // UNSAFE: printf is not async-signal-safe
    // Only used for demonstration purposes
    printf("\nReceived signal %d (USR%d), count: %d\n", 
           signum, signum == SIGUSR1 ? 1 : 2,
           signum == SIGUSR1 ? sigusr1_count : sigusr2_count);
}

// Handler that uses sigaction with information
void sigaction_info_handler(int signum, siginfo_t *info, void *context) {
    // Get information about the signal
    printf("\nReceived signal %d from PID %d\n", signum, info->si_pid);
    
    if (info->si_code == SI_USER) {
        printf("Signal sent by kill()\n");
    } else if (info->si_code == SI_KERNEL) {
        printf("Signal sent by the kernel\n");
    }
    
    // For SIGFPE, we can get more specific information
    if (signum == SIGFPE) {
        switch (info->si_code) {
            case FPE_INTDIV:
                printf("Integer divide by zero\n");
                break;
            case FPE_INTOVF:
                printf("Integer overflow\n");
                break;
            case FPE_FLTDIV:
                printf("Floating point divide by zero\n");
                break;
            case FPE_FLTOVF:
                printf("Floating point overflow\n");
                break;
            case FPE_FLTUND:
                printf("Floating point underflow\n");
                break;
            case FPE_FLTRES:
                printf("Floating point inexact result\n");
                break;
            case FPE_FLTINV:
                printf("Floating point invalid operation\n");
                break;
            case FPE_FLTSUB:
                printf("Subscript out of range\n");
                break;
            default:
                printf("Unknown floating point exception\n");
                break;
        }
    }
}

// Basic signal handling demonstration
void basic_signal_demo() {
    printf("\n=== BASIC SIGNAL HANDLING DEMO ===\n");
    
    // Register signal handler for SIGINT (Ctrl+C)
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Failed to register SIGINT handler");
        return;
    }
    
    printf("Registered SIGINT handler.\n");
    printf("Press Ctrl+C three times to exit...\n");
    
    // Main loop
    while (!termination_requested) {
        printf("Working... (Press Ctrl+C to interrupt)\r");
        fflush(stdout);
        sleep(1);
    }
    
    printf("\nShutting down gracefully...\n");
}

// Demonstrate sigaction API
void sigaction_demo() {
    printf("\n=== SIGACTION API DEMONSTRATION ===\n");
    
    // Set up sigaction struct
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    
    // Use sigaction_info_handler which receives additional info
    sa.sa_sigaction = sigaction_info_handler;
    
    // SA_SIGINFO flag tells the system to use sa_sigaction field instead of sa_handler
    // SA_RESTART flag causes system calls to be automatically restarted if interrupted
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    
    // Block SIGTERM during the execution of the handler
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGTERM);
    
    // Register handler for SIGUSR1
    if (sigaction(SIGUSR1, &sa, NULL) < 0) {
        perror("Failed to register SIGUSR1 handler");
        return;
    }
    
    // Also handle SIGFPE (floating point exceptions) for demonstration
    if (sigaction(SIGFPE, &sa, NULL) < 0) {
        perror("Failed to register SIGFPE handler");
        return;
    }
    
    printf("Registered signal handlers using sigaction().\n");
    printf("Send SIGUSR1 to this process (PID: %d) using:\n", getpid());
    printf("  kill -SIGUSR1 %d\n", getpid());
    printf("Press Enter to continue to the next demo...\n");
    
    // Wait for a while to allow signals to be received
    for (int i = 0; i < 10; i++) {
        sleep(1);
        printf("Waiting for signals... %d seconds left\r", 10 - i);
        fflush(stdout);
    }
    
    printf("\nSignal demonstration completed                   \n");
}

// Demonstrate signal masking
void signal_mask_demo() {
    printf("\n=== SIGNAL MASKING DEMONSTRATION ===\n");
    
    // Set up signal handlers for USR1 and USR2
    signal(SIGUSR1, usr_signal_handler);
    signal(SIGUSR2, usr_signal_handler);
    
    printf("Registered handlers for SIGUSR1 and SIGUSR2.\n");
    printf("Current process ID: %d\n", getpid());
    
    // Create a signal mask to block SIGUSR1
    sigset_t mask, old_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    
    // Block SIGUSR1
    printf("Blocking SIGUSR1 for 5 seconds...\n");
    if (sigprocmask(SIG_BLOCK, &mask, &old_mask) < 0) {
        perror("sigprocmask failed");
        return;
    }
    
    printf("SIGUSR1 is now blocked. You can send signals:\n");
    printf("  kill -SIGUSR1 %d  (this will be pending)\n", getpid());
    printf("  kill -SIGUSR2 %d  (this will be processed immediately)\n", getpid());
    
    // Wait while SIGUSR1 is blocked
    for (int i = 0; i < 5; i++) {
        printf("Waiting with SIGUSR1 blocked... %d seconds left\r", 5 - i);
        fflush(stdout);
        sleep(1);
    }
    
    // Check for pending signals
    sigset_t pending;
    sigpending(&pending);
    
    printf("\nChecking for pending signals...\n");
    if (sigismember(&pending, SIGUSR1)) {
        printf("SIGUSR1 is pending!\n");
    } else {
        printf("No pending SIGUSR1 signals.\n");
    }
    
    // Unblock SIGUSR1
    printf("Unblocking SIGUSR1...\n");
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        perror("sigprocmask failed");
        return;
    }
    
    printf("SIGUSR1 is now unblocked. Pending signals will be delivered.\n");
    sleep(1);  // Give time for pending signals to be delivered
    
    printf("\nSIGUSR1 received: %d times\n", sigusr1_count);
    printf("SIGUSR2 received: %d times\n", sigusr2_count);
}

// Demonstrate unsafe signal handling
void unsafe_signal_demo() {
    printf("\n=== UNSAFE SIGNAL HANDLING DEMONSTRATION ===\n");
    
    // Register an unsafe signal handler
    signal(SIGUSR1, unsafe_signal_handler);
    
    printf("Registered unsafe handler for SIGUSR1.\n");
    printf("Current process ID: %d\n", getpid());
    printf("Send multiple SIGUSR1 signals quickly to demonstrate problems:\n");
    printf("  kill -SIGUSR1 %d\n", getpid());
    
    // Wait for signals
    printf("Waiting for signals...\n");
    for (int i = 0; i < 10; i++) {
        printf("Waiting... %d seconds left\r", 10 - i);
        fflush(stdout);
        sleep(1);
    }
    
    printf("\nUnsafe signal demo completed                  \n");
    printf("Problematic practices shown:\n");
    printf("1. Using non-async-signal-safe functions (printf, sleep)\n");
    printf("2. Long-running operations in signal handlers\n");
    printf("3. No protection against re-entrancy issues\n");
}

// Demonstrate proper cleanup in a signal handler
void cleanup_handler(int signum) {
    printf("\nReceived termination signal %d. Cleaning up...\n", signum);
    
    // Set the termination flag
    termination_requested = 1;
    
    // Don't perform actual cleanup here - just signal the main loop to do it
}

void cleanup_demo() {
    printf("\n=== PROPER CLEANUP DEMONSTRATION ===\n");
    
    // Register handler for multiple signals
    signal(SIGINT, cleanup_handler);
    signal(SIGTERM, cleanup_handler);
    
    printf("Registered cleanup handlers for SIGINT and SIGTERM.\n");
    printf("Current process ID: %d\n", getpid());
    printf("Send a signal to trigger cleanup:\n");
    printf("  kill -SIGINT %d  or  kill -SIGTERM %d\n", getpid(), getpid());
    
    // Reset the termination flag
    termination_requested = 0;
    
    // Main loop with periodic checks for termination
    while (!termination_requested) {
        printf("Working... (Send SIGINT or SIGTERM to exit)\r");
        fflush(stdout);
        
        // Simulate work in small chunks, checking for termination
        for (int i = 0; i < 10 && !termination_requested; i++) {
            usleep(100000);  // 100ms
        }
    }
    
    // Proper cleanup in main program, not in signal handler
    printf("\nPerforming proper cleanup in main program...\n");
    printf("1. Closing files...\n");
    sleep(1);
    printf("2. Releasing resources...\n");
    sleep(1);
    printf("3. Saving state...\n");
    sleep(1);
    
    printf("Cleanup completed. Exiting gracefully.\n");
}

// Explain signals and best practices
void explain_signals() {
    printf("\n=== SIGNAL HANDLING BEST PRACTICES ===\n");
    
    printf("1. Signal Basics:\n");
    printf("   - Signals are software interrupts sent to a process\n");
    printf("   - Can be sent by the kernel, other processes, or the process itself\n");
    printf("   - Default actions: terminate, ignore, stop, or continue\n\n");
    
    printf("2. Common Signals:\n");
    printf("   - SIGINT (2): Interrupt from keyboard (Ctrl+C)\n");
    printf("   - SIGTERM (15): Termination request (default for kill)\n");
    printf("   - SIGKILL (9): Forced termination (cannot be caught)\n");
    printf("   - SIGSEGV (11): Invalid memory access\n");
    printf("   - SIGPIPE (13): Write to pipe with no readers\n");
    printf("   - SIGUSR1/2 (10/12): User-defined signals\n\n");
    
    printf("3. Best Practices:\n");
    printf("   - Keep signal handlers simple and fast\n");
    printf("   - Use only async-signal-safe functions in handlers\n");
    printf("   - Use sig_atomic_t for variables accessed in handlers\n");
    printf("   - Set flags in handlers, do actual work in main code\n");
    printf("   - Use sigaction() instead of signal() when possible\n");
    printf("   - Block signals during critical sections\n\n");
    
    printf("4. Avoid in Signal Handlers:\n");
    printf("   - malloc(), free() - not async-signal-safe\n");
    printf("   - printf() and other I/O - not async-signal-safe\n");
    printf("   - pthread functions - not async-signal-safe\n");
    printf("   - Accessing non-atomic, shared data structures\n");
    printf("   - Long-running operations\n\n");
    
    printf("5. Signal Mask Management:\n");
    printf("   - sigprocmask(): Block signals at process level\n");
    printf("   - pthread_sigmask(): Block signals at thread level\n");
    printf("   - sigsuspend(): Atomically unblock signals and wait\n");
}

int main() {
    printf("==== SIGNAL HANDLING DEMONSTRATION ====\n");
    
    // Run the demos
    basic_signal_demo();
    sigaction_demo();
    signal_mask_demo();
    unsafe_signal_demo();
    cleanup_demo();
    explain_signals();
    
    return 0;
}