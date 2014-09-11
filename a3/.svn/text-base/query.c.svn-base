#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "freq_list.h"
#include "worker.h"

int main(int argc, char **argv) {
	
	char ch;
	char path[PATHLENGTH];
	char *startdir = ".";

	while((ch = getopt(argc, argv, "d:")) != -1) {
		switch (ch) {
			case 'd':
			startdir = optarg;
			break;
			default:
			fprintf(stderr, "Usage: queryone [-d DIRECTORY_NAME]\n");
			exit(1);
		}
	}
	
	// Open the directory provided by the user (or current working directory)
	DIR *dirp;
	if((dirp = opendir(startdir)) == NULL) {
		perror("opendir");
		exit(1);
	} 
	
	/* For each entry in the directory, eliminate . and .., and check
	* to make sure that the entry is a directory,
	*/
	
	struct dirent *dp;	
	
	/*This while loop will actually extract the path and fork 
	 * the processes*/
	int pid;
	int fd[2][2];
	while((dp = readdir(dirp)) != NULL) {
			
		if(strcmp(dp->d_name, ".") == 0 || 
		   strcmp(dp->d_name, "..") == 0 ||
		   strcmp(dp->d_name, ".svn") == 0){
		   continue;
		}
		
		/*copies the subdirectories to the dir path*/
		strncpy(path, startdir, PATHLENGTH);
		strncat(path, "/", PATHLENGTH - strlen(path));
		strncat(path, dp->d_name, PATHLENGTH - strlen(path));
		path[PATHLENGTH-1] = '\0';
		
		struct stat sbuf;
		if(stat(path, &sbuf) == -1) {
			//This should only fail if we got the path wrong
			// or we don't have permissions on this entry.
			perror("stat");
			exit(1);
		} 
		// Only fork if it is a directory
		// Otherwise ignore it.	
		if(S_ISDIR(sbuf.st_mode)) {			
			pid = fork();	
			/*create pipes for each and every subdirectory*/
			if((pipe(fd[0])) == -1) {
				perror("pipe error");
				exit(1);
			}	
			if((pipe(fd[1])) == -1) {
				perror("pipe error");
				exit(1);
			}
			switch(pid){
				case 0: /*child*/          
					close(fd[1][0]); //Close the read for pipe 1, this will be write only
					close(fd[0][1]); //Close the write for pipe 2, this will be read only
					run_worker(path, fd[0][0], fd[1][1]);
				case -1:
					perror("fork error");
					exit(1);
				default: /*parent*/
					close(fd[0][0]); //Close the read for pipe 1, this will be only write
					close(fd[1][1]); //Close the write for pipe 2, this will be only read
			}
		}
	}
	
	/*Infinite loop that will repeatedly prompt for entries until
	 * user puts in ctrl d*/
	char word[MAXWORD];
	while(1){
		printf("Enter search word: ");
		read(STDIN_FILENO, word, MAXWORD);
		write(fd[0][1], word, MAXWORD);
		
		/*If STDIN_FILENO closes then break*/		
		if(read(STDIN_FILENO, word, MAXWORD) == EOF) {
			break;	
			}
		}
		
		/*wait for children to finish
		int i;
		for(i=0; i<pipenum; i++){
			wait(&status);
			}
		*/
		
		/*upon returning, make complete array of FreqRecordS*/			
		write(fd[1][1], STDIN_FILENO, MAXWORD);
		/*Collect the results from children*/
	
	return 0;		
}
	
		
		
		
		
		
		
		
		
		
		
		
		
		
