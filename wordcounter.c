/*
*       FILENAME: wordcounter.c | Made by Jouni Lammi, 2018 | MIT License | 3rd party sources given in the comments
*       This program takes a .txt text file and outputs the following things:
*           - the total amount of words in the file
*           - the amount of different words in file
*           - 100 most common words and their occurrence times
*           - Time (in seconds) needed to count the words after the filename is given
*       The program asks for the filename when run
*       Give the parameters with which you wish the program to separate words on line 25
*       Check that the .txt file is in the same directory than this program before running the program

		THE MIT LICENSE:
		Copyright (c) 2018 Jouni Lammi

		Permission is hereby granted, free of charge, to any person obtaining
		a copy of this software and associated documentation files (the
		"Software"), to deal in the Software without restriction, including
		without limitation the rights to use, copy, modify, merge, publish,
		distribute, sublicense, and/or sell copies of the Software, and to
		permit persons to whom the Software is furnished to do so, subject to
		the following conditions:

		The above copyright notice and this permission notice shall be included
		in all copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
		EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
		MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
		IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
		CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
		TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
		SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

// DEFINE STRUCT //
typedef struct{  //defining struct
    char singleWord[100];
    int occurrenceTimes;
    } oneWordStruct;

// GLOBAL VARIABLES //	
FILE *file;
char s[] = "§½£#¤&´\%%%@$013456789+ —–-|/\"\\()[]{}<>,:;.=_?*!\n\t"; //the list of characters that the words are separated with
int n = 0;  //gives the last index of words[] that contains information and at the same time gives the amount of different words in the file
int k = 0;  //used as a loop variable to go through
int firstTimeFLAG = 0;  //Flag to whether the first loop cycle has been gone through or not
int ifInListFLAG = 0;   //checker to whether the word is in the list or not
int howManyWords = 0;   //gives the amount of all detected words in the file
clock_t start = NULL;   //initialize clock start for easier use inside read_file()
clock_t end = NULL; //initialize clock end for easier use inside main()

//INITIALIZE STRUCT ARRAY //
oneWordStruct words[1000000];  //create a list with a million elements, each of which is a oneWordStruct


char *convert_to_lower(char *str){   //function with which the buffer str is made lowercase, used https://stackoverflow.com/questions/41148978/how-to-use-tolower-with-char

    unsigned char *mystr = (unsigned char *)str;
    while (*mystr) {
        *mystr = tolower(*mystr);
        mystr++;
    }
    return str;
}


int compare(oneWordStruct *a, oneWordStruct *b){ // function that qsort uses to determine whether the occurrence times of a or b is bigger, used https://stackoverflow.com/questions/6105513/need-help-using-qsort-with-an-array-of-structs
    if(a->occurrenceTimes < b->occurrenceTimes){
        return +1; //if a occurs less times than b, return +1
    }
    else if(a->occurrenceTimes > b->occurrenceTimes){
       return -1; //if a occurs more times than b, return -1
    }
    return 0;  //if a's and b's occurrence times are equal, return 0
}


void sorting(){  // function that sorts the words list with qsort and the function compoare() by the words' occurrence times from most to least
    qsort((void *)words, n, sizeof(oneWordStruct), (int(*)(const void *,const void *)) compare); // used https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
}


FILE read_file() {  // asks for opens the file for reading and returns 404 if the file reading fails, contains information from http://www.zentut.com/c-tutorial/c-read-text-file/
                    //and https://stackoverflow.com/questions/2795382/enter-custom-file-name-to-be-read
    char fileName[128]; //buffer for the file name
    printf("Please enter the full name of the textfile: ");
    scanf("%123s",fileName); //takes the user input
    printf("\n");

    start = clock(); //start measuring time
    file = fopen(fileName, "r"); //opens file for reading
    if (file == NULL){
        printf("Could not open file %s, please check the filename and rerun the program",fileName);
        exit(404);
    }
}


void handle_file(){     // reads the text file to the buffer str, makes it lowercase with convert_to_lower,
    char str[1000000];   //copies the result to str2, reads single words from str2 with strtok, stores words in the words list and adds occurrence times
    char str2[1000000];
        while(fgets(str, sizeof(str), file) != NULL){ //while loop to go through the whole text file, for a single cycle reads until reaches newline character or end of file.
                                                        //on the upper line used https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm
            sprintf(str2, convert_to_lower(str));  // copying the lowercase buffer to str2
            char *token = strtok(str2, s);  //strtok function is used to go through the buffer and separate words with the parameters given in the char s[] at line 25, used https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
                                            //gets the first token

            while( token != NULL ) {  //needed to get the rest of the tokens
                    for(k=0; k<n; k++){  //for loop to go through words[] and determine whether to add the word to list or add occurrence times
                        if(firstTimeFLAG = 0){ //checking if this loop cycle is the first one
                            ifInListFLAG = 0;
                            firstTimeFLAG = 1;  //firstTimeFLAG raised and the if loop will never be gone through again
                            break; //break to while( token != NULL ) loop
                        }
                        if(strcmp(words[k].singleWord,token) == 0){ //compares the word in words[k] and the token, if the word is in the list, raises flag
                            ifInListFLAG = 1;
                            break;
                        }

                        else{
                            ifInListFLAG = 0; //if the word is not in the list, no flag is raised
                        }
                    } //for

                    if(ifInListFLAG == 1){  //if the word is already in words[] add one to its occurrence times
                        words[k].occurrenceTimes++;
                    }

                    if(ifInListFLAG == 0){ //if the word wasn't inwords[], add it to the list and raise its occurrence times
                        strcpy(words[n].singleWord, token);
                        words[n].occurrenceTimes++;
                        n++;
                    }


                howManyWords += 1;  //raise total word count before next cycle
                token = strtok(NULL, s); //null token to start over the loop

            }

        }//if


}



void print_info(){
	printf("%d words in file \n",howManyWords);
    printf("%d different words in file \n",n);
    if(n<100){
        for(int j=0;j<n;j++){
            if(strlen(words[j].singleWord) > 7){  //to get the tabs work correctly
                printf("%s\t %d\n",words[j].singleWord,words[j].occurrenceTimes);
            }
            else{
                printf("%s\t\t %d\n",words[j].singleWord,words[j].occurrenceTimes);
            }
     	 }
	
    float seconds = (float)(end - start) / CLOCKS_PER_SEC; //calculate difference and convert to seconds, used https://stackoverflow.com/questions/3557221/how-do-i-measure-time-in-c
    printf("Running the program took %.3f seconds", seconds);	 
    }

    else{
        for(int j=0;j<100;j++){
            if(strlen(words[j].singleWord) > 7){ //to get the tabs work correctly
                printf("%s\t %d\n",words[j].singleWord,words[j].occurrenceTimes);
            }
            else{
                printf("%s\t\t %d\n",words[j].singleWord,words[j].occurrenceTimes);
            }
		
        }

    float seconds = (float)(end - start) / CLOCKS_PER_SEC; //calculate difference and convert to seconds, used https://stackoverflow.com/questions/3557221/how-do-i-measure-time-in-c
    printf("Running the program took %.3f seconds", seconds);
    }

}

void main(){
    read_file();
    handle_file(&file);
    sorting();
	
	end = clock(); //end time measuring
    fclose(file);
	
    print_info();
	
}

		