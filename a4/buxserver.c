#include "lists.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXNAME 100
#define MAXLINE 256

#ifndef PORT
	#define PORT x 
#endif

struct client{
	int fd;
	struct in_addr ipaddr;
    struct client *next;
	char buf[MAXLINE];
	char name[MAXNAME];
	char command[50];
	int curpos;
	}*top = NULL;

struct account{
	char *username;
	struct account *next;
	}*header = NULL;


typedef struct account Account;
typedef struct client Client;

static int listenfd;

static void addclient(int fd, struct in_addr addr);
static void removeclient(int fd);


char *readfromclient(Client *c){
	char *startptr = &c->buf[c->curpos];
	int len = read(c->fd, startptr, MAXLINE - c->curpos);
	do {
		if(len <= 0){
			if (len == -1){
				perror("read on socket");
				}
		}else {
			c->curpos += len;
			c->buf[c->curpos] = '\0';
			char *leftover = strchr(c->buf, '\n');
			if(strchr(c->buf, '\n') || strchr(c->buf, '\r')){
				strncat(c->command, c->buf, 
				(strlen(c->buf) - (strlen(leftover) + 1)));
				memmove(c->buf, leftover, c->curpos);
				c->curpos = 0;
			}else{
				return c->command;
				}
			}
	} while(strchr(c->buf, '\n') || strchr(c->buf, '\r'));
	return c->command;
}



/*Add a new account to the linkedlist of usernames in the system*/
void addaccount(char *name)
{
	Account *p = malloc(sizeof(Account));
    if (!p) {
		fprintf(stderr, "out of memory!\n");  /* highly unlikely to happen */
		exit(1);
    }
		printf("Adding account %s\n", name);
		fflush(stdout);
		p->username = name;
		p->next = header;
		header = p;
}

static void addclient(int fd, struct in_addr addr)
{
    Client *p = malloc(sizeof(Client));
    if (!p) {
		fprintf(stderr, "out of memory!\n");  /* highly unlikely to happen */
		exit(1);
    }
		printf("Adding client %s\n", inet_ntoa(addr));
		fflush(stdout);
		p->fd = fd;
		p->ipaddr = addr;
		p->next = top;
		top = p;
}

static void removeclient(int fd)
{
    Client **p;
    for (p = &top; *p && (*p)->fd != fd; p = &(*p)->next)
		;
    if (*p) {
	Client *t = (*p)->next;
	printf("Removing client %s\n", inet_ntoa((*p)->ipaddr));
	fflush(stdout);
	free(*p);
	*p = t;
    } else {
	fprintf(stderr, "Trying to remove fd %d, but I don't know about it\n", fd);
	fflush(stderr);
    }
}

void newconnection(void){
		int fd;
		Account *p;
		int overtime;
		char *username;
		struct sockaddr_in r;
		socklen_t socklen = sizeof r;
		char msgbuf[MAXLINE];
		
		/*Once a client is connected*/
		if ((fd = accept(listenfd, (struct sockaddr *)&r, &socklen)) < 0) {
			perror("accept");
			} else {
			sprintf(msgbuf, "Welcome to Buxfer! Please enter Username: \r\n");
			if(write(fd, msgbuf, sizeof(msgbuf)) == -1){
				perror("write");
				exit(1);
				}
			fd_set fdlist;
			struct timeval tv;
			FD_ZERO(&fdlist);
			FD_SET(fd, &fdlist);
			tv.tv_sec = 20;
			tv.tv_usec = 0;

			if (select(fd+1, &fdlist, NULL, NULL, &tv) != 1) {
				overtime = -1;
			}
			else if ((len = read(fd, msgbuf, sizeof(msgbuf))) == 0) {
				overtime = -1;
			} else {
				addclient(fd, r.sin_addr);
				username = readfromclient(top);
				/*Check if the client is already in buxfer*/
				for(p = header; p ; p = p->next){
					/*If client is not in buxfer*/
					if(strcmp(p->username, username) == 0){
						addaccount(username);
						sprintf(msgbuf, "New account %s added, please enter a command: \r\n", username);
						if(write(fd, msgbuf, sizeof(msgbuf)) == -1){
							perror("write");
							exit(1);
						}
					} else {   /*If user already in buxfer*/						
						sprintf(msgbuf, "Welcome %s, please enter a command: \r\n", username);
						if(write(fd, msgbuf, sizeof(msgbuf)) == -1){
							perror("write");
							exit(1);
						}
					}
				}
			}
			/*Handle timeout error*/
			if (overtime == -1) {
				sprintf(msgbuf, "Communication error or timeout\r\n");
				if(write(fd, msgbuf, sizeof(msgbuf)) == -1){
					perror("write");
					exit(1);
				}
				removeclient(fd);
			}
		}
	}


int main(int argc, char **argv){
	Client *p;
	char *command;
	Group *group_list = NULL;
	
	printf("Buxfer is running!");
	struct sockaddr_in r;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("socket");
	exit(1);
    }

    memset(&r, '\0', sizeof(r));
    r.sin_family = AF_INET;
    r.sin_addr.s_addr = INADDR_ANY;
    r.sin_port = htons(PORT);

    if (bind(listenfd, (struct sockaddr *)&r, sizeof r)) {
	perror("bind");
	exit(1);
    }

    if (listen(listenfd, 5)) {
		perror("listen");
    }
	
	while (1) {
		fd_set fdlist;
		int maxfd = listenfd;
		FD_ZERO(&fdlist);
		FD_SET(listenfd, &fdlist);
		/*set all accepted clients to select watch*/
		for (p = top; p; p = p->next) {
			printf("Buxfer is running!!");
			FD_SET(p->fd, &fdlist);
			if (p->fd > maxfd){
			maxfd = p->fd;
			}
		}
		
		if (select(maxfd + 1, &fdlist, NULL, NULL, NULL) < 0) {
			perror("select");
		} else {
			for (p = top; p; p = p->next){
				if (FD_ISSET(p->fd, &fdlist)){
					/*If client is already connected then send
					 * the information straight to buxfer*/
					command = readfromclient(p);
					if (buxfer(p->fd, command, group_list) == -1){
						removeclient(p->fd);
						};
				}
			}
			/*If the new client is ready to read in the fdlist*/
			if (FD_ISSET(listenfd, &fdlist)){
				newconnection();
				}
		}
	}
}

