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
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_RESET "\033[0m"

const char *builtin_commands[] = {
    "cd",       // Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
    "help",     //  List all builtin commands in the shell
    "exit",     // Exits the shell
    "usage",    // Provides a brief usage guide for the shell and its built-in command
    "env",      // Lists all the environment variables currently set in the shell
    "setenv",   // Sets or modifies an environment variable for this shell session
    "unsetenv", // Sets an alias for a command.
    "alias",    // Sets an alias for a command.
    "unalias",  // Removes an alias.
    "aliases",  // Lists all defined aliases.
    "history"   // Displays the command history.
};

// Array of built-in command descriptions
char *builtin_descriptions[] = {
    "cd directory_name to change the current working directory of the shell",
    "help for supported commands",
    "exit to terminate the shell gracefully",
    "usage <command> to display usage information for a specific command",
    "env to list all registered environment variables",
    "setenv ENV=VALUE to set a new env variable for the shell",
    "unsetenv ENV to remove this env from the list of env variables",
    "alias <name> <command> to set an alias",
    "unalias <name> to remove an alias",
    "aliases to list all aliases",
    "history to show command history"};

// Alias related functions
typedef struct
{
    char *name;
    char *command;
} Alias;

extern Alias aliases[];
extern int alias_count;

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
int set_alias(char **args);
int unset_alias(char **args);
int list_aliases(char **args);
char *get_alias_command(const char *name);
int shell_history(char **args);

/*** This is array of functions, with argument char ***/
int (*builtin_command_func[])(char **) = {
    &shell_cd,      // builtin_command_func[0]: cd
    &shell_help,    // builtin_command_func[1]: help
    &shell_exit,    // builtin_command_func[2]: exit
    &shell_usage,   // builtin_command_func[3]: usage
    &list_env,      // builtin_command_func[4]: env
    &set_env_var,   // builtin_command_func[5]: setenv
    &unset_env_var, // builtin_command_func[6]: unsetenv
    &set_alias,     // builtin_command_func[7]: alias
    &unset_alias,   // builtin_command_func[8]: unalias
    &list_aliases,  // builtin_command_func[9]: aliases
    &shell_history  // builtin_command_func[10]: history
};

int num_builtin_functions();
