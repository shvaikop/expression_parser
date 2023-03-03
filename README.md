First line of the input has to specify the number of lines in the input that will follow.

Parser accepts: <variable_assigment> or \<expression> on each line.

Expression may contain:
- integers, doubles
- +, -, /, *, % operators. C++ operator priority holds
- parentheses (...), must be balanced
- $ symbol which loads value from a variable, e.g. x= 5, $x will return 5
- variable self assigment, variable reassignment is prohibited
- any white space is ignored

Variable assigment:
- <variable_name>= \<expression>
- "=" must follow the <variable_name> immediate with no spaces in between

Tests with expected outputs are provided in the tests directory.
