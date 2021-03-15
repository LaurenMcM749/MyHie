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

void sortedInserta(bucket_t ** head_ref, bucket_t * new_node, char *arg) 
{ 
    bucket_t* current; 
    /* Special case for the head end */
    if (strcmp(arg,"0")== 0)
    {
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
    if (strcmp(arg,"3")== 0)
    {
        if (*head_ref == NULL || atoi((*head_ref)->numdep) >= atoi(new_node->numdep))
        { 
            new_node->next = *head_ref; 
            *head_ref = new_node; 
        } 
        else
        { 
            /* Locate the node before the point of insertion */
            current = *head_ref; 
            while (current->next!=NULL && ( (atoi(current->next->numdep)) < (atoi(new_node->numdep)) ) )
            { 
                current = current->next; 
            } 
            new_node->next = current->next; 
            current->next = new_node; 
        } 

    }
    if (strcmp(arg,"4")== 0)
    {
        if (*head_ref == NULL || atoi((*head_ref)->inc) >= atoi(new_node->inc))
        { 
            new_node->next = *head_ref; 
            *head_ref = new_node; 
        } 
        else
        { 
            /* Locate the node before the point of insertion */
            current = *head_ref; 
            while (current->next!=NULL && ( (atoi(current->next->inc)) < (atoi(new_node->inc)) ) )
            { 
                current = current->next; 
            } 
            new_node->next = current->next; 
            current->next = new_node; 
        } 

    }
    if (strcmp(arg,"5")== 0)
    {
        if (*head_ref == NULL || atoi((*head_ref)->zip) >= atoi(new_node->zip))
        { 
            new_node->next = *head_ref; 
            *head_ref = new_node; 
        } 
        else
        { 
            /* Locate the node before the point of insertion */
            current = *head_ref; 
            while (current->next!=NULL && ( (atoi(current->next->zip)) < (atoi(new_node->zip)) ) )
            { 
                current = current->next; 
            } 
            new_node->next = current->next; 
            current->next = new_node; 
        } 

    }
    
    
}


void sortedInsertd(bucket_t ** head_ref, bucket_t * new_node, char * arg) 
{ 
    bucket_t* current; 
    /* Special case for the head end */
    if (strcmp(arg,"0")== 0)
    {
        if (*head_ref == NULL || atoi((*head_ref)->rin) <= atoi(new_node->rin))
        { 
            new_node->next = *head_ref; 
            *head_ref = new_node; 
        } 
        else
        { 
            /* Locate the node before the point of insertion */
            current = *head_ref; 
            while (current->next!=NULL && ( (atoi(current->next->rin)) > (atoi(new_node->rin)) ) )
            { 
                current = current->next; 
            } 
            new_node->next = current->next; 
            current->next = new_node; 
        } 

    }
     if (strcmp(arg,"3")== 0)
    {
        if (*head_ref == NULL || atoi((*head_ref)->numdep) <= atoi(new_node->numdep))
        { 
            new_node->next = *head_ref; 
            *head_ref = new_node; 
        } 
        else
        { 
            /* Locate the node before the point of insertion */
            current = *head_ref; 
            while (current->next!=NULL && ( (atoi(current->next->numdep)) > (atoi(new_node->numdep)) ) )
            { 
                current = current->next; 
            } 
            new_node->next = current->next; 
            current->next = new_node; 
        } 

    }
     if (strcmp(arg,"4")== 0)
    {
        if (*head_ref == NULL || atoi((*head_ref)->inc) <= atoi(new_node->inc))
        { 
            new_node->next = *head_ref; 
            *head_ref = new_node; 
        } 
        else
        { 
            /* Locate the node before the point of insertion */
            current = *head_ref; 
            while (current->next!=NULL && ( (atoi(current->next->inc)) > (atoi(new_node->inc)) ) )
            { 
                current = current->next; 
            } 
            new_node->next = current->next; 
            current->next = new_node; 
        } 

    }
     if (strcmp(arg,"5")== 0)
    {
        if (*head_ref == NULL || atoi((*head_ref)->zip) <= atoi(new_node->zip))
        { 
            new_node->next = *head_ref; 
            *head_ref = new_node; 
        } 
        else
        { 
            /* Locate the node before the point of insertion */
            current = *head_ref; 
            while (current->next!=NULL && ( (atoi(current->next->zip)) > (atoi(new_node->zip)) ) )
            { 
                current = current->next; 
            } 
            new_node->next = current->next; 
            current->next = new_node; 
        } 

    }
    
} 

// function to sort a singly linked list using insertion sort 
void insertionSorta(bucket_t **head_ref, char *arg) 
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
        sortedInserta(&sorted, current, arg); 
  
        // Update current 
        current = next; 
    } 
  
    // Update head_ref to point to sorted linked list 
    *head_ref = sorted; 
} 

// function to sort a singly linked list using insertion sort 
void insertionSortd(bucket_t **head_ref, char * arg) 
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
        sortedInsertd(&sorted, current, arg); 
  
        // Update current 
        current = next; 
    } 
  
    // Update head_ref to point to sorted linked list 
    *head_ref = sorted; 
} 

void printList(bucket_t *start, FILE *outfile) 
{ 
    bucket_t *temp = start; 
    printf("\n"); 
    while (temp!=NULL) 
    { 
        fprintf(outfile,"%s %s %s %s %s %s %s\n ", "2: ",temp->rin, temp->first, temp->last, temp->numdep, temp->inc, temp->zip);
        temp = temp->next; 
    } 
} 


int main(int argc, char *argv[]){

    FILE *testdata;
    FILE *outfile;
    testdata = fopen(argv[4],"r");
    outfile = fopen(argv[5],"a");
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
    int fdnamed;
    char* sorter_results;
    int w;

    bucket_t *start = NULL; 

    char * sr = "sorter_results_file"; 
    int mkfifo3;
  
    mkfifo3 = mkfifo(sr, 0777); 

    if (mkfifo3 == 0)
    {
        printf("Myfifo success\n");
    }

    if (mkfifo3 == -1)
    {
        if (errno != EEXIST)
        {
        perror("Error: couldn't create myfifo pipe");
        exit(0);
        }
    }


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

    //If strcmp = 0
    
    if( strcmp(argv[1],"0" )==0){
        printf("Sorting RIN... \n");
        if( strcmp(argv[2],"a" ) == 0)
        {
            printf("Ascending IS \n");
            insertionSorta(&start,"0");
        }
        if( strcmp(argv[2],"d" ) == 0)
        {
            printf("Descending IS \n");
            insertionSortd(&start,"0");
        }
    }
    if( strcmp(argv[1],"3" )==0){
        printf("Sorting numdep... \n");
        if( strcmp(argv[2],"a" ) == 0)
        {
            printf("Ascending IS \n");
            insertionSorta(&start,"3");
        }
        if( strcmp(argv[2],"d" ) == 0)
        {
            printf("Descending IS \n");
            insertionSortd(&start,"3");
        }
    }
    if( strcmp(argv[1],"4" )==0){
        printf("Sorting income... \n");
        if( strcmp(argv[2],"a" ) == 0)
        {
            printf("Ascending IS \n");
            insertionSorta(&start,"4");
        }
        if( strcmp(argv[2],"d" ) == 0)
        {
            printf("Descending IS \n");
            insertionSortd(&start,"4");
        }
    }
    if( strcmp(argv[1],"5" )==0){
        printf("Sorting zip... \n");
        if( strcmp(argv[2],"a" ) == 0)
        {
            printf("Ascending IS \n");
            insertionSorta(&start,"5");
        }
        if( strcmp(argv[2],"d" ) == 0)
        {
            printf("Descending IS \n");
            insertionSortd(&start,"5");
        }
    }
    printList(start,outfile);
    fclose(outfile);

      while(!feof(outfile)){
        fgets(sorter_results,150,outfile);
        fdnamed = open(sr, O_NONBLOCK); 
        // Write the input arr2 on FIFO  and close it 
        w = write(fdnamed, sorter_results, strlen(sorter_results));
        if ( w < 0 ){
            perror("write");
        };  //+1 to account for \0 at the end of strings in C
        printf("Wrote %d bytes to %s to sorter_results pipe\n", w, sorter_results);
        close(fdnamed); 
       
    }

}