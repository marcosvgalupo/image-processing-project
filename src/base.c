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

#define bool _Bool

#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#include "..\\utils\\imagelib.h"
#elif defined(__linux__)
#include "../utils/imagelib.h"
#endif


/* apply threshold in image
* @param In input image
* @return thresholded image
*/
image first_threshold(image In){
    float T[In->ml + 1];

    image Out = img_clone(In);
    for (int i = 0; i < In->ml + 1; i++)
        T[i] = i > 101 ? 0 : 1;  // limiarização

    for (int i = 0; i < In->nr * In->nc; i++)
        Out->px[i] = T[In->px[i]];
    
    return Out;
}

/* apply threshold in image
* @param In input image
* @return thresholded image
*/
image second_threshold(image In){
    float T[In->ml + 1];

    image Out = img_clone(In);
    for (int i = 0; i < In->ml + 1; i++)
        T[i] = i > 5 ? 1 : 0;  // limiarização

    for (int i = 0; i < In->nr * In->nc; i++)
        Out->px[i] = T[In->px[i]];
    
    return Out;
}


/* function to find the minimum between two values
* @param a number
* @param b number
* @return minimum between a and b
*/
int min(int a, int b) {
    if (a < b) return a;
    return b;
}

/* calculate distance according to pixel neighbors
 * @param In input image
*/
void distance(image In) {
    int nr = In->nr;
    int nc = In->nc;
    int *px = In->px;
    int max_distance = 0;

    for (int i = 1; i < nr - 1; i++) {
        for (int j = 1; j < nc - 1; j++) {
            int pos = i * nc + j;
            int cur = px[pos];

            //neighbors
            int up = px[(i - 1) * nc + j];
            int esq = px[i * nc + j - 1];

            if (cur != 0) {
                px[pos] = min(up + 1, esq + 1); // Calcular a distância mínima
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
                int min_ = min(up + 1, esq + 1);
                px[pos] = (cur < min_) ? cur : min_; // Atualizar com a distância mínima
                if (max_distance < px[pos])
                    max_distance = px[pos]; // Atualizar a distância máxima encontrada
            }
        }
    }
    In->ml = max_distance;
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

/* aux function to label count the different labels (number of connected components)
 * @param In input image
 * @param parent array of parents (manage the relations between the pixels)
 * @return number of connected components
*/
int countDifLabels(image In, int parent[]) {
     int unique = 0;
     bool used[1000];

    for(int i = 0; i < 1000; i++)
        used[i] = false;

    for (int i = 0; i < In->nc * In->nr; i++) {
        int root = find(parent, In->px[i]);
        if (used[root] == 0 && root != 0) { // not used and not a background pixel
            used[root] = true;
            unique++;
        }
    }
    return unique;
}

/* labels the image in order to find the connected components
* @param In input image
* @return 
*/
int label(image In) {
    int nr = In->nr;
    int nc = In->nc;
    int *p = In->px;

    int label = 0;
    int parent[1000];

    for (int i = 0; i < 1000; i++)
        parent[i] = i;
    for (int i = 1; i < nr; i++)
        for (int j = 1; j < nc; j++){
            int pos = i * nc + j;

            int cur = p[pos]; // current pixel
            int up = p[(i - 1) * nc + j]; // up neighbor
            int left = p[pos - 1]; // left neighbor

            if (cur != 0)
            {
                if (up == 0 && left == 0) {
                    p[pos] = ++label;
                }
                if (up != 0 && left == 0) p[pos] = up;
                if (up == 0 && left != 0) p[pos] = left;
                if (up != 0 && left != 0 && left == up) p[pos] = up;
                if (up != 0 && left != 0 && left != up){
                    p[pos] = left;
                    Union(parent, up, left);
                }
            }
        }
        
    for (int i = 0; i < nr * nc; i++){
        if(In->px[i] != 0)
            In->px[i] = find(parent, In->px[i]);
    }

    In->ml = label;
    return countDifLabels(In, parent);
}

void msg(){
    printf("\nBeans Counter -- Marcos Vyctor Fonseca Galupo");
    printf("\n-------------------------------");
    printf("\nUsage:  ./contafeijao  image-name[.pgm]\n\n");
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
        msg();

//  * Define input/output file name
    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

//  * Read image
    In = img_get(nameIn, GRAY);


//  * Image transformations
    Out = first_threshold(In);
    distance(Out);
    Out = second_threshold(Out);
    int labels = label(Out);


//  * Saving and showing image
    img_put(Out, nameOut, GRAY);
    printf("\n\nImage File Name: %s", argv[1]);
    printf("\n#componentes= %i\n\n", labels);


 // * Showing image after transformations
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    puts(cmd);
    system(cmd);

//  * Free images
    img_free(In);
    img_free(Out);
    return 0;
}
