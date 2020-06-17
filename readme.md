# Shell

The given implementation of shell is capable of handling simple commands present in any UNIX/Linux Distribution such as ls, cat, etc.

![Pics](https://raw.githubusercontent.com/asmitks/images/master/pvz.png)

(
## Getting Started

An implementation of a basic shell in C.

### Design

The given implementation of shell is capable of handling simple commands present in any UNIX/Linux Distribution such as ls, cat, etc.

By default, the shell program waits for user input from the stdin. After the user enters some command, the shell program parses the input to interpret I/O redirection, pipe, etc.) and then:
* If there is a single command (i.e. no pipes):
		Then a child process is forked, and with appropriate output/input redirection and the command is executed.
* Else:
		The No. of pipes (and hence the no. of commands) are determined, and those many no. of pipes are created and at each iteration, and a child process is forked and this executes the corresponding command, after appropriately adjusting the output/input file descriptors according to the pipe in the original command.


## Walkthrough

When the command ** “/bin/ls | /usr/bin/sort | /usr/bin/uniq” ** is executed, the input string is passed to the read_command() input, the input is tokenised into three commands
* /usr/bin/ls
* /usr/bin/sort
* /usr/bin/uniq

And correspondingly line 90 of shell.c is executed (as cnt = 3), 2 pipes are created and these pipes are set accordingly by process
* pid[0]:
	redirects output to fd[0][1]
* pid[1]:
	redirects input to fd[0][0]
	redirects output to fd[1][1]
* pid[2]:
	redirects input to fd[1][0]
and each process calls exec(), while the parent waits for all child process finish their execution.

## Built With

* C

## Version Info

Version : 1.0.1

## Authors

* **Suchet Aggarwal** - *IIIT-Delhi* - [Other Work](https://github.com/Suchet-Agg)


## Acknowledgments

The shell was built as part of OPERATING SYSTEMS course at IIIT Delhi

