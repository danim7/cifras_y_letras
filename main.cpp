/*
Copyright (C) 2025 danim7 (https://github.com/danim7)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <limits.h>
#include <unordered_set>
#include <iostream>
#include <cstring>

#define ALLOW_FRACTIONAL_NUMBERS 0
#define ALLOW_NEGATIVE_NUMBERS 0
#define DEBUG 0

/*
fractional numbers testcase: ./main 1 2 25 50 7 4 582
i 3: 50.00 - 4.00 = 46.00
i 2: 2.00 / 46.00 = 0.04
i 1: 25.00 / 0.04 = 575.00
i 0: 575.00 + 7.00 = 582.00


negative numbers testcase: ./main 1 25 10 6 7 1 549
i 4: 1 - 7 = -6
i 3: 6 * -6 = -36
i 2: -36 - 25 = -61
i 1: 1 - 10 = -9
i 0: -9 * -61 = 549

*/

using namespace std;

unordered_set <string> already_searched;

char opr[] = { '+', '-', '*', '/' };

int original_numbers[6] = { -1, -1, -1, -1, -1, -1 };	//will contain numbers input by the user
int numbers[6] = { -1, -1, -1, -1, -1, -1 };	//working copy of the numbers

int numbers_length = sizeof (numbers) / sizeof (int);
int target = -1;

int best_diff = INT_MAX;
int best_numbers[6], best_length;
char best_op[5], best_pos[5];


void
print_solution (char *op, char *pos, int size) {
  int idx;
  char last_touched[6] = { -1, -1, -1, -1, -1, -1 };
  float r, res[6];

  for (int j = 0; j < size; j++)
    res[j] = numbers[j];

  for (int i = size - 2; i >= 0; i--) {
    idx = pos[i] - '0';
    int op1 = last_touched[idx], op2 = last_touched[idx + 1];

    switch (op[i]) {
    case '+':
      r = (op1 != -1 ? res[idx] : numbers[idx]) + (op2 != -1 ? res[idx + 1] : numbers[idx + 1]);
      break;
    case '-':
      r = (op1 != -1 ? res[idx] : numbers[idx]) - (op2 != -1 ? res[idx + 1] : numbers[idx + 1]);
      break;
    case '*':
      r = (op1 != -1 ? res[idx] : numbers[idx]) * (op2 != -1 ? res[idx + 1] : numbers[idx + 1]);
      break;
    case '/':
      r = (op1 != -1 ? res[idx] : numbers[idx]) / (op2 != -1 ? res[idx + 1] : numbers[idx + 1]);
      break;
    }

    last_touched[idx] = last_touched[idx + 1] = i;

#if ALLOW_FRACTIONAL_NUMBERS
    printf ("i %d: %.2f %c %.2f = %.2f\n", i, res[idx], op[i], res[idx + 1], r);
#else
    printf ("i %d: %.0f %c %.0f = %.0f\n", i, res[idx], op[i], res[idx + 1], r);
#endif

    if (op1 != -1) {
      for (int k = 0; k < size; k++)
	if (last_touched[k] == op1) {
	  last_touched[k] = i;
	  res[k] = r;
	}
    }
    if (op2 != -1) {
      for (int k = 0; k < size; k++)
	if (last_touched[k] == op2) {
	  last_touched[k] = i;
	  res[k] = r;
	}
    }
    res[idx] = res[idx + 1] = r;
  }
  exit (0);

}


/*
The vector "last_touched" is used to tell when each position in the result numbers vector was last touched.
It allows the propagation of a previous result to future operations where the input operand is not adjacent.
For example, given:
numbers: 100, 8, 3, 5, 1, 1
op: "++*+*"
pos: "41032"

iteration: size-2 = 6-2 = 4
pos[4] = 2, op[4] = *
r = 3*5 = 15
last_touched = -1, -1, 4, 4, -1, -1 
res = 100, 8, 15, 15, 1, 1

iteration: 3
pos[3] = 3, op[3] = +
r = 15+1 = 16
last_touched = -1, -1, 3, 3, 3, -1
res = 100, 8, 16, 16, 16, 1

iteration: 2
pos[2] = 0, op[2] = *
r = 100*8 = 800
last_touched = 2, 2, 3, 3, 3, -1
res = 800, 800, 16, 16, 16, 1

iteration: 1
pos[1] = 1, op[1] = +
r = 800+16 = 816
last_touched = 1, 1, 1, 1, 1, -1
res = 816, 816, 816, 816, 816, 1

iteration: 0
pos[0] = 4, op[0] = +
r = 816+1 = 817
last_touched = 0, 0, 0, 0, 0, 0
res = 817, 817, 817, 817, 817, 817
*/
void
evaluate (char *op, char *pos, int size) {
  float res[6];
  char last_touched[6] = { -1, -1, -1, -1, -1, -1 };

#if DEBUG
  char pop[6] = { 0 }, ppos[6] = { 0 };
  strncpy (pop, op, size - 1);
  strncpy (ppos, pos, size - 1);
  printf ("printo: %s %s\n", pop, ppos);
  if (!strcmp ("+*++*", pop) && !strcmp ("10432", ppos))
    printf ("aaaaa\n");
#endif

  for (int j = 0; j < size; j++)
    res[j] = numbers[j];

  int idx;
  float r;
  for (int i = size - 2; i >= 0; i--) {

    idx = pos[i] - '0';
#if DEBUG
    printf ("idx: %d, i: %d\n", idx, i);
#endif
    int op1 = last_touched[idx], op2 = last_touched[idx + 1];
#if DEBUG
    printf ("op1 %d op2 %d last_touched1: ", op1, op2);
    for (int k = 0; k < 6; k++) {
      printf ("%d, ", last_touched[k]);
    }
#endif
    switch (op[i]) {
    case '+':
      r = (op1 != -1 ? res[idx] : numbers[idx]) + (op2 != -1 ? res[idx + 1] : numbers[idx + 1]);
      break;
    case '-':
      r = (op1 != -1 ? res[idx] : numbers[idx]) - (op2 != -1 ? res[idx + 1] : numbers[idx + 1]);
#if ALLOW_NEGATIVE_NUMBERS == 0
      if (r < 0)
	return;
#endif
      break;
    case '*':
      r = (op1 != -1 ? res[idx] : numbers[idx]) * (op2 != -1 ? res[idx + 1] : numbers[idx + 1]);
      break;
    case '/':
      if ((op2 != -1 && res[idx + 1] == 0.0)  || (op2 == -1 && numbers[idx + 1] == 0)) {
#if DEBUG
	printf ("avoid div by zero %f / %f = %f\n",
		op1 != -1 ? res[idx] : numbers[idx],
		op2 != -1 ? res[idx + 1] : numbers[idx + 1],
		(op1 != -1 ? res[idx] : numbers[idx]) / (op2 != -1 ? res[idx + 1] : numbers[idx + 1]));
#endif
	return;			//avoid div by 0
      }
      r = (op1 != -1 ? res[idx] : numbers[idx]) / (op2 != -1 ? res[idx + 1] : numbers[idx + 1]);
#if ALLOW_FRACTIONAL_NUMBERS == 0
      if (((float) ((int) r)) != r)
	return;
#endif
      break;
    }


    last_touched[idx] = last_touched[idx + 1] = i;

#if DEBUG
    printf ("i: %d %f\n", i, r);
#endif

    if (op1 != -1) {
      for (int k = 0; k < size; k++)
	if (last_touched[k] == op1) {
	  last_touched[k] = i;
	  res[k] = r;
	}
    }
    if (op2 != -1) {
      for (int k = 0; k < size; k++)
	if (last_touched[k] == op2) {
	  last_touched[k] = i;
	  res[k] = r;
	}
    }
#if DEBUG
    printf ("last_touched1: ");
    for (int k = 0; k < 6; k++) {
      printf ("%d, ", last_touched[k]);
    }
    printf ("\n");
#endif
    res[idx] = res[idx + 1] = r;
  }

#if DEBUG
  printf ("holi %f!! pos0 %d\n", res[idx], idx);
#endif
  if (res[idx] == target) {
    print_solution (op, pos, size);
  }

  if (abs (target - (int) r) < best_diff) {
    best_diff = abs (target - (int) r);
    best_length = size;
    for (int i = 0; i < size - 1; i++) {
      best_numbers[i] = numbers[i];
      best_op[i] = op[i];
      best_pos[i] = pos[i];
    }
    best_numbers[size - 1] = numbers[size - 1];
    if (size < numbers_length) {
      best_op[size - 1] = 0;
      best_pos[size - 1] = 0;
    }
  }

}

/*
op: vector containing the operations to be calculated. the order matches the "pos" vector
pos: vector containing the position of operands to perform each operation. The value 0 means the operands are elements 0 and 1 
in the numbers vector, value 1 means the operands are element 1 and 2, etc...
-> in the two previous vectors, operations will be done in reverse order, starting on the end of the vector
-> both vector will have a size = size of "numbers" vector - 1
curr: in which position of op and pos vector we are working now
size: size of the numbers vector
*/
void
iterate (char *op, char *pos, char *used, char curr, int size) {
  if (curr >= size - 1) {
    evaluate (op, pos, size);
    return;
  }

  for (int i = 0; i < size - 1; i++) {
    if (used[i])
      continue;
    for (int j = 0; j < sizeof (opr); j++) {
      op[curr] = opr[j];
      pos[curr] = i + '0';
      used[i] = 1;
      iterate (op, pos, used, curr + 1, size);
      used[i] = 0;
    }
  }
}

int
is_already_searched (int size) {
  char buffer[4 * 6 + 1] = { 0 };
  int length = 0;

  for (int i = 0; i < size; i++) {
    length += sprintf (buffer + length, "%d,", numbers[i]);
  }
  string s (buffer);

#if DEBUG
  cout << "searching in set: " << s << endl;
#endif
  unordered_set < string >::const_iterator got = already_searched.find (s);

  if (already_searched.end () == got) {
    already_searched.insert (s);
    return 0;
  }

  else
    return 1;

}

/*
pos: position in the search vector "numbers" where the new number shall be placed
size: size of the search vector, ie: how many numbers form the max 6 we will actually use to search for the result
consumed[i]: it means if the number in position i from the original_numbers has already been consumed 
ignored: how many numbers have already been ignored in each recursion. At the start, size+ignored shall match 6 numbers
*/
void
rearrange (int pos, char *consumed, int size, int ignored) {


  if (pos == size) {

#if DEBUG
    printf ("buscando para: ");
    for (int i = 0; i < size; i++) {
      printf ("%d ", numbers[i]);
    }
    printf ("\n");
#endif

    if (numbers_length - size != ignored) {
#if DEBUG
      printf ("discarded, numbers_length: %d, size: %d, ignored: %d\n",
	      numbers_length, size, ignored);
#endif
      return;
    }

    if (is_already_searched (size)) {
#if DEBUG
      printf ("already searched, discarded\n");
#endif
      return;
    }

    char op[5] = { 0 };
    char pos[5] = { 0 };
    char used[5] = { 0 };
    char curr = 0;

    iterate (op, pos, used, curr, size);
    return;
  }



  for (int i = 0; i < numbers_length; i++) {
    if (consumed[i] == 0) {

      if (numbers_length - size > ignored) {
	consumed[i] = 1;
	rearrange (pos, consumed, size, ignored + 1);
	consumed[i] = 0;
      }


      consumed[i] = 1;
      numbers[pos] = original_numbers[i];
      rearrange (pos + 1, consumed, size, ignored);
      consumed[i] = 0;


    }
  }
}

int
main (int argc, char **argv) {
#if DEBUG
  printf (" sizeof(opr) %lu\n", sizeof (opr));
  printf (" sizeof(numbers) %d\n", numbers_length);
#endif

  if (argc != 8) {
    printf ("expected args are 6 given numbers and 1 target number\n");
    printf ("example: %s 4 5 25 9 7 1 771\n", argv[0]);
    exit (-1);
  }


  for (int i = 1; i <= 6; i++)
    original_numbers[i - 1] = atoi (argv[i]);
  target = atoi (argv[7]);

  already_searched.reserve (2000);

  char consumed[6] = { 0 };
  for (int i = 2; i <= numbers_length; i++)
    rearrange (0, consumed, i, 0);

  printf
    ("EXACT RESULT NOT FOUND\n*************************\nclosest solution, diff %d:\n",
     best_diff);
  for (int i = 0; i < 6; i++)
    numbers[i] = best_numbers[i];
  print_solution (best_op, best_pos, best_length);

}
