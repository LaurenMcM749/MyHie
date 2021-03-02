#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>


//Make multiple children using fork: https://www.geeksforgeeks.org/create-n-child-process-parent-process-using-fork-c/

// Data record: resident-id, first name, last name, number of dependents, income and postal code

//Root node: myhie program
// 1. Creates single coordinator node - coord 
// 2. Root passes coord: a) file of unsorted data records b) num workers c) numeric attribute of sorting (0,3,4,5) d) executable to be used for sorting
// 3. Root receives SIGUSR1/SIGUSR2 signals from other processes 
// 4. Root prints number of SIGUSR signals caught before root terminates
// 5. Reports
//      a) time each sorter took to sort its batch
//      a) time the merger took to complete its job
//      a) total time
//      a) number of SIGUSR1 and SIGUSR2 signals the root has caught/seen before end of exec.


//Coord node
// Coord passes to each sorter: a) the name of the file b) range of data c) which attr to use for sorting d)name of exec for sorting
// Creates merger node
//When receive SIGUSR2, gracefully release resources

//Sorting node
// Odd-indexed workers use Heap Sort and even-indexed workers use Bubble Sort
// Passes sorted data and time stats to merger node through name pipe
// Sends SIGUSR1 to the root before termination

//Merger node
//Receives(reads) ordered partial results from all pipes and creates outcome of entire sorting process
// Created by coord
// Prints out stats including time for each sorter to sort data
// Sends SIGUSR2 to coord to show work completed

// ./myhie -i InputFile -k NumOfWorkers -r -a AttributeNumber -o Order -s OutputFile
//AttributeNumber is a valid numeric-id that designates the field on which sorting is to be carried out
// Order is either ascending(a) or descending (d)
// OutFile is the file in which the outcome of myhie could be saved

int main(int argc, char*argv[]){

    FILE *fpointer;
    FILE *outfile;
    int k;
    char *atnum;
    char *ord;
    char *outfile_name;
    outfile_name = argv[5];
    fpointer = fopen(argv[1],"r"); 
    outfile = fopen(outfile_name,"w");
    k = atoi(argv[2]);
    atnum = argv[3];
    ord = argv[4];

    
    while(1){
        
        pid_t coordpid;
        pid_t sorterpid;
        pid_t mergerpid;
        coordpid = fork();

        if (coordpid == -1) {
            perror ( " Failed to fork " );
            exit (1) ;
            }
        //In child process of root (if coordpid == 0, in child of root = coord)
        if (coordpid == 0) {
            //Make merger node
            mergerpid = fork();
            //If in merger node
            if(mergerpid == 0){
                printf("Merger node pid: %d, Parent pid: %d", getpid(), getppid());
                exit(0);
            }
            if(mergerpid == 1){
                printf("Still in coord node - hasn't made merger");
            }
            // Make k sorter nodes
            for(int i=0;i<k;i++){ 
                //In sorter node
                sorterpid = fork();
                if(sorterpid == 0) 
                { 
                    printf("\n[sorter] pid %d from [parent] pid %d\n",getpid(),getppid()); 
                    exit(0); 
                }
                //If failed to create sorter node
                if (sorterpid == -1) {
                    perror("Failed to fork");
                    exit(1);
                }
            } 
            //TODO - What exactly does wait do here?
            //Waits for state changes in a child of the calling process, and obtains information about the child whose state has changed.
            //Parent waits for as many children as it makes
            //If sorterpid == 1, in parent of sorter - in coord
                for(int i=0;i<k;i++){
                    wait(NULL);   
                }
              
               

        }
      


        
    return 0;

    }
       
        
    
}