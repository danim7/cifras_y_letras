# cifras_y_letras
Solucionador para la prueba de cifras del concurso "Cifras y letras"

Solver for the math challenge on TV contest "Cifras y letras" (FR: Des chiffres et des lettres; EN: Numbers and Letters)

# Notes
It assumes the numbers given and the target are integers that can also perfectly fit into a IEEE-754 float.

There are two macros defined at the beggining of the source:

ALLOW_FRACTIONAL_NUMBERS 0 --> the division operation is valid only when the result is an integer, change to 1 to allow fractional results

ALLOW_NEGATIVE_NUMBERS 0 --> the substraction operation is valid only when the result is a positive integer, change to 1 to allow negative numbers


# Compiling
Ubuntu Linux:
>  g++ main.cpp -o main

# Running
[executable name] [numbers given * 6] [target]

Example:
>  ./main   5   8   3   25   1   1   372

Output:
> i 2: 5 * 25 = 125
> 
> i 1: 125 - 1 = 124
> 
> i 0: 124 * 3 = 372

