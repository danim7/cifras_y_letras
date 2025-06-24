# cifras_y_letras
Solucionador para la prueba de cifras del concurso "Cifras y letras"
Solver for the math challenge on TV contest "Cifras y letras" (FR: Des chiffres et des lettres; EN: Numbers and Letters)

# Notes
It assumes the numbers given and the target are integers that can also perfectly fit into a IEEE-754 float.

There are two macros defined by default at the beggining of the source:

INT_DIV_ONLY --> the division operation is valid only when the result is an integer

FORBID_NEG --> the substraction operation is valid only when the result is a positive integer

If these rules don't need to be enforced, they can be changed from 1 to 0 to disable these checks.

# Compiling
Ubuntu Linux:
> g++ main.cpp -o main

# Running
[executable name] [numbers given * 6] [target]
Example:
> ./main 5 8 3 25 1 1 372
