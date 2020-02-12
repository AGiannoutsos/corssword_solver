# corssword_solver
A small program in C to solve random crosswords with a given wordlist  
This was my 1st year at university project in Intriduction of Programming


the program can solve the crossword puzzles: Crossword1,2,3,4,5   
with Words.txt MoreWords.txt KnuthWorsd.txt wordlists 

### Running Commands
example for crossword5 and dictionary KnuthWords

```exemain Crossword5.txt -dict KnuthWords.txt -draw```

### About the algorithm
The program reads the words from the given dictionary and classifies the words into a 4-dimensional table based on  
1st First word length  
2nd Position of the letter  
3rd Letter 
4th The number of the word  


Then search and logging of words is done through the search_rec()  
This function searches through a given word for the words crossed with it and completes the word which has the fewest combinations.  
Each time a vertical and a horizontal word are alternately completed until the crossword is complete.  
Then the reccurent function is recalled if all the words in the crossword have been completed either if 5000 searches have been made or more than 5 reflexes in the same word are made without a solution.  
if there is no solution from the recall, it is called again until a suitable combination of words resolving the crossword is accidentally found.  
