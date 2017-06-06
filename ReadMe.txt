                 CALCULATOR USAGE

Operation       |   Symbol  | Example usage | Example return     |
----------------|-----------|---------------|--------------------|
Addition        |   +       |   5 + 3       |   8                |
Subtraction     |   -       |   5 - 6       |  -1                |
Multiplication  |   * x X   |   3 * 5       |  15                |
Division        |   /       |   6 / 2       |   3                |
Power           |   ^       |   2 ^ 3       |   8                |
Root            |   v V     |   16 v 2      |   4                |
----------------|---------------------------|--------------------|
Brackets        |   ( )     |   5 * (3 + 2) |  25                |
----------------|---------------------------|--------------------|
Create variable | _varName= | _myVar = 5 + 2| myVar = 5 + 2 = 7  |
Use variable    | _varName_ | 5 + _myVar_   |  12                |
----------------|-----------|---------------|--------------------|
Use last result |   l L     |   5 + l       |  17                |
----------------|-----------|---------------|--------------------|
Show help       |   ?       |   ?           |Displays this table |
----------------|-----------|---------------|--------------------|
Print variables |   p       |   p           |Display list of vars|
----------------|-----------|---------------|--------------------|

NEGATIVE NUMBERS
Negative numbers must be encapsulated in parenthesis, eg: (-1).

VARIABLES
Reassigning a variable can be done by creating a new variable with the same name
When creating a variable the equation is stored rather than the value itself.
This means that any variable contained in another variable can be changed.
All variable names are not case sensitive.

LAST VALUE
When last result is used in a variable the value of the last result is stored
rather than the last result operation itself.
If an operator is first in the input string last result will be used as the
first operand

ERROR HANDLING
If the variable is self containing the variable will be stored and once changed
all variables will be rescanned for self containing. This is done to avoid
infinite loops.
Division by 0 will result in an inf value
0th root will result in inf value

PARENTHESIS
Any unclosed parenthesis will be closed at end of calculation
Values or variables immediately preceding or following a set of parenthesis will
be multiplied with the parenthesis.

Advanced examples
_x = 8 + 3 * (9 + 5 ^ (-2)) / 10
_y = _x_ + 25 v 2
_z = _x_ - _y_