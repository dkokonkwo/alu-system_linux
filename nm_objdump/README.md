man or help:

elf (5)
nm (1)
objdump (1)
Learning Objectives
At the end of this project, you are expected to be able to explain to anyone, without the help of Google:

General
What is the ELF format
What are the commands nm and objdump
How to use them properly
How to parse the content of an ELF file
What information can be extracted from an ELF file
What are the differences between nm and objdump
Requirements
General
Allowed editors: vi, vim, emacs
All your files will be compiled on Ubuntu 14.04 LTS
Your C programs and functions will be compiled with gcc 4.8.4 using the flags -Wall -Werror -Wextra and -pedantic
All your files should end with a new line
A README.md file, at the root of the folder of the project, is mandatory
Your code should use the Betty style. It will be checked using betty-style.pl and betty-doc.pl
You are not allowed to have more than 5 functions per file
All your header files should be include guarded
Allowed Functions and System Calls
Unless specified otherwise, you are allowed to use the C standard library
You’re not allowed to use system(3)
You’re not allowed to use exec*(2 and 3)
Compilation
You have to provide a Makefile in order to compile each task
Make sure each task compiles on Ubuntu 14.04 LTS, with gcc 4.8.4
You must compile each task using the flags -Wall -Werror -Wextra and -pedantic
Tests
Your program must be able to handle both 32-bit and 64-bit ELF files
Your program must be able to handle both little and big endian ELF files
Your program must be able to handle all types of ELF files
More Info
See also /usr/include/elf.h
