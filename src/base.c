/*=============================================================
*              UNIFAL = Universidade Federal de Alfenas.
*               BACHARELADO EM CIENCIA DA COMPUTACAO.
* Trabalho...: Contagem de feijoes
* Professor..: Luiz Eduardo da Silva
* Aluno......: Marcos Vyctor Fonseca Galupo
* Data.......: 20/05/2024
*=============================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#include "..\\utils\\imagelib.h"
#elif defined(__linux__)
#include "../utils/imagelib.h"
#endif

#define INT_MAX __INT_MAX__
#define min(a, b) ((a) < (b) ? (a) : (b))


int min_of_3(int x, int y, int z) {
    if (x < y && x < z) return x;
    if (y < z) return y;

    return z;
}

int min2(int a, int b) {
    if (a < b){
        return a;
    }
    return b;
}

image intensity(image In){
    float T[In->ml + 1];

    image Out = img_clone(In);
    for (int i = 0; i < In->ml + 1; i++)
        T[i] = i > In->ml/2 ? 0 : 1;  // limiarização

    for (int i = 0; i < In->nr * In->nc; i++)
        Out->px[i] = T[In->px[i]];
    
    return Out;
}

image neg_pgm(image In)
{
    image Out = img_clone(In);
    for (int i = 0; i < In->nr * In->nc; i++)
        Out->px[i] = In->ml - In->px[i];
    return Out;
}

/* find the parent of the group the element belongs to
 * @param parent store the parent of a group
 * @param e element 
*/
int find(int parent[], int e)
{
    while (parent[e] != e)
        e = parent[e];
    return e;
}

/* join two components into a union structure
* @param parent store the parent of a group
* @param i parent to be joined
* @param j parent to be joined
*/
void Union(int parent[], int i, int j)
{
    int x = find(parent, i);
    int y = find(parent, j);
    parent[y] = x;
}


int countDifferentLabels(image In, int parent[]) {
     int unique = 0;
     int used[In->nc * In->nr];

    for(int i = 0; i < In->nc * In->nr; i++)
        used[i] = 0;

    for (int i = 0; i < In->nc * In->nr; i++) {
        int root = find(parent, In->px[i]);
        if (used[root] == 0) { // not used
            used[root] = true;
            unique++;
        }
    }
    return unique;
}

int distance(image In) {
    int nr = In->nr;
    int nc = In->nc;
    int *px = In->px;
    int max_distance = -1;

    for (int i = 1; i < nr - 1; i++) {
        for (int j = 1; j < nc - 1; j++) {
            int pos = i * nc + j;
            int cur = px[pos];

            //neighbors
            int up = px[(i - 1) * nc + j];
            int esq = px[i * nc + j - 1];

            if (cur != 0) {
                px[pos] = (up + 1) < (esq + 1) ? (up + 1) : (esq + 1); // Calcular a distância mínima
            }
        }
    }

    for (int i = nr - 2; i > 0; i--) {
        for (int j = nc - 2; j > 0; j--) {
            int pos = i * nc + j;
            int cur = px[pos];

            //neighbors
            int up = px[(i + 1) * nc + j];
            int esq = px[i * nc + j + 1];

            if (cur != 0) {
                px[pos] = min_of_3(up+1, esq+1, cur); // Atualizar com a distância mínima
                if (max_distance < px[pos])
                    max_distance = px[pos]; // Atualizar a distância máxima encontrada
            }
        }
    }
    return max_distance;
}

int label(image In)
{
    int nr = In->nr;
    int nc = In->nc;
    int *p = In->px;

    int label = 0;
    int parent[1000];

    for (int i = 0; i < 1000; i++)
        parent[i] = i;
    for (int i = 1; i < nr; i++)
        for (int j = 1; j < nc; j++)
        {
            int pos = i * nc + j;

            int cur = p[pos]; // current pixel
            int up = p[(i - 1) * nc + j]; // up neighbor
            int left = p[pos - 1]; // left neighbor

            if (cur != 0)
            {
                if (up == 0 && left == 0) p[pos] = ++label;
                if (up != 0 && left == 0) p[pos] = up;
                if (up == 0 && left != 0) p[pos] = left;
                if (up != 0 && left != 0 && left == up) p[pos] = up;
                if (up != 0 && left != 0 && left != up){
                    p[pos] = left;
                    Union(parent, up, left);
                }
            }
        }
        
    for (int i = 0; i < nr * nc; i++)
        p[i] = find(parent, p[i]);
    return countDifferentLabels(In, parent);
}

void msg(char *s)
{
    printf("\nNegative image");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  image-name[.pgm]\n\n", s);
    printf("    image-name[.pgm] is image file in pgm format \n\n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int nc, nr, ml, tp;
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    if (argc < 2)
        msg(argv[0]);

    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

    //-- read image
    In = img_get(nameIn, GRAY);

/*-------------------------------------------------------------------------
 * Image transformations
 *-------------------------------------------------------------------------*/
    //Out = neg_pgm(In); // -- image negativity

    Out = intensity(In); // -- image thresholding
    Out->ml = distance(Out);
    //int labels = label(Out);


    img_put(Out, nameOut, GRAY); //-- save image

    //printf("#componentes = %i\n", labels); // -- show number of components in terminal

/*-------------------------------------------------------------------------
 * Showing image after transformations
 *-------------------------------------------------------------------------*/
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    puts(cmd);
    system(cmd);

    // -- free images
    img_free(In);
    img_free(Out);
    return 0;
}
