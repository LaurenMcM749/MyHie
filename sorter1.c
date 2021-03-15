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


void bubbleSorta(bucket_t *start, char*arg) 
{ 
    int swapped, i; 
    bucket_t *ptr1; 
    bucket_t *lptr = NULL; 
  
    if (strcmp(arg,"0")==0)
    {   
        /* Checking for empty list */
        if (start == NULL) 
            return; 
        do
        { 
            swapped = 0; 
            ptr1 = start; 
            while (ptr1->next != lptr) 
            { 
                if ( atoi(ptr1->rin) > atoi(ptr1->next->rin) ) 
                {  
                    swap(ptr1, ptr1->next); 
                    swapped = 1; 
                } 
                ptr1 = ptr1->next; 
            } 
            lptr = ptr1; 
        } 
        while (swapped); 
    }
    if (strcmp(arg,"3")==0)
    {   
        /* Checking for empty list */
        if (start == NULL) 
            return; 
        do
        { 
            swapped = 0; 
            ptr1 = start; 
            while (ptr1->next != lptr) 
            { 
                if ( atoi(ptr1->numdep) > atoi(ptr1->next->numdep) ) 
                {  
                    swap(ptr1, ptr1->next); 
                    swapped = 1; 
                } 
                ptr1 = ptr1->next; 
            } 
            lptr = ptr1; 
        } 
        while (swapped); 
    }
    if (strcmp(arg,"4")==0)
    {   
        /* Checking for empty list */
        if (start == NULL) 
            return; 
        do
        { 
            swapped = 0; 
            ptr1 = start; 
            while (ptr1->next != lptr) 
            { 
                if ( atoi(ptr1->inc) > atoi(ptr1->next->inc) ) 
                {  
                    swap(ptr1, ptr1->next); 
                    swapped = 1; 
                } 
                ptr1 = ptr1->next; 
            } 
            lptr = ptr1; 
        } 
        while (swapped); 
    }
    if (strcmp(arg,"5")==0)
    {   
        /* Checking for empty list */
        if (start == NULL) 
            return; 
        do
        { 
            swapped = 0; 
            ptr1 = start; 
            while (ptr1->next != lptr) 
            { 
                if ( atoi(ptr1->zip) > atoi(ptr1->next->zip) ) 
                {  
                    swap(ptr1, ptr1->next); 
                    swapped = 1; 
                } 
                ptr1 = ptr1->next; 
            } 
            lptr = ptr1; 
        } 
        while (swapped); 
    }
} 

void bubbleSortd(bucket_t *start, char* arg) 
{ 
    int swapped, i; 
    bucket_t *ptr1; 
    bucket_t *lptr = NULL; 

     /* Checking for empty list */
        if (start == NULL) 
            return; 
  
    if (strcmp(arg,"0")==0)
    {   
       
        do
        { 
            swapped = 0; 
            ptr1 = start; 
            while (ptr1->next != lptr) 
            { 
                if ( atoi(ptr1->rin) < atoi(ptr1->next->rin) ) 
                {  
                    swap(ptr1, ptr1->next); 
                    swapped = 1; 
                } 
                ptr1 = ptr1->next; 
            } 
            lptr = ptr1; 
        } 
        while (swapped); 
    }
    if (strcmp(arg,"3")==0)
    {   
        // /* Checking for empty list */
        // if (start == NULL) 
        //     return; 
        do
        { 
            swapped = 0; 
            ptr1 = start; 
            while (ptr1->next != lptr) 
            { 
                if ( atoi(ptr1->numdep) < atoi(ptr1->next->numdep) ) 
                {  
                    swap(ptr1, ptr1->next); 
                    swapped = 1; 
                } 
                ptr1 = ptr1->next; 
            } 
            lptr = ptr1; 
        } 
        while (swapped); 
    }
    if (strcmp(arg,"4")==0)
    {   
        // /* Checking for empty list */
        // if (start == NULL) 
        //     return; 
        do
        { 
            swapped = 0; 
            ptr1 = start; 
            while (ptr1->next != lptr) 
            { 
                if ( atoi(ptr1->inc) < atoi(ptr1->next->inc) ) 
                {  
                    swap(ptr1, ptr1->next); 
                    swapped = 1; 
                } 
                ptr1 = ptr1->next; 
            } 
            lptr = ptr1; 
        } 
        while (swapped); 
    }
    if (strcmp(arg,"5")==0)
    {   
        // /* Checking for empty list */
        // if (start == NULL) 
        //     return; 
        do
        { 
            swapped = 0; 
            ptr1 = start; 
            while (ptr1->next != lptr) 
            { 
                if ( atoi(ptr1->zip) < atoi(ptr1->next->zip) ) 
                {  
                    swap(ptr1, ptr1->next); 
                    swapped = 1; 
                } 
                ptr1 = ptr1->next; 
            } 
            lptr = ptr1; 
        } 
        while (swapped); 
    }
} 



void printList(bucket_t *start, FILE *outfile) 
{ 

    bucket_t *temp = start; 
    printf("\n"); 
    while (temp!=NULL) 
    { 
        fprintf(outfile,"%s %s %s %s %s %s %s\n ", "1:", temp->rin, temp->first, temp->last, temp->numdep, temp->inc, temp->zip);
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

      //Make sorter results pipe

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
            printf("Ascending BS \n");
            bubbleSorta(start,"0");
        }
        if( strcmp(argv[2],"d" ) == 0)
        {
            printf("Descending BS \n");
            bubbleSortd(start,"0");
        }
    }
    if( strcmp(argv[1],"3" )==0){
        printf("Sorting numdep... \n");
        if( strcmp(argv[2],"a" ) == 0)
        {
            printf("Ascending BS \n");
            bubbleSorta(start,"3");
        }
        if( strcmp(argv[2],"d" ) == 0)
        {
            printf("Descending BS \n");
            bubbleSortd(start,"3");
        }
    }
    if( strcmp(argv[1],"4" )==0){
        printf("Sorting income... \n");
        if( strcmp(argv[2],"a" ) == 0)
        {
            printf("Ascending BS \n");
            bubbleSorta(start,"4");
        }
        if( strcmp(argv[2],"d" ) == 0)
        {
            printf("Descending BS \n");
            bubbleSortd(start,"4");
        }
    }
    if( strcmp(argv[1],"5" )==0){
        printf("Sorting zip... \n");
        if( strcmp(argv[2],"a" ) == 0)
        {
            printf("Ascending BS \n");
            bubbleSorta(start,"5");
        }
        if( strcmp(argv[2],"d" ) == 0)
        {
            printf("Descending BS \n");
            bubbleSortd(start,"5");
        }
    }

    printList(start, outfile);
   

    

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