#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"
#include <unistd.h>

#define INPUT_BUFFER_SIZE 256
#define INPUT_ARG_MAX_NUM 5
#define DELIM " \r\n"


/* A standard template for error messages */
void error(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}

/* 
 * Read and process buxfer commands
 */
int process_args(int cmd_argc, char **cmd_argv, Group **group_list_addr, int fd) {
    Group *group_list = *group_list_addr; 
    Group *g;
	char *buf;

    if (cmd_argc <= 0) {
        return 0;
    } else if (strcmp(cmd_argv[0], "quit") == 0 && cmd_argc == 1) {
			buf = "Thank you for using Buxfer. Goodbye\r\n";
			if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
			return -1;
			
    } else if (strcmp(cmd_argv[0], "add_group") == 0 && cmd_argc == 2) {
           buf = add_group(group_list_addr, cmd_argv[1]);
           if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
		   return 0;
        
    } else if (strcmp(cmd_argv[0], "list_groups") == 0 && cmd_argc == 1) {
           buf = list_groups(group_list);
		   if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
		   return 0;
		           
    } else if (strcmp(cmd_argv[0], "add_user") == 0 && cmd_argc == 3) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
           buf = "Group does not exist";
           if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
		   return 0;
        } else {
            buf = add_user(group_list, cmd_argv[1]);
			if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
		    return 0;
        }
        
    } else if (strcmp(cmd_argv[0], "list_users") == 0 && cmd_argc == 2) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
            buf = "Group does not exist";
           if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
   		   return 0;
        } else {
            buf = list_users(g);
			if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
		    return 0;
        }
        
    } else if (strcmp(cmd_argv[0], "user_balance") == 0 && cmd_argc == 3) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
           buf = "Group does not exist";
           if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
		   return 0;
        } else {
            buf = user_balance(g, cmd_argv[2]);
			if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
		    return 0;      
            }
        
    } else if (strcmp(cmd_argv[0], "add_xct") == 0 && cmd_argc == 4) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
           buf = "Group does not exist";
           if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
		   return 0;
        } else {
            char *end;
            float amount = strtof(cmd_argv[3], &end);
            if (end == cmd_argv[3]) {
                buf = "Incorrect number format";
                if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
			}
				return 1;
            } else {
				buf = add_xct(g, cmd_argv[2], amount);
                if(write(fd, buf, sizeof(buf)) == -1){
				perror("write");
				}
				return 0;
                }
            }
    } else {
        error("Incorrect syntax");
    }
    return 0;
}

int buxfer(int fd, char *command, Group *group_list){
	
	/* Tokenize arguments */
	char *next_token = strtok(command, DELIM);
	int cmd_argc = 0;
	char *cmd_argv[30];
	while (next_token != NULL) {
		if (cmd_argc >= INPUT_ARG_MAX_NUM - 1) {
			error("Too many arguments!");
			cmd_argc = 0;
			break;
		}
		cmd_argv[cmd_argc] = next_token;
		cmd_argc++;
		next_token = strtok(NULL, DELIM);
	}
	cmd_argv[cmd_argc] = NULL;
	if(cmd_argc > 1 && process_args(cmd_argc, cmd_argv, &group_list, fd) == -1){
		return -1;
	}
	printf(">");
	return 0;
}

