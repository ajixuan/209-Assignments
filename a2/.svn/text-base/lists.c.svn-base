#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Add a group with name group_name to the group_list referred to by 
* group_list_ptr. The groups are ordered by the time that the group was 
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.) 
*/
int add_group(Group **group_list_ptr, const char *group_name) {
    Group *new_group = malloc(sizeof(Group));
    
    //Check for if memory is allocated
    if (new_group == NULL) {
        perror("Error allocating space for group.");
        exit(1);
    }
	
	//Set other pointers and values to null for safety reasons
	new_group->xcts = NULL;
	new_group->users = NULL;
	
	//Allocate memory for name of the group
	char *name  = malloc(sizeof(group_name));
    new_group->name = strcpy(name, group_name);
	
	//If the group name already exists
	if(find_group(*group_list_ptr, group_name) != NULL){
		exit(-1);
	}
	
	while(*group_list_ptr != NULL){
		group_list_ptr = &(*group_list_ptr)->next;
	}
	new_group->next = *group_list_ptr;
	*group_list_ptr = new_group;
    return 0;
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
	
	//While there are still groups left in the list
	while(group_list != NULL){
		printf("%s\n", group_list->name);
		group_list = group_list->next;
	}
} 
	
/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the 
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
    while(group_list != NULL){
		if(strcmp(group_list->name, group_name) == 0){
		return group_list;
		}
		group_list = group_list->next;
	}
    return NULL;
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
    User *new_user = malloc(sizeof(User));
    
    //Check for if memory is allocated
    if (new_user == NULL) {
        perror("Error allocating space for group.");
        exit(1);
    }
    
    //Set other pointers to null for safety reasons
    new_user->balance = 0;
    
    //Allocate memory for name of the user
	char *name  = malloc(sizeof(user_name));
    new_user->name = strcpy(name, user_name);
    
    User **user_ptr = &(*group).users;
    while(*user_ptr != NULL){
		//If the group name already exists
		if(strcmp((*user_ptr)->name, new_user->name) == 0){
			exit(-1);
		}
		user_ptr = &(*user_ptr)->next;
	}
	new_user->next = *user_ptr;
	*user_ptr = new_user;
      
    return 0;
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list. 
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you 
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
    User *ptr;
    ptr = group->users;
    while( ptr->next != NULL){
		//If the next node is the node to be deleted
		if(strcmp((ptr->next)->name, user_name) == 0){
			
			//Keep a pointer on the thing to delete so as to free up
			//memory later
			User *delete = ptr->next;
			ptr->next = (ptr->next)->next;
			
			//Remove the xcts
			remove_xct(group, user_name);
			free(delete);
			return 0;
		
		}
		else{
		ptr = ptr->next;
	}
	}
	//If it is the first element that needs to be deleted
	if(strcmp(ptr->name, user_name) == 0){
		group->users = ptr->next;
		User *delete = ptr;
		free(delete);
		
		//Remove the xcts
		remove_xct(group, user_name);
		free(delete);
		return 0;
	}
    return -1;
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely 
* lowest payer first.
*/
void list_users(Group *group) {

//While there are still users left in the list
	User *ptr;
	ptr = group->users;
	while(ptr!= NULL){
		printf("%s\n", ptr->name);
		ptr = ptr->next;
	}
}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {
    User *ptr;
	ptr = group->users;
    while(ptr != NULL){
		if(strcmp(ptr->name, user_name) == 0){
		printf("%f\n", ptr->balance);
		return 0;
		}
		ptr = ptr->next;
	}
    return -1;
}

/* Print to standard output the name of the user who has paid the least 
* If there are several users with equal least amounts, all names are output. 
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance). 
*/
int under_paid(Group *group) {
	User *ptr;
	ptr = group->users;
	
	//Check for if *group is defined 
    if (ptr == NULL) {
        perror("group is not declared");
        exit(1);
    }
	
	char **min_user;
	double smallest = ptr->balance;
	
	//A counter to keep track of the number of people with same balance
	int incr = 0;
	
	while(ptr != NULL){
		if(smallest > ptr->balance){
			smallest = ptr->balance;
			
			//Reset minimum user names and counter
			min_user = &ptr->name;
			incr = 0;
		}
		/*else if(smallest == ptr->balance){
			//increase counter and add name to string array
			incr ++;
			(*min_user + incr) = &ptr->name;
		}*/
		ptr = ptr->next;
	}	
	
	//Print the min usernames
	int i;
	for(i = 0; i <= incr; i++){
		printf("%s", min_user[i]);
	}
    return 0;
}

/* Return a pointer to the user prior to the one in group with user_name. If 
* the matching user is the first in the list (i.e. there is no prior user in 
* the list), return a pointer to the matching user itself. If no matching user 
* exists, return NULL. 
*
* The reason for returning the prior user is that returning the matching user 
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
    User *ptr;
	ptr = group->users;
	
	//If the first node is the specified node
	if(strcmp(ptr->name, user_name) == 0){
	return ptr;
	}
	
	//if the next of now = current pointer then return 
    while(ptr->next != NULL){
		if(strcmp((ptr->next)->name, user_name) == 0){
		return ptr;
		}
		ptr = ptr->next;
	}
    return NULL;
}

/* Add the transaction represented by user_name and amount to the appropriate 
* transaction list, and update the balances of the corresponding user and group. 
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
    User *ptr;
	ptr = group->users;
    
    Xct *xptr = malloc(sizeof(Xct));
    Xct *head = group->xcts;
    
    //Check for if memory is allocated
    if (xptr == NULL) {
        perror("Error allocating space for transaction.");
        exit(1);
    }
    
    //Updating transaction records
    char *name = malloc(sizeof(user_name));
    xptr->name = strcpy(name, user_name);
    xptr->amount = amount;
    
    //Put transactions in order
    xptr->next = head;
    group->xcts = xptr;
    
    //Updating user expense
    
    while(ptr != NULL){
		if(strcmp(xptr->name, ptr->name) == 0){
		ptr->balance += amount;
		break; 
		}
		ptr = ptr->next;
	}
	
    //Sort user in order of balance ascending
    User *prev = find_prev_user(group, ptr->name);
    User *new_ptr;
    
    //If there are still nodes behind the current node
    if(strcmp(prev->name, ptr->name) != 0){
		//Take the node out of the list
		prev->next = ptr->next;
		 new_ptr = group->users;
	}
	else{
		//If the current node is head then skip this node and move to
		//next node for comparison
		new_ptr = ptr->next;
	}
	
    //Declare new ptr to return to front of list to iterate through
    //the whole list.
    
    while(new_ptr != NULL){
		//If we find a value that is larger than specified node balance
		
		if(new_ptr->balance > ptr->balance){
			//Point the next pointer of specified node to current node
			ptr->next = new_ptr;
			
			//Point the next pointer of previous node of current node to
			//specified node
			prev = find_prev_user(group, new_ptr->name);
			prev->next = ptr;
			break; 
		}
		//If the balance of the node is the largest
		if(new_ptr->next == NULL){
			//Place the node in the last position of the list
			new_ptr->next = ptr;
			ptr->next = NULL;
		}
		new_ptr = new_ptr->next;
	}
    return 0;
}

/* Print to standard output the num_xct most recent transactions for the 
* specified group (or fewer transactions if there are less than num_xct 
* transactions posted for this group). The output should have one line per 
* transaction that prints the name and the amount of the transaction. If 
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long nu_xct) {
	Xct *xptr;
	xptr = group->xcts;
	while(nu_xct > 0 && xptr != NULL){
		printf("%s %f\n", xptr->name, xptr->amount);
		xptr = xptr->next;
	}

}

/* Remove all transactions that belong to the user_name from the group's 
* transaction list. This helper function should be called by remove_user. 
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/

void remove_xct(Group *group, const char *user_name) {
	Xct *xptr = group->xcts;
	
	//Iterate through the whole list except firt element
	while(xptr->next != NULL){
	if(strcmp((xptr->next)->name, user_name) == 0){
		//Keep pointer on xct to free up memory later
		Xct *delete = xptr->next;
		xptr->next = (xptr->next)->next;
		free(delete);
	}
	else{
		xptr = xptr->next;
	}	
	}
	
	xptr = group->xcts;
	//Now check the first element of the list
	if(strcmp(xptr->name, user_name) == 0){
		group->xcts = xptr->next;
		Xct *delete = xptr;
		free(delete);
	}
}
