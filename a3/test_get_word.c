#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "freq_list.h"
#include "worker.h"
FreqRecord *get_word(char *word, Node *head, char **filenames);

int main(){
	char *filenames[3] = {"sounds", "shapes", "blocks"};	
	Node *header = create_node("bloop", 3, 0);
	
	/*
	Node *node1 = add_word(header, filenames, "poop", "sounds");
	Node *node2 = add_word(node1, filenames, "poop", "sounds");
	Node *node3 = add_word(node2, filenames, "poop", "sounds");

	Node *node4 = add_word(node3, filenames, "triangle", "shapes");
	Node *node5 = add_word(node4, filenames, "triangle", "shapes");
	Node *node6 = add_word(node5, filenames, "triangle", "shapes");
	
	Node *node7 = add_word(node6, filenames, "concrete", "blocks");
	Node *node8 = add_word(node7, filenames, "concrete", "blocks");
	Node *node9 = add_word(node8, filenames, "concrete", "blocks");*/
	
	display_list(header, filenames);
	FreqRecord *record = (FreqRecord *) get_word("bloop", header, filenames);
	print_freq_records(record);
	
	Node *nothing = NULL;
	FreqRecord *nada = (FreqRecord *) get_word("bloop", nothing, filenames);
	print_freq_records(nada);
	
	return 0;
}
