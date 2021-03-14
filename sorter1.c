#include<stdio.h>
#include<stdlib.h>
#include "myhie.h"

// https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/

//Bubble Sort

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

void swap(bucket_t *a, bucket_t *b) 
{ 
    char* temp = a->rin; 
    a->rin = b->rin; 
    b->rin = temp; 
} 


void bubbleSort(bucket_t *start) 
{ 
    int swapped, i; 
    bucket_t *ptr1; 
    bucket_t *lptr = NULL; 
  
    /* Checking for empty list */
    if (start == NULL) 
        return; 
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->next != lptr) 
        { 
            //While current RIN > next RIN
            if ( atoi(ptr1->rin) > atoi(ptr1->next->rin) ) 
            {  //
                swap(ptr1, ptr1->next); 
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); 
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
    // printList(start);

    //If strcmp = 0
    
   
    if( strcmp(argv[1],"0" )==0){
        printf("Sorting RIN... \n");
        if( strcmp(argv[2],"a" ) == 0)
        {
            printf("Ascending \n");
            bubbleSort(start);
            
           
        }

        if( strcmp(argv[2],"d" ) == 0)
        {
            printf("Descending \n");
        }


    }

    printList(start);
    

}