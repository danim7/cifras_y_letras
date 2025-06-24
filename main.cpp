//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include <limits.h>
#include <unordered_set>
#include <iostream>

#define INT_DIV_ONLY 1
#define FORBID_NEG 1

using namespace std;

int ori_cifras[6]= {5, 8, 3, 25, 1, 1};

char opr[] = {'+','-','*','/'}; 
int cifras[6]= {100, 8, 3, 5, 1, 1};
int max_cifras = sizeof(cifras) / sizeof(int);
int target = 372; //816

unordered_set<string> ya_buscados;
int hay_duplicados;

int best_diff = INT_MAX;
int best_cifras[6], best_tam;
char  best_op[5], best_pos[5];


void print_solution(char *op, char *pos, int tam) {
  int idx;
  char ult_toq[6] = {-1,-1,-1,-1,-1,-1};
  float r, res[6];
  
  for (int j = 0; j < tam; j++)
    res[j] = cifras[j];
  
  for (int i = tam-2; i >= 0; i--) {
    idx = pos[i]-'0';
    int op1=ult_toq[idx], op2=ult_toq[idx+1];
    
    switch (op[i]) {
      case '+':
        r = (op1!=-1?res[idx]:cifras[idx]) + (op2!=-1?res[idx+1]:cifras[idx+1]);
        break;
     case '-':
        r = (op1!=-1?res[idx]:cifras[idx]) - (op2!=-1?res[idx+1]:cifras[idx+1]);
        break;
     case '*':
        r = (op1!=-1?res[idx]:cifras[idx]) * (op2!=-1?res[idx+1]:cifras[idx+1]);
        break;
     case '/':
        r = (op1!=-1?res[idx]:cifras[idx]) / (op2!=-1?res[idx+1]:cifras[idx+1]);
        break;
    }
    
    ult_toq[idx]=ult_toq[idx+1] = i;

#if INT_DIV_ONLY
    printf("i %d: %.0f %c %.0f = %.0f\n", i, res[idx], op[i], res[idx+1], r);
#else
    printf("i %d: %.2f %c %.2f = %.2f\n", i, res[idx], op[i], res[idx+1], r);
#endif
    
    if (op1!=-1) {
      for (int k = 0; k < tam; k++)
        if (ult_toq[k] == op1) {
          ult_toq[k] = i;
          res[k]=r;
          }
    }
   if (op2!=-1) {
      for (int k = 0; k < tam; k++)
        if (ult_toq[k] == op2){
          ult_toq[k] = i;
          res[k]=r;
          }
    }
    res[idx]=res[idx+1]=r;
  }
   exit(0);

}


/*
El vector ult_toq indica cuando cada posición del vector cifras de resultado fue tocada por última vez.
Permite propagar un resultado anterior a operandos futuros que no sean necesariamente adyacentes.
Por ejemplo, partiendo de esta base:
cifras: 100, 8, 3, 5, 1, 1
op: "++*+*"
pos: "41032"

iteracion: tam-2 = 6-2 = 4
pos[4] = 2, op[4] = *
r = 3*5 = 15
ult_toq = -1, -1, 4, 4, -1, -1 
res = 100, 8, 15, 15, 1, 1

iteracion: 3
pos[3] = 3, op[3] = +
r = 15+1 = 16
ult_toq = -1, -1, 3, 3, 3, -1
res = 100, 8, 16, 16, 16, 1

iteracion: 2
pos[2] = 0, op[2] = *
r = 100*8 = 800
ult_toq = 2, 2, 3, 3, 3, -1
res = 800, 800, 16, 16, 16, 1

iteracion: 1
pos[1] = 1, op[1] = +
r = 800+16 = 816
ult_toq = 1, 1, 1, 1, 1, -1
res = 816, 816, 816, 816, 816, 1

iteracion: 0
pos[0] = 4, op[0] = +
r = 816+1 = 817
ult_toq = 0, 0, 0, 0, 0, 0
res = 817, 817, 817, 817, 817, 817
*/
void evalua(char *op, char *pos, int tam) {
  float res[6];
  char ult_toq[6] = {-1,-1,-1,-1,-1,-1};
 /* char pop[6] = {0}, ppos[6] = {0};
  strncpy(pop, op, tam-1);
  strncpy(ppos, pos, tam-1);*/
  
  
 /* printf("printo: %s %s\n", pop, ppos);
  if (!strcmp("+*++*", pop) && !strcmp("10432", ppos))
    printf("aaaaa\n");*/
  
  for (int j = 0; j < tam; j++)
    res[j] = cifras[j];
    
  int idx;
  float r;
  for (int i = tam-2; i >= 0; i--) {
 
    idx = pos[i]-'0';
    //printf("idx: %d, i: %d\n",idx, i);
    int op1=ult_toq[idx], op2=ult_toq[idx+1];
       // printf("op1 %d op2 %d ult_toq1: ",op1,op2);
 /*   for(int k = 0; k < 6; k++) {
     printf("%d, ", ult_toq[k]);
    }*/
    switch (op[i]) {
      case '+':
        r = (op1!=-1?res[idx]:cifras[idx]) + (op2!=-1?res[idx+1]:cifras[idx+1]);
        break;
     case '-':
        r = (op1!=-1?res[idx]:cifras[idx]) - (op2!=-1?res[idx+1]:cifras[idx+1]);
        if (FORBID_NEG && r < 0)
          return;
        break;
     case '*':
        r = (op1!=-1?res[idx]:cifras[idx]) * (op2!=-1?res[idx+1]:cifras[idx+1]);
        break;
     case '/':
        if ((op2!=-1 && res[idx+1] == 0.0) || (op2==-1 && cifras[idx+1] == 0)) {
          //printf("avoid div by zero %f / %f = %f\n", op1!=-1?res[idx]:cifras[idx], op2!=-1?res[idx+1]:cifras[idx+1], (op1!=-1?res[idx]:cifras[idx]) / (op2!=-1?res[idx+1]:cifras[idx+1]));
          return; //avoid div by 0
        }
        r = (op1!=-1?res[idx]:cifras[idx]) / (op2!=-1?res[idx+1]:cifras[idx+1]);
        if (INT_DIV_ONLY && ((float)((int)r)) != r)
          return;
        break;
    }

    
    ult_toq[idx]=ult_toq[idx+1] = i;
    
   // printf("i: %d %f\n", i, r);
    
    if (op1!=-1) {
      for (int k = 0; k < tam; k++)
        if (ult_toq[k] == op1) {
          ult_toq[k] = i;
          res[k]=r;
          }
    }
   if (op2!=-1) {
      for (int k = 0; k < tam; k++)
        if (ult_toq[k] == op2){
          ult_toq[k] = i;
          res[k]=r;
          }
    }
    /*printf("ult_toq1: ");
    for(int k = 0; k < 6; k++) {
     printf("%d, ", ult_toq[k]);
    }
    printf("\n");*/
    res[idx]=res[idx+1]=r;
  }
  
 // printf("holi %f!! pos0 %d\n", res[idx], idx);
  if (res[idx] == target) {
   // printf("found !!\n");
    print_solution(op, pos,tam);
  }
  
  if (abs(target-(int)r)<best_diff) {
    best_diff = abs(target-(int)r);
    best_tam = tam;
    for (int i = 0; i < tam-1; i++) {
      best_cifras[i] = cifras[i];
      best_op[i] = op[i];
      best_pos[i] = pos[i];
    }
    best_cifras[tam-1] = cifras[tam-1];
    if (tam < max_cifras) {
      best_op[tam-1] = 0;
      best_pos[tam-1] = 0;
    }
  }

}

/*
op: vector con operaciones a realizar, en un orden que encaja con el vector siguiente
pos: vector con la posicion de los operandos en que se realiza cada operacion. El valor 0 indica que los operandos
  son los elementos 0 y 1 del vector de cifras, el valor 1 indica que son 1 y 2, etc...
-> en los dos vectores anteriores, las operaciones empezaran a aplicarse desde el final hasta el principio.
-> los dos vectores anteriores tendran un tamaño = tamaño del vector cifras-1
curr: en que posicion de los vectores op y pos estoy trabajando ahora
tam: tamaño del vector cifras con el que estoy trabajando
*/
void itera(char *op, char *pos, char *usados, char curr, int tam) {
  if (curr >= tam-1) {
    evalua(op, pos, tam);
    return;
  }
  
  for (int i = 0; i < tam-1; i++) {
    if (usados[i])
      continue;
    for (int j = 0; j < sizeof(opr); j++) {
      op[curr] = opr[j];
      pos[curr] = i+'0';
      usados[i] = 1;
      itera(op, pos, usados, curr+1, tam);
      usados[i] = 0;
    }
  }
}

int ya_buscado(int tam) {
  char buffer[4*6+1] = {0};
  int length = 0;

  for (int i = 0; i < tam; i++) {
     length += sprintf(buffer+length, "%d,", cifras[i]);
  }
  string s(buffer);
  
  //cout << "busco en el set: " << s << endl;
  unordered_set<string>::const_iterator got = ya_buscados.find(s);

  if (ya_buscados.end() == got) {
    ya_buscados.insert(s);
    return 0;
  }
  
  else return 1;

}

/*
pos, posicion del vector de busqueda (cifras) en la que colocar la nueva cifra
conumidos[i] indica si la cifra en la posición i del vector de cifras original (ori_cifras) ya se ha usado
tam: tamaño del vector de busqueda, aka, cuantas cifras de las 6 totales voy a usar realmente para buscar el resultado
ignorados: cuantas cifras he ignorado ya en cada recursión. Al empezar ha buscar, deberan encajar el tam+ignorados = 6 cifras
*/
void reordena(int pos, char *consumidos, int tam, int ignorados) {


   if (pos == tam) {
      
     printf("buscando para: ");
     for (int i = 0; i < tam; i++) {
       printf ("%d ", cifras[i]);
     }
     printf("\n");
     
     if (max_cifras-tam!=ignorados) {
       printf("descartado, max_cifras: %d, tam: %d, ignorados: %d\n", max_cifras, tam, ignorados);
       return;
     }

     if (hay_duplicados && ya_buscado(tam)) {
       //printf("descartado por ya buscado\n");
       return;
     }
   
     char op[5] = {0};
     char pos[5] = {0};
     char usados[5] = {0};
     char curr = 0;
  
     itera(op, pos, usados, curr, tam);
     return;
   }



   for (int i = 0; i < max_cifras; i++) {
      if (consumidos[i] == 0) {
      
          if (max_cifras-tam>ignorados) {
            consumidos[i] = 1;
            reordena(pos, consumidos, tam, ignorados+1);
            consumidos[i] = 0;
          }
          
          
          consumidos[i] = 1;
          cifras[pos] = ori_cifras[i];
          reordena(pos+1, consumidos, tam, ignorados);
          consumidos[i] = 0;
          

      }
   }
}

int main(int argc, char **argv) {
 printf(" sizeof(opr) %lu\n",  sizeof(opr));
 printf(" sizeof(cifras) %d\n",  max_cifras);
 
  if (argc != 8) {
    printf("esperaba 6 cifras de busqueda y 1 cifra objetivo\n");
    exit(-1);
  }
  
  printf("%s %s %s...\n", argv[0], argv[1], argv[2]);
  
  for (int i = 1; i <= 6; i++)
    ori_cifras[i-1] = atoi(argv[i]);
  target = atoi(argv[7]);
  
  unordered_set<int> cifras_set;
  cifras_set.insert(ori_cifras[0]);cifras_set.insert(ori_cifras[1]);cifras_set.insert(ori_cifras[2]);
  cifras_set.insert(ori_cifras[3]);cifras_set.insert(ori_cifras[4]);cifras_set.insert(ori_cifras[5]);
  hay_duplicados = (cifras_set.size() == max_cifras ? 0 : 1);
  cout << "hay_duplicados: " << hay_duplicados << endl;
  hay_duplicados = 1;
  if (hay_duplicados)
     ya_buscados.reserve(2000); //calculo rapido y mal de cuantas posibles permutas de cifras hay para disitntas longitudes de tam

  char consumidos[6] = {0};
  for (int i = 2; i <= max_cifras; i++)
    reordena(0, consumidos, i, 0);
  
  printf ("EXACTO NO ENCONTRADO\n*************************\nsolucion mas proxima, diff %d\n", best_diff);
  for (int i = 0; i < 6; i++)
      cifras[i] = best_cifras[i];
  print_solution(best_op, best_pos, best_tam);

}
