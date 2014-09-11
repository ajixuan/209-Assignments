#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "worker.h"
#include "freq_list.h"

/* The function get_word should be added to this file */
FreqRecord *get_word(char *word, Node *head, char **filenames){

FreqRecord *freqArray = (FreqRecord *) malloc(MAXFILES * sizeof(FreqRecord));
	if (freqArray == NULL) {
			perror("Error allocating space for freqNode.");
			exit(1);
	}
	
	int fIndex = 0;
	int i;
	while(head != NULL && word != NULL){
		/*If the word is found in the linked list*/
		if(strcmp(head->word, word) == 0){
			/*Iterate through the frequency list*/
			for(i = 0; i < MAXFILES; i++){
				/*If the frequency is greater than 0 then copy into
				 * FreqRecord freq*/
				if(head->freq[i] > 0){
					freqArray[fIndex].freq = head->freq[i];
					strncpy(freqArray[fIndex].filename, filenames[i], strlen(filenames[i]));
					fIndex++;
				}
			}
		break;
		}
	head = head->next;
	}
	/*insert the ending indicator*/
	freqArray[fIndex].freq = 0;
	return freqArray;
}

/* Print to standard output the frequency records for a word.
* Used for testing.
*/
void print_freq_records(FreqRecord *frp) {
	int i = 0;
	while(frp != NULL && frp[i].freq != 0) {
		printf("%d    %s\n", frp[i].freq, frp[i].filename);
		i++;
	}
}

/* run_worker
* - load the index found in dirname
* - read a word from the file descriptor "in"
* - find the word in the index list
* - write the frequency records to the file descriptor "out"
*/
void run_worker(char *dirname, int in, int out){
	/*Build an empty filenames array*/
	char **filenames = init_filenames();
		
	/*Find the index and filenames files*/
	char *dfilenames = malloc(PATHLENGTH);
	if(dfilenames == NULL) {
		perror("filename error:");
        exit(1);
        }

	strcpy(dfilenames, dirname);
	strcat(dfilenames, "/filenames");
	
	char *dlistfile = malloc(PATHLENGTH);
	if(dlistfile == NULL) {
		perror("index error:");
        exit(1);
        }

	strcpy(dlistfile, dirname);
	strcat(dlistfile, "/index");
	
	/*Declare empty node*/
	Node **head = malloc(sizeof(Node));
	if(dfilenames == NULL) {
		perror("node declaration error:");
        exit(1);
        }
 
	/*Build the linked list*/
	read_list(dlistfile, dfilenames, head, filenames);
		
	char buff[MAXWORD];
    ssize_t reader;
    
    /*repeatedly read from in*/
    do{
		reader = read(in, buff, MAXWORD);    
		if(reader == 0){
			char *nl = strrchr(buff, '\n');
			if (nl) *nl = '\0';
			break;
			}
		else if (reader < 0){
			perror("reader error");
			exit(1);
			} 

		/*Each time a word is read pass it to get_word*/
		FreqRecord *freqArray;	
		freqArray = get_word(buff, *head, filenames);

		int i = 0;
		while(freqArray[i].freq > 0){	
		/*Write freqArray to out*/
			write(out, &(freqArray[i]), sizeof(FreqRecord));
			i++;
			}
	}while (reader > 0);	
}
