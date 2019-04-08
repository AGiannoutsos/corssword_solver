#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include "functions_basic.h"
#include "search_functions.h"


int countlines(char *filename){
  /* count the number of lines in the file called filename*/                                    
	FILE *fp = fopen(filename,"r");
	int ch=0;
	int lines=0;
	
	if (fp == NULL)
		return 686;
	lines++;
	while(!feof(fp)){
		ch = fgetc(fp);
		if(ch == '\n')
			lines++;
	}
	fclose(fp);
	return lines;
}

/*drawing the grid letter by letter*/
void draw_func(int dim,int ***grid){
	int i,j;
	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			if (grid[i][j][0] == '#')
				printf("###");
			else{
				putchar(' ');
				putchar(grid[i][j][0]);
				putchar(' ');
			//	printf(" %d ",grid[i][j][0]);
			}
		}
		putchar('\n');
	}
}
/*print letter to asci from the file*/
void print_to_ascii(int dim,int length,int *buff,int **words){
	int i,j;
	for(i=0;i<dim;i++){
		printf("%d %d.",i,dim);
		for(j=0;j<length;j++){
			putchar(words[buff[i]][j]);
		}
		putchar('\n');
	}
}


int stringtoascii(int row,int length,char *stra,int **strb,int ***words_info){
	int i;
	int j;
	for(i=0;i<length;i++){
		j = stra[i];
		strb[row][i] = j;
		words_info[length][i][j] = words_info[length][i][j] +1;
	}
	
}
/*transforming the 1d to the 4d array and the necessary calculations*/
int words_s_read(int *words,int i,int j,int k,int l,int maxl,int maxi){
	return words[(i*maxl*26*maxi)+(j*26*maxi)+(k*maxi)+l];
}

void words_s_wright(int a,int *words,int i,int j,int k,int l,int maxl,int maxi){
	words[(i*maxl*26*maxi)+(j*26*maxi)+(k*maxi)+l] = a;
}
/*return of the pointer of the 4d array*/
int * words_s_read_a(int *words,int i,int j,int k,int l,int maxl,int maxi){
	return &words[(i*maxl*26*maxi)+(j*26*maxi)+(k*maxi)+l];
}

/*searching each horizontal and vertical line and find the available spaces for words to be completed*/
int grid_info(int *total_words,int *max_place,int dim,int ***grid,int **grid_words){
	int gi = 1;
	int i,j,max=0;
	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			if(grid[i][j][0] != '#'){
				grid_words[gi][0]++;
				grid_words[gi][1] = i;
				if(grid_words[gi][0] == 1)
					grid_words[gi][2] = j;
				grid_words[gi][3] = 1;
				grid[i][j][1] = gi;
			}
			else{
				if(grid_words[gi][0] == 1){	
					grid[i][j-1][1] = 0;	
					grid_words[gi][0] = 0;//gi--;
				}
				else if(grid_words[gi][0] > 1 )
					gi++;
				else;
			}
		}
				if(grid_words[gi][0] == 1){	
					grid[i][j-1][1] = 0;	
					grid_words[gi][0] = 0;//gi--;
				}
				else if(grid_words[gi][0] > 1 )
					gi++;
				else;
	}
	for(j=0;j<dim;j++){
		for(i=0;i<dim;i++){
			if(grid[i][j][0] != '#'){
				grid_words[gi][0]++;
				if(grid_words[gi][0] == 1)
					grid_words[gi][1] = i;
				grid_words[gi][2] = j;
				grid_words[gi][3] = -1;
				grid[i][j][2] = gi;
			}
			else{
				if(grid_words[gi][0] == 1){	
					grid[i-1][j][2] = 0;	
					grid_words[gi][0] = 0;//gi--;
				}
				else if(grid_words[gi][0] > 1 )
					gi++;
				else;
			}
		}
		if(grid_words[gi][0] == 1){	
			grid[i-1][j][2] = 0;	
			grid_words[gi][0] = 0;//gi--;
		}
		else if(grid_words[gi][0] > 1 )
			gi++;
		else;
	}
	/*finding the longest word and its position*/
		for(i=1;i<gi;i++){
		//	printf("%d: [%d] [i%d] [j%d] [o%d]\n",i,grid_words[i][0],grid_words[i][1],grid_words[i][2],grid_words[i][3]);
			if(grid_words[i][0]>max){
				*max_place = i;
				max = grid_words[i][0];
			}
		}
	*total_words = gi-1;
	return max;	
}

/*search words from the 4d array with the proper i->length j->letter position ->letter and *word_info for any additional letter*/ 
int *search_word_results(int index_temp,int *buff_words,int *min,int **words,int*words_s,int maxl,int maxi,int i,int j,int k,int *search_info){
	int n,ai,aj,ak=0,info,*indexi,*indexi_const;
//	index_temp = words_s_read(words_s,i,j,k,0,maxl,maxi);
	ak = index_temp;
/*	int *buff_words;
	if ((buff_words = malloc((index_temp+1)* sizeof(int))) == NULL) {
			fprintf(stderr, "Not enough memory\n");
			return 0;}*/
	indexi = words_s_read_a(words_s,i,j,k,1,maxl,maxi);
	buff_words[0] = -1;
	for(n=0;n<i;n++){
		if((info = search_info[n]) != 0 && n!=j){	
			ak = 0;
			for(ai=0;ai<index_temp;ai++){
				if( words[*(indexi++)][n] == info){
					buff_words[ak++] = *(indexi-1);//printf("%d [%d]\n",ak-1,buff_words[ak-1]);
				}
			}
			index_temp = ak;
			indexi = &buff_words[0];//printf("((%d))",*(indexi));
		}
	}

	*min = ak;
	if (buff_words[0] == -1)
		return indexi;
	else
		return &buff_words[0];
}

/*serching the number of words with the coresponding information about size position of letters and letters*/
int search_number_results(int **words,int*words_s,int maxl,int maxi,int i,int j,int k,int *search_info){
	int n,ai,aj,ak,info,index_temp,*indexi,*indexi_const;//printf("k%d(%d,%d).",i,j,k);
	index_temp = words_s_read(words_s,i,j,k,0,maxl,maxi);
	ak = index_temp;
	int *buff_words;
		
	if ((buff_words = malloc((index_temp+1)* sizeof(int))) == NULL) {
			fprintf(stderr, "Not enough memory\n");
			return 0;}
	indexi = words_s_read_a(words_s,i,j,k,1,maxl,maxi);
	
	for(n=0;n<i;n++){
		if((info = search_info[n]) != 0 && n!=j){	
			ak = 0;
			for(ai=0;ai<index_temp;ai++){
				if( words[*(indexi++)][n] == info){
					buff_words[ak++] = *(indexi-1);//printf("%d [%d]\n",ak-1,buff_words[ak-1]);
				}
			}
			index_temp = ak;
			indexi = &buff_words[0];//printf("((%d))",*(indexi));
		}
	}

	free(buff_words);
	return ak;
//	return &buff_words[0];
}

/*same searching function but with the ability to search without the existance of the required lettter and position of letter*/
/*it compares the *searc_info array and finds words that contain the same information as *search info does*/
int *search_number_results_final(int *min,int *words_first,int **words,int*words_s,int maxl,int maxi,int i,int *search_info){
	int n,ai,aj,ak,info,index_temp,*indexi,*indexi_const;
	int j = -1;
	int k = -1;
	int rando;
	
	for(n=0;n<i;n++){
		if(search_info[n]){
			j = n;
			k = search_info[n];
		}			
	}
	/*if search_info was no inforamtio return a word with only the information of its size*/
	if (j==-1){
		*min = words_first[(i*16000)];
		return &words_first[16000*i+1];
	}
	/*if not then do the same as *search_number_results()*/
	index_temp = words_s_read(words_s,i,j,k-'a',0,maxl,maxi);
	ak = index_temp;
	int *buff_words;
	if ((buff_words = malloc((index_temp+1)* sizeof(int))) == NULL) {
			fprintf(stderr, "Not enough memory\n");
			return 0;}
	indexi = words_s_read_a(words_s,i,j,k-'a',1,maxl,maxi);
	
	buff_words[0] = -1;
	for(n=0;n<i;n++){
		if((info = search_info[n]) != 0 && n!=j){	
			ak = 0;
			for(ai=0;ai<index_temp;ai++){
				if( words[*(indexi++)][n] == info){
					buff_words[ak++] = *(indexi-1);//printf("%d [%d]\n",ak-1,buff_words[ak-1]);
				}
			}
			index_temp = ak;
			indexi = &buff_words[0];//printf("((%d))",*(indexi));
		}
	}

	*min = ak;
	if (buff_words[0] == -1)
		return indexi;
	else
		return &buff_words[0];
}

/*print the word given to the grid*/
/*and secure the places written as non writable*/
int print_to_grid(int *words_completed_real,int ***grid,int **grid_words,int **words,int word_num,int *word){
	int i;
	int word_length = grid_words[word_num][0];
	int word_i = grid_words[word_num][1];
	int word_j = grid_words[word_num][2];
	int hori_down = grid_words[word_num][3];
	if(hori_down > 0){
		for(i=0;i<word_length;i++){
			grid[word_i][word_j+i][0] = words[*word][i];
			grid[word_i][word_j+i][3] = 1;
		/*	if (words[*word][i]>200)
		printf("fatal error\n");*/
		}
	}
	else{
		for(i=0;i<word_length;i++){
			grid[word_i+i][word_j][0] = words[*word][i];
			grid[word_i+i][word_j][3] = 1;
		/*	if (words[*word][i]>200)
		printf("fatal error\n");*/
		}
	}
	if(grid_words[word_num][4]==0){
		(*words_completed_real)++;
	}
	grid_words[word_num][4]=1;

}

/*erase words from grid*/
/*but NOT the coresponding letters with other words*/
int erase_from_grid(int *words_completed_real,int ***grid,int **grid_words,int **words,int word_num,int *info){
	int i;
	int word_length = grid_words[word_num][0];
	int word_i = grid_words[word_num][1];
	int word_j = grid_words[word_num][2];
	int hori_down = grid_words[word_num][3];
	if(hori_down > 0){
		for(i=0;i<word_length;i++){
			if(!info[i]){
				grid[word_i][word_j+i][0] = 0;/*compare the coresponding letters and avoid them*/
				grid[word_i][word_j+i][3] = 0;
			}
		}
	}
	else{
		for(i=0;i<word_length;i++){
			if(!info[i]){
				grid[word_i+i][word_j][0] = 0;
				grid[word_i+i][word_j][3] = 0;
			}
		}
	}
	grid_words[word_num][4]=0;	
	(*words_completed_real)--;
}

int finding_letter(int ***grid,int **grid_words,int dim){
	int i,j,ifin,jfin,num_word,times=0;
	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			if(!grid[i][j][0]&&times==0){
				ifin = i;
				jfin = j;
				times = 1;
			}
		}
	}
	if(num_word = grid[ifin][jfin][1])
		return num_word;
	else
		return grid[ifin][jfin][2];
}













