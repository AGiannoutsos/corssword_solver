#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include "functions_basic.h"
#include "search_functions.h"

/*the recursive searching function*/
int search_rec(int *buff_words,int *words_completed_real,int *calculations,int total_words,int *words_first,int *words_put,int *words_completed,int word_num,int ***grid,int **grid_words,int **words,int *words_s,int maxl,int maxi){
	((*words_completed)++);
	/*the recursive stops after 5000 calculations or... after 5 times in the same word later..*/
	int aaa=0;
	if(*calculations >= 5000){
		if(aaa==1){
			//	printf("\nFREEE");
				free(buff_words);}
			return 3;
			
		}

//	printf(".%d.",*words_completed_real);
	int l,i,j,k,letter,a=maxi+10,n,min=maxi+10,mini=0,prev;
	int word_length = grid_words[word_num][0];
	int word_i = grid_words[word_num][1];
	int word_j = grid_words[word_num][2];
	int hori_down = grid_words[word_num][3];
	int *search_info = malloc(30*sizeof(int));
	int grid_word,grid_word_length,grid_word_i,grid_word_j;
	int *word_list;
	int rando,rando_index=0;
	int return_value;
	



if(*words_completed!=1)	{
	/*extracting from the grid the right information to complete the words*/
	/*the information is put inside the *info_word that was the letter and its position in the grid*/
	for(i=0;i<word_length;i++){
		prev = 0;
		if(hori_down > 0){			
			letter = grid[word_i][word_j+i][0] - 'a';
			grid_word = grid[word_i][word_j+i][2];
			grid_word_length = grid_words[grid_word][0];
			grid_word_i = grid_words[grid_word][1];
			grid_word_j = grid_words[grid_word][2];
			for(k=0;k<grid_word_length;k++){
				search_info[k] = grid[grid_word_i+k][grid_word_j][0];
				prev = prev + grid[grid_word_i+k][grid_word_j][3];
			}
			j = word_i - grid_word_i; 
		}
		else{			
			letter = grid[word_i+i][word_j][0] - 'a';
			grid_word = grid[word_i+i][word_j][1];
			grid_word_length = grid_words[grid_word][0];
			grid_word_i = grid_words[grid_word][1];
			grid_word_j = grid_words[grid_word][2];
			for(k=0;k<grid_word_length;k++){
				search_info[k] = grid[grid_word_i][grid_word_j+k][0];
				prev = prev + grid[grid_word_i][grid_word_j+k][3];
			}
			j = word_j - grid_word_j;
		}
			
		/*then there are caclculated the possible compinations of its pisition*/
		if(grid_word_length)
			a = search_number_results(words,words_s,maxl,maxi,grid_word_length,j,letter,search_info);
	
		if(grid_word_length && prev == grid_word_length){
			if(a!=1){
				free(search_info);
				return 0;
			}
			a=maxi+10;		
		}
		else{/*the position with the least compinations is selected to be filled*/
			if(a<min && a!=0){
				min = a;
				mini = i;
			}
		}/*if there are no vompinations then there is a dead end and the recurent has to return to the previous state to chose another word*/
		if(a == 0){
			free(search_info);
			return 0;
		}		
	}
}
/*if the there are not any intersected words to be filled and the crossword is partially completed
then it searches for an empty word to continue its searching*/
if(min==maxi+10 && *words_completed!=1){
	/*finding the next empty word to fill*/	
	i=1;
	while(grid_words[i][4]==1)
		i++;	
		
		
	grid_words[i][5]++;
//	printf("move to {%d}.times-> (%d).// >[%d]<",i%(total_words+1),grid_words[i][5],*calculations);
/*if the program returns to this word over 5 times then it shuts down the recursive function and starts from the begin*/
	if(grid_words[i][5]>5){
		if(aaa==1){
			//	printf("\nFREEE");
				free(buff_words);}
		return 3;
	}
		
	word_length = grid_words[i][0];
	word_i = grid_words[i][1];
	word_j = grid_words[i][2];
	hori_down = grid_words[i][3];

}
	/*initializing for one more time the ideal *search_info array with the least compinations available*/ 
	i=mini;
	if(*words_completed==1 || min==maxi+10){
		hori_down = hori_down*-1;
		i=0;
	}
		

		if(hori_down > 0){
			letter = grid[word_i][word_j+i][0] - 'a';
			grid_word = grid[word_i][word_j+i][2];//printf("grid word (%d)",grid_word);
			grid_word_length = grid_words[grid_word][0];
			grid_word_i = grid_words[grid_word][1];
			grid_word_j = grid_words[grid_word][2];
			for(k=0;k<grid_word_length;k++){
				search_info[k] = grid[grid_word_i+k][grid_word_j][0];
			}
			j = word_i - grid_word_i; 
		}
		else{
			letter = grid[word_i+i][word_j][0] - 'a';
			grid_word = grid[word_i+i][word_j][1];
			grid_word_length = grid_words[grid_word][0];
			grid_word_i = grid_words[grid_word][1];
			grid_word_j = grid_words[grid_word][2];
			for(k=0;k<grid_word_length;k++){
				search_info[k] = grid[grid_word_i][grid_word_j+k][0];
			}
			j = word_j - grid_word_j;
		}
		
	/*and regarding if it is the first word to be completed or not, selecting the searching function*/
		if(*words_completed==1){
			
			do{word_list = search_number_results_final(&min,words_first,words,words_s,maxl,maxi,grid_word_length,search_info);
			rando_index = (double)  min*rand()/RAND_MAX;
			min = min - rando_index;
		//	printf("min= %d.",min);
			}while(min<2);
			//rando_index =1;		
		}
		else if(min==maxi+10){
			do{word_list = search_number_results_final(&min,words_first,words,words_s,maxl,maxi,grid_word_length,search_info);
			rando_index = (double)  min*rand()/RAND_MAX;
			min = min - rando_index;
		//	printf("min= %d.",min);
			}while(min<1);
		}
		else{
			aaa=1;
			int index_temp;
			index_temp = words_s_read(words_s,grid_word_length,j,letter,0,maxl,maxi);
			if ((buff_words = malloc((index_temp+1)* sizeof(int))) == NULL) {
				fprintf(stderr, "Not enough memory\n");
				return 0;}
			word_list = search_word_results(index_temp,buff_words,&min,words,words_s,maxl,maxi,grid_word_length,j,letter,search_info);
			rando_index = (double)  min*rand()/RAND_MAX;
			min = min - rando_index;
		}
	/*storing in a pointer word_list the array of the available words and 
	selecting a random word*/
	/*	rando_index = (double)  min*rand()/RAND_MAX;*/
		

		print_to_grid(words_completed_real,grid,grid_words,words,grid_word,word_list+rando_index);
		words_put[*words_completed] = grid_word;

	/*if all words are filled it ends the recursion successfully*/
	if(*words_completed_real >= total_words)
		return 1;
	

	
	/*for the number of the available words*/
	for(i=0;i<min;i++){
		((*calculations)++);
		if(*calculations >= 5000){
			if(aaa==1){
			//	printf("\nFREEE");
				free(buff_words);}
			return 3;
		}
			
		return_value = search_rec(buff_words,words_completed_real,calculations,total_words,words_first,words_put,words_completed,grid_word,grid,grid_words,words,words_s,maxl,maxi);
		/*if there is no other compination
		it selects another available word*/	
		if(	return_value==0){
			((*words_completed)--);

			print_to_grid(words_completed_real,grid,grid_words,words,grid_word,word_list++);
			words_put[*words_completed] = grid_word;
		}
		else if(return_value ==1){
			return 1;}
		else{
			free (search_info);
		//	free (word_list);
			if(aaa==1){
			//	printf("\nFREEE");
				free(buff_words);}
			return 3;
		}
	}
	/*if the available words are all used with no success it returns to the previous state*/
	
	erase_from_grid(words_completed_real,grid,grid_words,words,grid_word,search_info);
	free(search_info);
if(aaa==1){
			//	printf("\nFREEE");
				free(buff_words);}
//	free(word_list);
	return 0;

}


