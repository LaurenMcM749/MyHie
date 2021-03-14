//https://www.geeksforgeeks.org/insertion-sort-for-singly-linked-list/

#include<stdio.h>
#include<stdlib.h>
#include "myhie.h"

typedef struct bucket_t {
    //char *sorter;
    char *rin; 
    char *first; 
    char *last; 
    char *numdep; 
    char *inc; 
    char *zip;
    struct bucket_t *next; //Might not need next - not a linked list actually
} bucket_t ;

//Make the voter entry - with RIN, last, first, voted, zip
bucket_t *create_bucket(bucket_t **start_ref, const char *rin, const char *first, const char *last, const char *numdep, const char *inc, const char *zip) {
    // allocate the entry
    // Create entry
    // Assign key, last
    
    bucket_t *bucket = (struct bucket_t*)malloc(sizeof(bucket_t) * 1);
    bucket->rin = malloc(strlen(rin) + 1);
    bucket->first = malloc(strlen(first) + 1);
    bucket->last = malloc(strlen(last) + 1);
    bucket->numdep = malloc(strlen(numdep) + 1);
    bucket->inc = malloc(strlen(inc) + 1);
    bucket->zip = malloc(strlen(zip) + 1);
    bucket->next = *start_ref; 
    *start_ref = bucket; 
   
    strcpy(bucket->rin, rin);
    strcpy(bucket->first, first);
    strcpy(bucket->last, last);
    strcpy(bucket->numdep, numdep);
    strcpy(bucket->inc, inc);
    strcpy(bucket->zip, zip);
    
    // next starts out null but may be set later on
    // bucket->next = NULL;

    return bucket;
   
}

void sortedInsert(bucket_t ** head_ref, bucket_t * new_node) 
{ 
    bucket_t* current; 
    /* Special case for the head end */
    if (*head_ref == NULL || atoi((*head_ref)->rin) >= atoi(new_node->rin))
    { 
        new_node->next = *head_ref; 
        *head_ref = new_node; 
    } 
    else
    { 
        /* Locate the node before the point of insertion */
        current = *head_ref; 
        while (current->next!=NULL && ( (atoi(current->next->rin)) < (atoi(new_node->rin)) ) )
        { 
            current = current->next; 
        } 
        new_node->next = current->next; 
        current->next = new_node; 
    } 
} 

// function to sort a singly linked list using insertion sort 
void insertionSort(bucket_t **head_ref) 
{ 
    // Initialize sorted linked list 
    bucket_t *sorted = NULL; 
  
    // Traverse the given linked list and insert every 
    // node to sorted 
    bucket_t *current = *head_ref; 
    while (current != NULL) 
    { 
        // Store next for next iteration 
        bucket_t *next = current->next; 
  
        // insert current in sorted linked list 
        sortedInsert(&sorted, current); 
  
        // Update current 
        current = next; 
    } 
  
    // Update head_ref to point to sorted linked list 
    *head_ref = sorted; 
} 

void printList(bucket_t *start) 
{ 
    bucket_t *temp = start; 
    printf("\n"); 
    while (temp!=NULL) 
    { 
        printf("%s %s %s %s %s %s \n ", temp->rin, temp->first, temp->last, temp->numdep, temp->inc, temp->zip); 
        temp = temp->next; 
    } 
} 


int main(int argc, char *argv[]){

    FILE *testdata;
    FILE *outfile;
    testdata = fopen("test_data.txt","r");
    outfile = fopen("outfile.txt","w");
    char singleLine[500];
    char *token;
    int i;
    char *rin;
    char *first;
    char *last;
    char *numdep;
    char *inc;
    char *zip;
    int count;

    bucket_t *start = NULL; 

    //Change to integers
    while(!feof(testdata)){ //file end of file - while not end of file
        //Run this loop from beginning of file to the end
        //Default- file pointer starts at the beginning
        fgets(singleLine, 150, testdata); //gets one line from FILE, not from standard input like keyboard
        //Tokenize line
        token = strtok(singleLine, " ");
        i=0;
        count = 0;
        while (token != NULL){
            //For each line - assign key,first,last,zip
            if (i==0){
                rin = token;
            }
            if (i == 1){
                first = token;
            }
            if (i == 2){
                last = token;
            }
            if (i == 3){
                numdep = token;
            }
            if (i == 4){
                inc = token;
            }
            if (i == 5){
                zip = token;
            }
            token = strtok(NULL, " "); 
            i++;
        }
        //Insert each entry (use i)
        create_bucket(&start, rin, first, last, numdep, inc, zip);
        count = count + 1;

    }
    fclose(testdata);

    //List created
    printList(start);

    //If strcmp = 0
    
    if( strcmp(argv[1],"0" )==0){
        printf("Sorting RIN... \n");
        if( strcmp(argv[2],"a" ) == 0)
        {
            printf("Ascending \n");
            insertionSort(&start);
           
        }

        if( strcmp(argv[2],"d" ) == 0)
        {
            printf("Descending \n");
        }


    }
    printList(start);

}