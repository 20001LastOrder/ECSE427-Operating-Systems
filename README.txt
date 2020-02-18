the mysh file is compiled with the remote shell of mimi.cs.mcgill.ca server

Some design decisions:
- every CMD expect quit takes exact number of arguments, that means if 'set' receives 1 or 3 arguments this will result to an angument arror.
- any error will immediately stop the executing of a script file
- The errors are grouped into several categories:
	- CMD DOES NOT EXIST
	- ARGUMENT ERROR
	- SHELL MEMORY ERROR
	- IO ERROR