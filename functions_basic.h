#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

int countlines(char*);
void draw_func(int,int***);
void print_to_ascii(int,int,int*,int **);
int chartoascii(int,int,char*,int**,int***);
int words_s_read(int*,int,int,int,int,int,int);
void words_s_wright(int,int*,int,int,int,int,int,int);
int *words_s_read_a(int*,int,int,int,int,int,int);
int grid_info(int*,int*,int,int ***,int **);
int *search_word_results(int,int*,int *,int **,int*,int,int,int,int,int,int *);
int search_number_results(int **,int*,int,int,int,int,int,int *);
int *search_number_results_final(int *,int *,int **,int*,int,int,int,int *);

int print_to_grid(int *,int ***,int **,int **,int,int *);
int erase_from_grid(int *,int ***,int **,int **,int,int*);
int finding_letter(int ***,int **,int);
