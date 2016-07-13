# Compilers
The purpose of this project was to create a compiler for an educational  programming language. This educational language included a lot of common commands with other programming languages, but also some prototype commands. This language included functions and procedures, pass parameters by reference and value, backward calls, nesting in functions and procedures declaration etc<b>(look at test file)</b>. This compiler is seperated in stages: lectical analysis, syntactic analysis, production of intermediate code, sysmbols table and final code.

<b>Lectical Analysis</b><br>
	The functionality of lectical analysis is to locate variables, token words, numbers, symbols and return all that to syntactical analysis.

<b>Syntactic analysis</b><br>
	Syntactic analysis is to control that the lectical unit's sequence does not brake the grammatical rules.

<b>Production of Intermediate Code</b><br>
	Production of intermediate code creates quads, which quads will be used in the production of the final code. Quads are generated when we have act, assignment, jump, comparison, function parameter, function call, start/end of the program and return value.

<b>Sysmbols Table</b><br>
	The purpose of symbols table is to save the functions, the variables and the programs parameters. We keep these infromation to known which variable we have in our program and which functions have to right to access in which variable(variable scope). 

<b>Final Code</b><br>
	The fucntionality of final code is to produce assembly language commands from the quads.
