// Include the shell header file for necessary constants and function declarations
#include "shell.h"

// Function to read a command from the user input
void read_command(char **cmd)
{
  // Define a character array to store the command line input
  char line[MAX_LINE];
  // Initialize count to keep track of the number of characters read
  int count = 0, i = 0;
  // Array to hold pointers to the parsed command arguments
  char *array[MAX_ARGS], *command_token;

  // Infinite loop to read characters until a newline or maximum line length is reached
  for (;;)
  {
    // Read a single character from standard input
    int current_char = fgetc(stdin);
    // Store the character in the line array and increment count
    line[count++] = (char)current_char;
    // If a newline character is encountered, break out of the loop
    if (current_char == '\n')
      break;
    // If the command exceeds the maximum length, print an error and exit
    if (count >= MAX_LINE)
    {
      printf("Command is too long, unable to process\n");
      exit(1);
    }
  }
  // Null-terminate the command line string
  line[count] = '\0';

  // If only the newline character was entered, return without processing
  if (count == 1)
    return;

  // Use strtok to parse the first token (word) of the command
  command_token = strtok(line, " \n");

  // Continue parsing the line into words and store them in the array
  while (command_token != NULL)
  {
    char *alias_command = get_alias_command(command_token);
    if (alias_command)
    {
      // Expand alias command and split into tokens
      char alias_copy[MAX_LINE];
      strncpy(alias_copy, alias_command, MAX_LINE);
      char *alias_token = strtok(alias_copy, " \n");
      while (alias_token != NULL)
      {
        array[i++] = strdup(alias_token);
        alias_token = strtok(NULL, " \n");
      }
    }
    else
    {
      array[i++] = strdup(command_token);
    }
    command_token = strtok(NULL, " \n");
  }

  // Copy the parsed command and its parameters to the cmd array
  for (int j = 0; j < i; j++)
  {
    cmd[j] = array[j];
  }
  // Null-terminate the cmd array to mark the end of arguments
  cmd[i] = NULL;
}

// Function to display the shell prompt
void type_prompt()
{
  // Use a static variable to check if this is the first call to the function
  static int first_time = 1;
  if (first_time)
  {
    // Clear the screen on the first call
#ifdef _WIN32
    system("cls"); // Windows command to clear screen
#else
    system("clear"); // UNIX/Linux command to clear screen
#endif
    first_time = 0;
  }

  // Buffer to store the current working directory
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
  {
    // Print the current working directory
    printf(COLOR_YELLOW "%s\n" COLOR_RESET, cwd);
  }
  else
  {
    perror("getcwd() error");
  }
  fflush(stdout);                       // Flush the output buffer
  printf(COLOR_GREEN "> " COLOR_RESET); // Print the shell prompt
}

// Helper function to figure out how many built-in commands are supported by the shell
int num_builtin_functions()
{
  return sizeof(builtin_commands) / sizeof(char *);
}

int shell_exit(char **args)
{
  return 0;
}

// Handler functions for built-in commands
int shell_cd(char **args)
{
  if (args[1] == NULL)
  {
    fprintf(stderr, "shell: expected argument to \"cd\"\n");
  }
  else
  {
    if (chdir(args[1]) != 0)
    {
      perror("shell");
    }
  }
  return 1;
}

int shell_help(char **args)
{
  printf("Builtin commands:\n");
  for (int i = 0; i < num_builtin_functions(); i++)
  {
    printf("  %s\n", builtin_commands[i]);
  }
  return 1;
}

// Function to handle the usage command
int shell_usage(char **args)
{
  if (args[1] == NULL)
  {
    printf("Command not given. Type: usage <command>\n");
    return 1;
  }

  for (int i = 0; i < num_builtin_functions(); i++)
  {
    if (strcmp(args[1], builtin_commands[i]) == 0)
    {
      printf("Type: %s\n", builtin_descriptions[i]);
      return 1;
    }
  }

  printf("The command you gave, %s, is not part of CSEShell's built-in command\n", args[1]);
  return 1;
}

int list_env(char **args)
{
  extern char **environ;
  for (char **env = environ; *env != 0; env++)
  {
    printf("%s\n", *env);
  }
  return 1;
}

int set_env_var(char **args)
{
  if (args[1] == NULL)
  {
    fprintf(stderr, "shell: expected argument to \"setenv\"\n");
    return 1;
  }

  char *key = strtok(args[1], "=");
  char *value = strtok(NULL, "=");

  if (key == NULL || value == NULL)
  {
    fprintf(stderr, "shell: expected argument to \"setenv\" in the form KEY=VALUE\n");
    return 1;
  }

  if (setenv(key, value, 1) != 0)
  {
    perror("shell");
  }

  return 1;
}

int unset_env_var(char **args)
{
  if (args[1] == NULL)
  {
    fprintf(stderr, "shell: expected argument to \"unsetenv\"\n");
    return 1;
  }

  if (unsetenv(args[1]) != 0)
  {
    perror("shell");
  }

  return 1;
}

#define MAX_ALIASES 50
Alias aliases[MAX_ALIASES];
int alias_count = 0;

// Function to get the alias command for a given alias name
char *get_alias_command(const char *name)
{
  for (int i = 0; i < alias_count; i++)
  {
    if (strcmp(name, aliases[i].name) == 0)
    {
      return aliases[i].command;
    }
  }
  return NULL;
}

// Function to set a new alias
int set_alias(char **args)
{
  if (args[1] == NULL || args[2] == NULL)
  {
    fprintf(stderr, "Usage: alias <name> <command>\n");
    return 1;
  }

  // Concatenate all parts of the alias command into a single string
  char command[MAX_LINE] = "";
  for (int i = 2; args[i] != NULL; i++)
  {
    strcat(command, args[i]);
    if (args[i + 1] != NULL)
    {
      strcat(command, " ");
    }
  }

  // Check if the alias already exists and update it
  for (int i = 0; i < alias_count; i++)
  {
    if (strcmp(args[1], aliases[i].name) == 0)
    {
      free(aliases[i].command);
      aliases[i].command = strdup(command);
      return 1;
    }
  }

  // If the alias does not exist, add a new one
  if (alias_count >= MAX_ALIASES)
  {
    fprintf(stderr, "Maximum number of aliases reached.\n");
    return 1;
  }

  aliases[alias_count].name = strdup(args[1]);
  aliases[alias_count].command = strdup(command);
  alias_count++;

  return 1;
}

// Function to unset an alias
int unset_alias(char **args)
{
  if (args[1] == NULL)
  {
    fprintf(stderr, "Usage: unalias <name>\n");
    return 1;
  }

  for (int i = 0; i < alias_count; i++)
  {
    if (strcmp(args[1], aliases[i].name) == 0)
    {
      free(aliases[i].name);
      free(aliases[i].command);
      aliases[i] = aliases[alias_count - 1];
      alias_count--;
      return 1;
    }
  }

  fprintf(stderr, "Alias not found: %s\n", args[1]);
  return 1;
}

// Function to list all aliases
int list_aliases(char **args)
{
  for (int i = 0; i < alias_count; i++)
  {
    printf("%s='%s'\n", aliases[i].name, aliases[i].command);
  }
  return 1;
}

int execute_builtin_command(char **cmd)
{
  for (int command_index = 0; command_index < num_builtin_functions(); command_index++)
  {
    if (strcmp(cmd[0], builtin_commands[command_index]) == 0)
    {
      return (*builtin_command_func[command_index])(cmd);
    }
  }
  return 0;
}

void execute_command(char **cmd)
{
  int child_status;
  pid_t pid;

  pid = fork();

  if (pid < 0)
  {
    printf("Fork failed");
    exit(1);
  }
  else if (pid == 0)
  {
    // char full_path[PATH_MAX];
    // char *project_dir = getenv("PWD");
    // snprintf(full_path, sizeof(full_path), "%s/bin/%s", project_dir, cmd[0]);

    // execv(full_path, cmd);

    execvp(cmd[0], cmd);

    // If execvp returns, it must have failed
    perror("execvp failed");
    // return 1;

    printf("Command %s not found\n", cmd[0]);
    _exit(1);
  }
  else
  {
    waitpid(pid, &child_status, 0);

    if (WIFEXITED(child_status))
    {
      int exit_status = WEXITSTATUS(child_status);
      if (exit_status != 0)
      {
        printf("Command exited with status %d\n", exit_status);
      }
    }
    else if (WIFSIGNALED(child_status))
    {
      printf("Command terminated by signal %d\n", WTERMSIG(child_status));
    }

    for (int i = 0; cmd[i] != NULL; i++)
    {
      free(cmd[i]);
    }
    memset(cmd, 0, sizeof(cmd));
  }
}

void execute_rc_file(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    perror("Failed to open .rc file");
    return;
  }

  char line[MAX_LINE];
  while (fgets(line, sizeof(line), file))
  {
    if (line[0] == '\n' || line[0] == '#')
    {
      continue; // Skip empty lines and comments
    }

    // Remove trailing newline character
    line[strcspn(line, "\n")] = '\0';

    char *cmd[MAX_ARGS];
    char *command_token = strtok(line, " \n");
    int i = 0;

    while (command_token != NULL)
    {
      cmd[i++] = strdup(command_token);
      command_token = strtok(NULL, " \n");
    }

    cmd[i] = NULL;

    if (cmd[0] == NULL)
    {
      continue;
    }

    if (strncmp(line, "PATH=", 5) == 0)
    {
      char *path = line + 5;
      path[strlen(path)] = '\0';
      setenv("PATH", path, 1);
    }
    else
    {
      // Execute the command
      if (!execute_builtin_command(cmd))
      {
        execute_command(cmd);
      }
    }
  }

  fclose(file);
}

// The main function where the shell's execution begins
int main(void)
{
  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS];

  // Get the project directory from the PWD environment variable
  char *project_dir = getenv("PWD");
  if (project_dir == NULL)
  {
    fprintf(stderr, "Failed to get PWD environment variable.\n");
    exit(1);
  }

  // Execute commands from .cseshellrc
  execute_rc_file(".cseshellrc");

  while (1)
  {

    type_prompt();     // Display the prompt
    read_command(cmd); // Read a command from the user

    // If the command is "exit", break out of the loop to terminate the shell
    if (cmd[0] == NULL)
      continue; // Skips command exceution if the command is empty

    // If the command is "exit", break out of the loop to terminate the shell
    if (strcmp(cmd[0], "exit") == 0)
      break;

    if (!execute_builtin_command(cmd))
    {
      execute_command(cmd);
    }
  }
  return 0;
}
