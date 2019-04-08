#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "functions_basic.h"
#include "search_functions.h"



int main(int argc,char *argv[]){

	double sttime, endtime;
	sttime = ((double) clock())/CLOCKS_PER_SEC;

	int i, j, k, l, n, f=0, a, gi, wf,max_word,num_word,check=0, draw=0, dict=0, dim=0, twords=0, maxword=0, length=0;
	int maxi=0,maxl=0,maxk=0,words_inf,words_completed=0,max_place,total_words,words_completed_real=0,ls=1,calculations = 0;
	char *arg, buff[dim+1];				/*buff no more words than 100 characters*/
	int **words,*words_first,*words_s,*words_l,*buff_words,*wf_info;
	FILE *cross_file, *words_file;
	int words_info[30][30][26] = {0};
	int l_check[26] = {0};
	long long aa=0;
//	srand(time(NULL));
	int *words_put = malloc(300*sizeof(int));

	
	if ((cross_file = fopen(argv[1], "r")) == NULL) {	/*check if file given exist and set a pointer for the file*/
		fprintf(stderr, "wrong file (crossword)\n"); 
		return 1;}
	
	fscanf(cross_file,"%d",&dim);			/*finding the dimension of the crossord*/ 
	maxl = dim+1;
	
	int ***grid,**grid_words;
	
	grid_words = malloc(dim*dim*sizeof(int*));
	for(i=0;i<dim*dim;i++){
		grid_words[i] = malloc(6*sizeof(int));
		grid_words[i][0] = 0;
		grid_words[i][4] = 0;
		grid_words[i][5] = 0;}
		grid_words[0][4] = 1;
		
	grid = malloc(dim*sizeof(int**));
	for(i=0;i<dim;i++){
		grid[i] = malloc(dim*sizeof(int*));
		for(j=0;j<dim;j++){
			grid[i][j] = malloc(dim*sizeof(int));
			grid[i][j][0] = 0;
		}
	}	

	while(!feof(cross_file)){				/*fiding the spaces in crossward and place in them '#'*/
		fscanf(cross_file,"%d %d",&i,&a);
		grid[i-1][a-1][0] = '#';
	}
	max_word = grid_info(&total_words,&max_place,dim,grid,grid_words);

//	for(i=0;i<87;i++)
//		printf("i:%d d0[%d] d1i[%d] d2j[%d] d3-[%d]\n",i,grid_words[i][0],grid_words[i][1],grid_words[i][2],grid_words[i][3]);
	

	i = argc;
	while (--i) {
    	arg = argv[i];
    	if (!strcmp(arg, "-check"))   			 /*select check*/
    		check = 1;
    	if (!strcmp(arg, "-draw"))     			 /*Select draw*/
    		draw = 1;
    	if (!strcmp(arg, "-dict"))       		 /*Select dict and set index to find the file name*/
    		dict = i+1;
	}
/////////////////	printf("check[%d] draw[%d] dict[%d] words[%s]\n",check,draw,dict,argv[dict]);
	
	if (dict){
		if ((words_file = fopen(argv[dict], "r")) == NULL) {		/*check if file given exist and set a pointer for the file*/
			fprintf(stderr, "wrong file (words)\n");
			return 1;}
			twords = countlines(argv[dict]);
		}
	else{
		if ((words_file = fopen("Words.txt", "r")) == NULL) {		/*check if file default words file exist and set a pointer for the file*/
			fprintf(stderr, "wrong file (words)\n"); 
			return 1;}
			twords = countlines("Words.txt");
	}
			
	if ((words = malloc(twords * sizeof(int *))) == NULL) {		/*memmory allocation for the words matrix*/
		fprintf(stderr, "Not enough memory\n");
		return 1;}
	if ((words_first = malloc((max_word+1) * 17000 * sizeof(int))) == NULL) {		/*memmory allocation for the first words to place on the grid <=> the largest ones*/
		fprintf(stderr, "Not enough memory\n");
		return 1;}
	if ((words_l = malloc(twords * sizeof(int))) == NULL) {		/*memmory allocation for the words length matrix*/
		fprintf(stderr, "Not enough memory\n");
		return 1;}
		
	
	for(i=0;i<twords;i++){						/*read wprds from the wordlist*/
		fgets(buff,100,words_file);
		length = strlen(buff)-1;					/*length - 1 for the \n*/
		words_l[i] = length;
		if (length>maxl)
			maxl = length;
		if(length<=max_word){								
			if ((words[i] = malloc((length)* sizeof(int))) == NULL) {
				fprintf(stderr, "Not enough memory\n");
				return 2;}
			for(j=0;j<length;j++){
				words[i][j] = buff[j]; // putchar(buff[j]);
				words_info[length][j][buff[j]-'a'] = words_info[length][j][buff[j]-'a'] +1;
			//	printf("[%d](j%d)(l.%d) ",i,j,buff[j]-'a');
			}
			(words_first[(length*16000)])++;	
		}
	}
	wf_info = malloc((max_word+1)*sizeof(int));
	for(i=1;i<=max_word;i++)
		wf_info[i] = 1;
	
	/*getting information about the size of the dorectory to add later in a matrix*/
	for(i=0;i<30;i++){		
		for(j=0;j<30;j++){
			for(k=0;k<26;k++){
				if(words_info[i][j][k]>maxi){
					maxi = words_info[i][j][k]+1;		
				}
				aa = words_info[i][j][k] + aa;
			//	printf("(%lld) [%d][%d][%d] {%d} [[%d]]\n",aa,i,j,k,words_info[i][j][k],maxi);
			//	printf("%d\n",words_info[i][j][k]);
			}		
		}
	}

	/*setting a 4D matrix with  words_s[length] [position of letter] [letter] [word]*/
	
	
	if ((words_s = malloc((maxi+500) * maxl * maxl * 26 * sizeof(int))) == NULL) {
			fprintf(stderr, "Not enough memory for words search\n");
			return 2;}
	/*take words from the file and put them in the 4d matrix in the corresponding position*/
	/*words are differentiated by
	1 length
	2 letter position
	3 letter*/
	
	for(i=0;i<twords;i++){
		length = words_l[i];
		if (length<=max_word){
			words_first[(16000*length+(wf_info[length]++))] = i;
			for(j=0;j<length;j++){
				words_inf = words_info[length][j][words[i][j]-'a'];
				if(words_s_read(words_s,length,j,words[i][j]-'a',0,maxl,maxi) == 0){
					words_s_wright(words_inf,words_s,length,j,words[i][j]-'a',0,maxl,maxi);
					words_s_wright(i,words_s,length,j,words[i][j]-'a',words_inf,maxl,maxi);
					words_info[length][j][words[i][j]-'a'] = words_info[length][j][words[i][j]-'a'] - 1;
				//	printf("(%d)",i);
				}
				else{
					words_s_wright(i,words_s,length,j,words[i][j]-'a',words_inf,maxl,maxi);
					words_info[length][j][words[i][j]-'a'] = words_info[length][j][words[i][j]-'a'] - 1;
				//	printf("(%d)",l_check[words[i][j]-'a']);
				}
			}	
		}
	}


	/*single recursive function*/
	//search_rec(buff_words,&words_completed_real,&calculations,total_words,words_first,words_put,&words_completed,1,grid,grid_words,words,words_s,maxl,maxi);

if(check==0){
	/*calling multiple times the recursive searching function until a solution is found in less than 5000 compinations or 3 recuurents in the same word*/
	  ls=1;
while(search_rec(buff_words,&words_completed_real,&calculations,total_words,words_first,words_put,&words_completed,ls,grid,grid_words,words,words_s,maxl,maxi) == 3){
//	printf("\nATTEMPT %d\n",ls);
/*resseting each time the grid and the words completed to start a new searching*/

	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			if(grid[i][j][0]!='#'){
				grid[i][j][0] = 0;
				grid[i][j][3] = 0;
			}
		}
	}
	for(i=0;i<dim*dim;i++){
		grid_words[i][4] = 0;
		grid_words[i][5] = 0;
		grid_words[i][6] = 0;}
	words_completed = 0;
	calculations = 0;
	words_completed_real=0;
//	free(buff_words);
ls++;
if(ls==total_words+1)
	ls=1;
}
	/*printing the words of the grid*/
	for(i=1;i<=total_words;i++){
		l = grid_words[i][0];
		for(j=0;j<l;j++){
			if(grid_words[i][3]>0){
				putchar(grid[grid_words[i][1]] [grid_words[i][2]+j] [0]);
			}
			else{
				putchar(grid[grid_words[i][1]+j] [grid_words[i][2]] [0]);
			}
			
		}
		putchar('\n');
	}
	if(draw){
		draw_func(dim,grid);
	}
}
else{
	/*if check option enabled then check each word if it is on the directory 
	if not end the program*/
	int **checklist = malloc((total_words+2)*sizeof(int*));
	char *bufff = malloc(80*sizeof(char));
	int min=0;
	for(i=1;i<=total_words;i++){
		checklist[i] = malloc(80*sizeof(int));
		scanf("%s",bufff);
		length = strlen(bufff);

		for(j=0;j<length;j++){
			checklist[i][j] = bufff[j];
		}
		search_number_results_final(&min,words_first,words,words_s,maxl,maxi,length,checklist[i]);
		if(min<1){
			printf("Word '%s' not in directory",bufff);
			return 0;
		}
	}
	//printf("\ncorrect!!\n");
	
}

	
	endtime = ((double) clock())/CLOCKS_PER_SEC;   /* Search end time */
//	printf("Searching time is %.2f seconds\n", endtime-sttime);
}
