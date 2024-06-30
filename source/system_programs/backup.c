#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>
#include <limits.h>
#include <string.h>

void create_archive_directory(const char *project_dir)
{
    char archive_path[PATH_MAX];
    snprintf(archive_path, sizeof(archive_path), "%s/archive", project_dir);

    struct stat st = {0};

    if (stat(archive_path, &st) == -1)
    {
        mkdir(archive_path, 0700);
    }
}

void get_current_time(char *buffer, size_t buffer_size)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, buffer_size, "%Y%m%d%H%M%S", timeinfo);
}

void create_backup(const char *backup_dir, const char *project_dir)
{
    char zip_filename[256];
    char timestamp[32];
    char command[8192];
    char abs_backup_dir[PATH_MAX];
    struct stat path_stat;

    // Convert relative path to absolute path based on PWD
    if (backup_dir[0] == '/')
    {
        // Absolute path
        strncpy(abs_backup_dir, backup_dir, sizeof(abs_backup_dir));
    }
    else
    {
        // Relative path
        snprintf(abs_backup_dir, sizeof(abs_backup_dir), "%s/%s", project_dir, backup_dir);
    }

    // Check if the path is a directory or file
    if (stat(abs_backup_dir, &path_stat) != 0)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    get_current_time(timestamp, sizeof(timestamp));
    snprintf(zip_filename, sizeof(zip_filename), "backup-%s-%s.zip", basename((char *)backup_dir), timestamp);

    // Prepare the zip command
    if (S_ISDIR(path_stat.st_mode))
    {
        snprintf(command, sizeof(command), "zip -r %s/archive/%s %s", project_dir, zip_filename, abs_backup_dir);
    }
    else
    {
        snprintf(command, sizeof(command), "zip %s/archive/%s %s", project_dir, zip_filename, abs_backup_dir);
    }

    printf("Creating backup of '%s' at '%s/archive/%s'\n", abs_backup_dir, project_dir, zip_filename);

    int result = system(command);
    if (result != 0)
    {
        fprintf(stderr, "Error creating backup. Command failed with exit code %d\n", result);
        exit(EXIT_FAILURE);
    }

    printf("Backup created successfully.\n");
}

int main()
{
    const char *backup_dir = getenv("BACKUP_DIR");
    const char *project_dir = getenv("PWD");

    if (backup_dir == NULL)
    {
        fprintf(stderr, "Error: BACKUP_DIR environment variable is not set.\n");
        exit(EXIT_FAILURE);
    }

    if (project_dir == NULL)
    {
        fprintf(stderr, "Error: PWD environment variable is not set.\n");
        exit(EXIT_FAILURE);
    }

    create_archive_directory(project_dir);
    create_backup(backup_dir, project_dir);

    return 0;
}
