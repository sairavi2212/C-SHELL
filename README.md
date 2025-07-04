# C-SHELL

A feature-rich shell implementation in C with various built-in commands and utilities.

## Features

### Built-in Commands
- **hop**: Directory navigation (similar to `cd`)
  - `hop ~`: Navigate to home directory
  - `hop -`: Navigate to previous directory
  - `hop <path>`: Navigate to specified path

- **reveal**: List directory contents (similar to `ls`)
  - `reveal -l`: Long listing with permissions
  - `reveal -a`: Show hidden files
  - `reveal -al`: Combine both flags
  - `reveal <path>`: Show contents of specified directory

- **seek**: Search for files and directories
  - `seek <filename>`: Search for file/directory
  - `seek -d <filename>`: Search for directories only
  - `seek -f <filename>`: Search for files only
  - `seek -e <filename>`: Execute found file/directory (open directory or show file contents)

- **proclore**: Display process information
  - `proclore`: Show information about shell process
  - `proclore <pid>`: Show information about specific process

- **log**: Command history management
  - `log`: Display command history
  - `log purge`: Clear command history
  - `log execute <n>`: Execute nth command from history

- **activities**: Display background processes

- **ping**: Send signals to processes
  - `ping <pid> <signal>`: Send signal to process

- **fg**: Bring a background process to foreground
  - `fg <pid>`: Bring process with specified PID to foreground

- **bg**: Resume a stopped background process
  - `bg <pid>`: Resume process with specified PID in background

- **neonate**: Display most recently created process ID
  - `neonate -n <time>`: Display newest PID every n seconds (press 'x' to exit)

- **iMan**: Fetch man page information from the internet
  - `iMan <command>`: Display manual for specified command

### Additional Features
- **I/O Redirection**: Support for `<`, `>`, and `>>` operators
- **Command Piping**: Support for `|` operator to pipe commands
- **Background Processes**: Run commands in background with `&`
- **Signal Handling**: Proper handling of CTRL+C, CTRL+Z, and CTRL+D
- **Command Aliases**: Define aliases in `.myshrc` file

## How to Compile and Run

1. Navigate to the SHELL directory:
   ```
   cd SHELL
   ```

2. Compile the project:
   ```
   make
   ```

3. Run the shell:
   ```
   ./myprogram
   ```

4. Exit the shell:
   ```
   exit
   ```
   or press CTRL+D

## Implementation Details

- **Command Execution**: Commands are parsed and executed with proper handling of arguments
- **Process Management**: Efficient tracking of foreground and background processes
- **File Handling**: Proper implementation of file operations with permission checks
- **Memory Management**: Careful allocation and deallocation of memory resources
- **Signal Handling**: Customized signal handlers for various terminal signals
- **Networking**: Implementation of network functionality for iMan command

## Note

This shell implementation aims to provide a subset of the functionality found in standard shells like bash while maintaining simplicity and educational value.