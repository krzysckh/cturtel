.TL 
Intro to scripting in turtel
.AU 
krzych

.NH 1
Syntax

.PP 
Syntax of turtel is quite easy. Every line should consist of:

.B1
.DS B
Keyword, SEPARATOR, options, ..., SEPARATOR

or

Variable Name, SEPARATOR, Variable Type, SEPARATOR, Variable Value, SEPARATOR
.DE
.B2

.PP
So every keyword must be separated with 
.B SEPARATOR 
(defined in stdturtel.h).


.NH 1
Data types

.PP 
Turtel has 3 data types and no way of adding more.

.IP \(bu
num - A basic number. Stores a value of a long long int.
.IP \(bu
str - A basic string. Stores a string. Can be changed and turtel(.c) will manage the memory
.IP \(bu
tof - A basic boolean. Stores 0 or 1. There is actually no need in using it. It won't improve speed or anything. It's just there.

.NH 1
Declaring variables

.PP
To declare a variable, you have to know its type and name.
.PP 
Syntax:
.B1
.DS B
Variable_name:data_type:value:
.DE
.B2

.PP
Examples:
.B1
.DS B
"msg:str:Hello, World!:"

Will declare a string msg with the value of "Hello, World!".

"x:num::"

Will declare a number x with the value of 0.
.DE
.B2

.PP
This syntax allows 3 variables of the same name with different types.

.PP
Turtel has a builtin variable with type str:
.IP \(bu
.I __newline:
 is literally a newline (\\n)

.NH 1
Builtin functions

.IP \(bu
print
.RS
.IP \(bu
syntax:
.I print:type:var:
.IP \(bu
info: Prints variable
.I 'var'
with type
.I 'type'.
.RE
.IP \(bu
read
.RS
.IP \(bu
syntax:
.I read:type:var:
.IP \(bu
info: Reads user input to variable 
.I 'var'
with type
.I 'type'.
.RE
.IP \(bu
if
.RS
.IP \(bu
syntax:
.I if:num_1:operator:num_2:gototag1:gototag2:
.IP \(bu
info: If (num_1 operator num_2) is true goto 
.I gototag1, 
else goto 
.I gototag2.
Operator can be:
.RS
.IP \(bu
.I 'eq'
for equal
.IP \(bu
.I 'lt' 
for a less than b
.IP \(bu
.I 'gt' 
for a greater than b
.IP \(bu
.I 'ne'
for not equal
.RE
.RE
.IP \(bu
srun
.RS
.IP \(bu
syntax:
.I srun:var:
.IP \(bu
info: Runs 
.I 'var'
content with /bin/sh. 
.I 'var' 
must be a str.
.RE
.IP \(bu
exit
.RS
.IP \(bu
syntax:
.I exit:
.IP \(bu
info: exits.
.RE
.IP \(bu
goto
.RS
.IP \(bu
syntax:
.I goto:gototag_name:
.IP \(bu
info: Jumps to 
.I 'gototag_name'.
.RE
.IP \(bu
gototag
.RS
.IP \(bu
syntax:
.I gototag:gototag_name:
.IP \(bu
info: Place to jump from 
.I goto:gototag_name:
.RE
.IP \(bu
add, sub, mul, div, mod
.RS
.IP \(bu
syntax:
.I command:num1:num2:
.IP \(bu
info: Runs arithmetic operations on 
.I 'num1'
op.
.I 'num2' 
and stores the result in num1.
.RE
.IP \(bu
nowequ
.RS
.IP \(bu
syntax:
.I nowequ:type1:var1:type2:var2:
.IP \(bu
info: Converts 
.I 'var2'
with type 
.I 'type2'
to 
.I 'type1' 
and stores the result in 
.I 'var1'.
.RE

.NH 1
Comments

.PP
Full-line comments should start with a '#' symbol.
Everything after the last SEPARATOR in the line is not interpreted,
so it can also be used as a way to comment code.

