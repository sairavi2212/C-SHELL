# C SHELL

## Assumptions

### Display Requirements

### Input Requirements

#### & and ; should not come together

#### Also works with \t

#### Gives error and discards whole command if ;; or && or ;& is contained (as my original shell does) and prints error and even if command contains two ampersand seperated by space then it prints error

#### Max input length to be 4096.

### Hop

#### Max length of directory is 1000 characters.

#### I am setting the previous directory by default to my home directory (as my shell (zsh) does), so hop - would remain in the home directory if its the first instance of changing the directory.

#### If any path in between is erroneous then I am continuing ignoring the erroneous path like hop .. ../~ - would first would go to parent directory and then ../~ is invalid so it ignores it and then it hops to previous directory (-)

### Reveal

#### Sorting the names of files/directories using the qsort function (but it places capital letters before small letters).

#### If user gives any invalid flags with the valid flags combinedly then I considering only valid flags. I am not handling the invalid flags.

### Log

#### I am creating a file history.txt to store the previous commands accross various sessions

#### I am storing the erroneous commands also in the log.

#### I am not storing the command being executed in log execute index.

### System Commands

#### If a foreground process takes more than or equal to 2 seconds, then I will be printing the whole command with its total time taken (foreground) in the new prompt.

#### Maximum number of background processes is 4096

#### If any erroneous command is run in the background then it runs and exits abnormally.

### Proclore

#### For some processes, we cannot obtain the executable path due to the file permissions, In that case my code outputs permission denied in place of executable path.

#### Virtual Memory size is in Bytes.

### Seek

#### If user gives just a directory name i.e. a direct subdirectory of the current directory and is a ancestral directory then I am considering it as sub directory as per the guidelines.

### Aliases

#### I assume that aliases can occur only in the start of the command enclosed within semicolon.

#### I have implemented only two aliases i.e. reveala and home. 

### Pipe

#### I am assuming the whole command as a single command and I am considering whole command pid to be the foreground pid.

#### I am running the pipeline sequentially since running it concurrently would output undeterministically (I can run it in concurrently by just storing all the pids and waiting for them to terminate/stop as far as I know) though presently I am running it sequentially.

#### I have dealed with commands like a < b > c and also a > b < c where they have the usual meaning as of bash.

#### I am not dealing with commands like echo hi > a.txt > b.txt

#### For commands like hop .. | wc my code would not change the current directory

### Activities

#### I am printing "Running" if the status in the stat file is S/Z/R else I am printing as "Stopped".

#### I am printing in the lexicographic order of the command name.

### iMan

#### If there is no man page found then it prints "No matches for the man page i.e. requested".

## Resources:

#### https://stackoverflow.com/questions/39066998/what-are-the-meaning-of-values-at-proc-pid-stat

#### https://stackoverflow.com/questions/40445596/how-to-make-a-signal-handler-for-sigchld-that-will-reap-background-processes-in

#### https://stackoverflow.com/questions/14663548/c-program-to-list-hidden-and-read-only-files

#### https://stackoverflow.com/questions/845556/how-to-ignore-hidden-files-with-opendir-and-readdir-in-c-library

#### https://stackoverflow.com/questions/900411/how-to-properly-wait-for-foreground-background-processes-in-my-own-shell-in-c

#### https://stackoverflow.com/questions/71862546/properly-reaping-all-child-processes-and-collecting-exit-status

#### https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program

#### https://stackoverflow.com/questions/1610203/unix-programming-not-sure-how-to-use-the-passwd-struct

#### https://stackoverflow.com/questions/42426816/in-what-way-does-waitnull-work-exactly-in-c

#### https://chatgpt.com/c/d6c1977a-bc4e-492d-8ba3-31baa448f506

#### https://chatgpt.com/c/770216fa-5f6a-447e-a97b-4fdca0676270

#### https://chatgpt.com/c/c792deb5-ac2e-4f0d-92ae-f57b50fe0d47

#### https://chatgpt.com/c/5f5b2d24-20ab-41b7-962c-bc50f060f7b2

#### https://chatgpt.com/c/f7f193d3-c46e-4158-897f-d90169ddb514

#### https://chatgpt.com/c/2dca7448-6668-419d-ad0d-ef5a1fe3cc02

#### https://stackoverflow.com/questions/78298936/sending-sigint-to-background-process-does-not-stop-it

#### https://stackoverflow.com/questions/8738925/trapping-sigint-in-a-backgrounded-process?rq=1

#### https://askubuntu.com/questions/890591/why-doesnt-ctrl-c-kill-the-terminal-itself

#### https://stackoverflow.com/questions/42986593/redirecting-inputtext-file-in-c

#### https://stackoverflow.com/questions/35823864/detecting-ctrl-d-in-c

#### https://stackoverflow.com/questions/16639275/grouping-child-processes-with-setpgid

#### https://stackoverflow.com/questions/11987495/what-do-the-numbers-in-proc-loadavg-mean-on-linux

#### https://chatgpt.com/c/66deebd5-6cec-8010-908d-b460c3109a1d

#### 

