#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

char output_file_path[PATH_MAX];

static int daemon_work()
{
    int num = 0;
    FILE *fptr;
    char buffer[1024];

    fptr = fopen(output_file_path, "a");
    if (fptr == NULL)
    {
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Daemon process running with PID: %d, PPID: %d, opening logfile with FD %d\n", getpid(), getppid(), fileno(fptr));

    if (getcwd(buffer, sizeof(buffer)) == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    fprintf(fptr, "Current working directory: %s\n", buffer);
    fclose(fptr);

    while (1)
    {
        fptr = fopen(output_file_path, "a");
        if (fptr == NULL)
        {
            return EXIT_FAILURE;
        }

        fprintf(fptr, "PID %d Daemon writing line %d to the file.\n", getpid(), num);
        num++;
        fclose(fptr);

        sleep(10);

        if (num == 10)
        {
            break;
        }
    }

    return EXIT_SUCCESS;
}

void dspawn()
{
    pid_t pid, daemon_pid;

    if (getcwd(output_file_path, sizeof(output_file_path)) == NULL)
    {
        perror("getcwd() error, exiting now.");
        exit(1);
    }
    strcat(output_file_path, "/dspawn.log");

    pid = fork();
    if (pid < 0)
    {
        exit(0);
    }
    else if (pid > 0)
    {
        exit(0);
    }
    else if (pid == 0)
    {
        setsid();
    }

    signal(SIGHUP, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    daemon_pid = fork();
    if (daemon_pid < 0)
    {
        exit(0);
    }
    if (daemon_pid > 0)
    {
        exit(0);
    }

    umask(0);
    chdir("/");

    for (int f = sysconf(_SC_OPEN_MAX); f >= 0; f--)
    {
        close(f);
    }

    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);

    daemon_work();
}

int main()
{
    dspawn();
    return 0;
}