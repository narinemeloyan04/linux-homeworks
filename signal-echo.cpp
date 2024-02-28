#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>

// Signal handler for SIGUSR1
void sigusr1_handler(int signo, siginfo_t *info, void *context) 
{
    // Extracting information from siginfo_t structure
    pid_t sender_pid = info->si_pid;
    uid_t sender_uid = info->si_uid;
    const char* sender_username = "unknown";  

    // Extracting register values using user_regs_struct
    user_regs_struct* registers = (user_regs_struct*)context;
    unsigned long eip = registers->rip;
    unsigned long eax = registers->rax;
    unsigned long ebx = registers->rbx;

    // Printing the information
    std::cout << "Received a SIGUSR1 signal from process " << sender_pid
              << " executed by " << sender_uid << " (" << sender_username << ")." << std::endl;
    
    std::cout << "State of the context: EIP = " << eip
              << ", EAX = " << eax
              << ", EBX = " << ebx << "." << std::endl;
}

int main() 
{
    // Declare and initialize PID variable
    pid_t this_process_pid = getpid();
    std::cout << "This process PID: " << this_process_pid << std::endl;

    // Register signal handler
    struct sigaction signal_action;
    signal_action.sa_sigaction = sigusr1_handler;
    signal_action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &signal_action, nullptr);

    // Enter infinite loop with sleep
    while (true) 
    {
        sleep(10);
    }

    return 0;
}
