#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/sysinfo.h>

void print_sys_info()
{
    struct utsname uname_data;
    struct sysinfo sys_info;
    struct passwd *pw;
    char *user;

    // get system information
    if (uname(&uname_data) == -1)
    {
        perror("uname");
        exit(EXIT_FAILURE);
    }

    // Get system uptime, load, etc.
    if (sysinfo(&sys_info) == -1)
    {
        perror("sysinfo");
        exit(EXIT_FAILURE);
    }

    // Get the current user
    user = getenv("USER");

    // Print system information
    printf("Simple System Information\n");
    printf("OS: %s\n", uname_data.sysname);
    printf("Kernel: %s\n", uname_data.release);
    printf("Hostname: %s\n", uname_data.nodename);
    printf("Uptime: %ld days, %ld hours, %ld minutes\n", sys_info.uptime / 86400, (sys_info.uptime % 86400) / 3600, (sys_info.uptime % 3600) / 60);
    printf("User: %s\n", user);
    printf("CPU: %s\n", uname_data.machine);
    printf("Total RAM: %lu MB\n", sys_info.totalram / 1024 / 1024);
}

int main()
{
    print_sys_info();
    return 0;
}
