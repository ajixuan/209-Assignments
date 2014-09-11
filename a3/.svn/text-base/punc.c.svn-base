#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *remove_punc(char *word) {
    char *result;
    int i = 0;
    int j = 0;

    if((result = malloc(strlen(word) + 1)) == NULL) {
	perror("Out of memory");
	exit(1);
    }

    /* remove punctuation from the beginning of the word */
    while(ispunct(word[i])) {
	i++;
    }

    while(word[i] != '\0') {

	result[j] = tolower(word[i]);
	j++;
	i++;
    }
    result[j] = '\0';

    /* remove punctuation from the end of the word */
    i = strlen(result) - 1;
    while((ispunct(result[i]) || isspace(result[i]))&& i >= 0) {
	result[i] = '\0';
	i--;
    }
    return result;
}
