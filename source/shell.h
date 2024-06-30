#include <limits.h> // For PATH_MAX
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define BIN_PATH "./bin/"
#define PROJECT_DIR "/home/gable/programming-assignment-1-2024-team/bin/"

const char *builtin_commands[] = {
    "cd",      // Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
    "help",    //  List all builtin commands in the shell
    "exit",    // Exits the shell
    "usage",   // Provides a brief usage guide for the shell and its built-in command
    "env",     // Lists all the environment variables currently set in the shell
    "setenv",  // Sets or modifies an environment variable for this shell session
    "unsetenv" // Removes an environment variable from the shell
};

// Array of built-in command descriptions
char *builtin_descriptions[] = {
    "cd directory_name to change the current working directory of the shell",
    "help for supported commands",
    "exit to terminate the shell gracefully",
    "usage <command> to display usage information for a specific command",
    "env to list all registered environment variables",
    "setenv ENV=VALUE to set a new env variable for the shell",
    "unsetenv ENV to remove this env from the list of env variables"};

/*
Handler of each shell builtin function
*/
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_usage(char **args);
int list_env(char **args);
int set_env_var(char **args);
int unset_env_var(char **args);

/*** This is array of functions, with argument char ***/
int (*builtin_command_func[])(char **) = {
    &shell_cd,     // builtin_command_func[0]: cd
    &shell_help,   // builtin_command_func[1]: help
    &shell_exit,   // builtin_command_func[2]: exit
    &shell_usage,  // builtin_command_func[3]: usage
    &list_env,     // builtin_command_func[4]: env
    &set_env_var,  // builtin_command_func[5]: setenv
    &unset_env_var // builtin_command_func[6]: unsetenv
};

int num_builtin_functions();
