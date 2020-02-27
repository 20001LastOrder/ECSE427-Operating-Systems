the project can be compiled with both remote shell of mimi.cs.mcgill.ca server and windows 10

Some design decisions:
- every CMD expect quit takes exact number of arguments, that means if 'set' receives 1 or 3 arguments this will result to an angument arror.
- any error will result an error message but the script will continue running to the end
- The errors are grouped into several categories:
	- CMD DOES NOT EXIST
	- ARGUMENT ERROR
	- SHELL MEMORY ERROR
	- IO ERROR
	- KERNEL ERROR

File Structure:
	-root
	 -header
	  -header files...
	 -c
	  -c files....
	 -object file and compilation result...

Compile and run
	-Windows
		-Compile (requires make for Windows): make
		-Run: mykernel.exe
	-Linux
		-Compile: make or sh compile.sh
		Run: ./mykernel

To delete all the object file: make clear
