# how does it look like?

- example "hello world" turtel program
```
hello:str:Hello, World!:
print:str:hello:
```
- example loop
```
message:str:oi! im in a loop..!:
gototag:_loop:
	print:str:message:
	print:str:__newline:
	goto:_loop:
```

# how to run programs?

although turtel is interpreted, programs need to be lexed first using the `turtel_lex` program.

syntax:
```
turtel_lex -f infile.trl -o outfile.tlx
# or
turtel_lex < infile.trl > outfile.tlx
```

then run it with
```
turtel file.tlx
# or
turtel < file.tlx
```

# basics

everything in turtel is treated literally, so in theory you can have a variable called "a gamer moment".

everything also needs to be seperated with a SEPERATOR (defined in stdturtel.h). by default it's ':'.

# data structures

num: basic number (long long)
str: basic string [char\* (resizable)]
tof: true or false statement (bool)

# builtin constants

- \_\_OS - tof - 1 if windows, 0 else,
- \_\_newline - str - '\\n',
- \_\_space - str - ' '

# builtin functions

+---------------------------------------+-------------------------------+
| name	| syntax			| definition			|
+-------+-------------------------------+-------------------------------+
| print	| print:data_struc:var:		| prints variable content	|
+-------+-------------------------------+-------------------------------+
| read	| read:data_struc:var:		| reads into variable		|
+-------+-------------------------------+-------------------------------+
| if	| if:var1:op:var2:goto1:goto2	| logic moment			|
+-------+-------------------------------+-------------------------------+
| srun	| srun:var:			| run variable content in shell	|
+-------+-------------------------------+-------------------------------+
| exit	| exit:				| exits				|
+-------+-------------------------------+-------------------------------+
| goto	| goto:gototag_name:		| goes to gototag		|
+-------+-------------------------------+-------------------------------+
|gototag| gototag:name:			| creates a gototag		|
+-------+-------------------------------+-------------------------------+
| add	| add:dest_var:var:		| adds var to dest		|
+-------+-------------------------------+-------------------------------+
| sub	| sub:dest_var:var:		| subtratcs var from dest	|
+-------+-------------------------------+-------------------------------+
| mul	| mul:dest_var:var:		| multiplies dest by var	|
+-------+-------------------------------+-------------------------------+
| div	| div:dest_var:var:		| divides dest by var		|
+-------+-------------------------------+-------------------------------+
| mod	| mod:dest_var:var:		| modulus dest by var		|
+-------+-------------------------------+-------------------------------+
| nowequ| not added yet			|				|
+-------+-------------------------------+-------------------------------+
