# CSEShell

CSEShell is a simple, custom shell for Unix-based systems, designed to provide an interface for executing system programs. This project includes a basic shell implementation, a set of system programs (`find`, `ld`, `ldr`, `backup`, `sys`, `dspawn`, `dcheck`), and some test files.

## Directory Structure

The project is organized as follows:

- `bin/` - Contains compiled executables for system programs.
  - `find` - Program to find files.
  - `ld` - Program for listing the contents of the current directory.
  - `ldr` - Program for listing the contents of the current directory recursively.
  - `backup` - Program for creating backup copies of files.
  - `sys` - Program to provide system information.
  - `dspawn` - Program to spawn a daemon process.
  - `dcheck` - Program to check the status of daemon processes.
- `cseshell` - The main executable for the CSEShell.
- `files/` - Contains various test files used with the shell and system programs.
  - `combined.txt`, `file1.txt`, `file2.txt`, ... - Test text files.
  - `notes.pdf` - A PDF file for testing.
  - `ss.png` - An image file.
- `makefile` - Makefile for building the CSEShell and system programs.
- `source/` - Source code for the shell and system programs.
  - `shell.c` and `shell.h` - Source and header files for the shell.
  - `system_programs/` - Source code and header for the system programs.

## Building the Project

To build the CSEShell and system programs, run the following command in the root directory:

```bash
make
```

This will compile the source code and place the executable files in the appropriate directories.

## Running CSEShell

After building, you can start the shell by running:

```bash
./cseshell
```

### Built-in Commands

CSEShell supports the following built-in commands:

* **cd:** Changes the current working directory.
    * Usage: `cd [directory_name]`
* **help:** Lists all available built-in commands.
    * Usage: `help`
* **exit:** Terminates the shell.
    * Usage: `exit`
* **usage:** Provides a brief usage guide for the shell and built-in commands.
    * Usage: `usage [command]`
* **env:** Lists all environment variables.
    * Usage: `env`
* **setenv:** Sets or modifies an environment variable.
    * Usage: `setenv ENV=VALUE`
* **unsetenv:** Removes an environment variable.
    * Usage: `unsetenv ENV`
* **alias:** Creates a shortcut (alias) for a command.
    * Usage: `alias [name] [command]`
* **unalias:** Removes an alias.
    * Usage: `unalias [name]`
* **aliases:** Lists all defined aliases.
    * Usage: `aliases`
* **history:** Displays the command history.
    * Usage: `history`

### Additional Features

* **Alias:** Allows creating shortcuts for commands (e.g., `alias ll ls -la`).
* **Command History:** Tracks previously executed commands accessible with `history`.

### System Programs

CSEShell can execute various system programs:

* `find.c`: Searches for files in a directory.
* `ld.c`: Lists the contents of the current directory.
* `ldr.c`: Lists directory contents recursively.
* `backup.c`: Creates file backups.
* `sys.c`: Provides system information.
* `dspawn.c`: Spawns a daemon process.
* `dcheck.c`: Checks the status of daemon processes.

### Files Directory

The `files/` directory contains various test files (text, PDF, and image) for the shell and system programs.

### Makefile

The `Makefile` provides instructions for compiling the shell and system programs. You can clean the build using:

```bash
make clean
```

### Source Directory

The source code for the shell and system programs is located in the `source/` directory. It includes:

* `shell.c`: Shell implementation.
* `shell.h`: Shell header file.
* `system_programs/`: Directory containing system program source code.

### Sustainability and Inclusivity

**Sustainability**

* **Lightweight System Programs:** Emphasis on creating efficient and resource-friendly system programs.
    * Example: Lightweight implementation of command history for minimal resource usage.
* **Resource Usage Feedback:** Future implementation to display statistics like CPU time, memory usage, and disk I/O per command, promoting resource awareness.

**Inclusivity**

* **Enhanced Accessibility Features:** Clear error messages and support for aliases for easier use.
    * Future Suggestions:
        * Energy-efficient algorithms for reduced power consumption.
        * Customizable interface (text size, color schemes, prompt styles).
        * User-friendly and informative error messages.
     
* **Alias Shortcuts:** Aliases allow users to create shortcuts for frequently used commands, making the shell more accessible and easier to use for everyone, including those who may have difficulty typing long commands.
    * Implementation: Users can set aliases for longer commands, reducing the amount of typing required and minimizing the chance for errors. This feature enhances productivity and makes the shell more user-friendly.
    * Future Suggestions:
        * Allow users to create their own shell scripts via the command line with all the shortcuts they created that they are used to 
