#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <sys/time.h>    /* To calculate time taken for operation */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* terminal control definitions */
#include <stdlib.h>
#include <signal.h>

#define ERROR -1
#define SUCCESS 1
#define DEFAULT_UART_DEV "/dev/ttyS0"

/* UART stress test structure */
struct uart_stress_test {
    int fd;                                         /* file descriptor for port */
    long int baudrate;                              /* baudrate to be set */
    int flow_cntrl;                                 /* flow control setting */
    struct timeval start_time, end_time, diff_time; /* time measurement */
    int test_cycles;                                /* number of test cycles */
    int current_cycle;                              /* current test cycle */
    int success_count;                              /* successful operations */
    int failure_count;                              /* failed operations */
    char device_name[256];                          /* device name */
};

struct uart_stress_test stress_test;
int test_running = 1;

/* Function prototypes */
int initport(int fd, long baudrate, int flow_ctrl);
void signalHandler(int sig);
void display_usage(const char* program_name);
void display_results();

int initport(int fd, long baudrate, int flow_ctrl) {
    struct termios options;
    memset(&options, 0, sizeof(options));
    
    if (ERROR == tcgetattr(fd, &options)) {
        perror("tcgetattr");
        return ERROR;
    }

    /* Set baudrate */
    switch (baudrate) {
        case 300:
            cfsetispeed(&options, B300);
            cfsetospeed(&options, B300);
            options.c_cflag |= B300;
            break;
        case 600:
            cfsetispeed(&options, B600);
            cfsetospeed(&options, B600);
            options.c_cflag |= B600;
            break;
        case 1200:
            cfsetispeed(&options, B1200);
            cfsetospeed(&options, B1200);
            options.c_cflag |= B1200;
            break;
        case 2400:
            cfsetispeed(&options, B2400);
            cfsetospeed(&options, B2400);
            options.c_cflag |= B2400;
            break;
        case 4800:
            cfsetispeed(&options, B4800);
            cfsetospeed(&options, B4800);
            options.c_cflag |= B4800;
            break;
        case 9600:
        default:
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            options.c_cflag |= B9600;
            break;
        case 19200:
            cfsetispeed(&options, B19200);
            cfsetospeed(&options, B19200);
            options.c_cflag |= B19200;
            break;
        case 38400:
            cfsetispeed(&options, B38400);
            cfsetospeed(&options, B38400);
            options.c_cflag |= B38400;
            break;
        case 115200:
            cfsetispeed(&options, B115200);
            cfsetospeed(&options, B115200);
            options.c_cflag |= B115200;
            break;
    }

    /* Enable or Disable Hardware flow control */
    if (!flow_ctrl) {
        options.c_cflag &= ~CRTSCTS;
    } else {
        options.c_cflag |= CRTSCTS;
    }

    /* No Parity, 8N1 */
    options.c_cflag &= ~PARENB; // No parity bit
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;  // Clear character size
    options.c_cflag |= CS8;     // 8 bits

    /* Hardware Control Options */
    options.c_cflag |= (CLOCAL | CREAD);

    /* Terminal Control options */
    options.c_lflag &= ~(ICANON | IEXTEN | ECHO | ISIG);
    options.c_iflag &= ~(ICRNL | INPCK | ISTRIP | BRKINT | IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 3;

    if (ERROR == tcflush(fd, TCOFLUSH)) {
        perror("tcflush");
        return ERROR;
    }

    if (ERROR == tcsetattr(fd, TCSANOW, &options)) {
        perror("tcsetattr");
        return ERROR;
    }

    return SUCCESS;
}

void signalHandler(int sig) {
    printf("\n\nReceived signal %d, stopping test...\n", sig);
    test_running = 0;
    display_results();
    exit(0);
}

void display_usage(const char* program_name) {
    printf("\nUsage: %s <cycles> <baudrate> <flow_control> [device]\n", program_name);
    printf("  cycles       : Number of test cycles (0 for infinite)\n");
    printf("  baudrate     : Baud rate (300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 115200)\n");
    printf("  flow_control : Flow control (0=disabled, 1=enabled)\n");
    printf("  device       : Serial device (optional, default: %s)\n", DEFAULT_UART_DEV);
    printf("\nExample: %s 1000 115200 0 /dev/ttyS0\n", program_name);
    printf("         %s 0 9600 1    (infinite cycles with default device)\n", program_name);
}

void display_results() {
    gettimeofday(&stress_test.end_time, NULL);
    timersub(&stress_test.end_time, &stress_test.start_time, &stress_test.diff_time);
    
    printf("\n=== UART Stress Test Results ===\n");
    printf("Device: %s\n", stress_test.device_name);
    printf("Baudrate: %ld\n", stress_test.baudrate);
    printf("Flow Control: %s\n", stress_test.flow_cntrl ? "Enabled" : "Disabled");
    printf("Test Cycles: %d\n", stress_test.current_cycle);
    printf("Successful Operations: %d\n", stress_test.success_count);
    printf("Failed Operations: %d\n", stress_test.failure_count);
    printf("Success Rate: %.2f%%\n", 
           stress_test.current_cycle > 0 ? 
           (float)stress_test.success_count / stress_test.current_cycle * 100 : 0);
    printf("Total Time: %ld.%06ld seconds\n", 
           stress_test.diff_time.tv_sec, stress_test.diff_time.tv_usec);
    
    if (stress_test.current_cycle > 0) {
        double avg_time = (double)stress_test.diff_time.tv_sec + 
                         (double)stress_test.diff_time.tv_usec / 1000000.0;
        printf("Average Time per Cycle: %.6f seconds\n", avg_time / stress_test.current_cycle);
    }
    printf("===============================\n");
}

int main(int argc, char **argv) {
    int j, error = 0;
    
    /* Initialize structure */
    memset(&stress_test, 0, sizeof(stress_test));
    strcpy(stress_test.device_name, DEFAULT_UART_DEV);
    
    /* Register signal handler for Ctrl+C */
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        printf("ERROR: Failed to register signal handler\n");
        exit(1);
    }
    
    /* Check command line arguments */
    if (argc < 4) {
        display_usage(argv[0]);
        exit(1);
    }
    
    /* Parse arguments */
    j = sscanf(argv[1], "%d", &stress_test.test_cycles);
    if (j != 1 || stress_test.test_cycles < 0) {
        printf("ERROR: Invalid test cycles\n");
        error = 1;
    }
    
    j = sscanf(argv[2], "%li", &stress_test.baudrate);
    if (j != 1) {
        printf("ERROR: Invalid baudrate\n");
        error = 1;
    }
    
    j = sscanf(argv[3], "%i", &stress_test.flow_cntrl);
    if (j != 1 || (stress_test.flow_cntrl != 0 && stress_test.flow_cntrl != 1)) {
        printf("ERROR: Invalid flow control setting (0 or 1)\n");
        error = 1;
    }
    
    /* Optional device name */
    if (argc >= 5) {
        strncpy(stress_test.device_name, argv[4], sizeof(stress_test.device_name) - 1);
        stress_test.device_name[sizeof(stress_test.device_name) - 1] = '\0';
    }
    
    if (error) {
        display_usage(argv[0]);
        exit(1);
    }
    
    /* Display test configuration */
    printf("\n=== UART Stress Test Configuration ===\n");
    printf("Device: %s\n", stress_test.device_name);
    if (stress_test.test_cycles == 0) {
        printf("Test Cycles: Infinite\n");
    } else {
        printf("Test Cycles: %d\n", stress_test.test_cycles);
    }
    printf("Baudrate: %ld\n", stress_test.baudrate);
    printf("Flow Control: %s\n", stress_test.flow_cntrl ? "Enabled" : "Disabled");
    printf("=====================================\n");
    
    /* Start timing */
    gettimeofday(&stress_test.start_time, NULL);
    
    printf("\nStarting UART stress test... (Press Ctrl+C to stop)\n");
    
    /* Main test loop */
    stress_test.current_cycle = 0;
    while (test_running && (stress_test.test_cycles == 0 || stress_test.current_cycle < stress_test.test_cycles)) {
        stress_test.current_cycle++;
        
        /* Try to open the port */
        stress_test.fd = open(stress_test.device_name, O_RDWR | O_NOCTTY | O_NDELAY);
        if (stress_test.fd == -1) {
            printf("Cycle %d: Failed to open port %s - %s\n", 
                   stress_test.current_cycle, stress_test.device_name, strerror(errno));
            stress_test.failure_count++;
            continue;
        }
        
        /* Configure the port */
        if (initport(stress_test.fd, stress_test.baudrate, stress_test.flow_cntrl) == ERROR) {
            printf("Cycle %d: Failed to configure port\n", stress_test.current_cycle);
            close(stress_test.fd);
            stress_test.failure_count++;
            continue;
        }
        
        /* Sleep for 1 second */
        sleep(1);
        
        /* Close the port */
        if (close(stress_test.fd) == -1) {
            printf("Cycle %d: Failed to close port - %s\n", 
                   stress_test.current_cycle, strerror(errno));
            stress_test.failure_count++;
        } else {
            stress_test.success_count++;
        }
        
        /* Display progress every 100 cycles */
        if (stress_test.current_cycle % 100 == 0) {
            printf("Completed %d cycles (Success: %d, Failure: %d)\n", 
                   stress_test.current_cycle, stress_test.success_count, stress_test.failure_count);
        }
    }
    
    /* Display final results */
    display_results();
    
    return 0;
}
