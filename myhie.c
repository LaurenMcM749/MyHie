#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 101
#define READ_END 0
#define WRITE_END 1


//Make multiple children using fork: https://www.geeksforgeeks.org/create-n-child-process-parent-process-using-fork-c/

// Data record: resident-id, first name, last name, number of dependents, income and postal code

//Root node: myhie program
// 2. PIPE: Root passes coord: a) file of unsorted data records b) num workers c) numeric attribute of sorting (0,3,4,5) d) executable to be used for sorting
// 3. SIG: Root receives SIGUSR1/SIGUSR2 signals from other processes 
// 4. Root prints number of SIGUSR signals caught before root terminates
// 5. Reports
//      a) time each sorter took to sort its batch
//      a) time the merger took to complete its job
//      a) total time
//      a) number of SIGUSR1 and SIGUSR2 signals the root has caught/seen before end of exec.
////// // 1. Creates single coordinator node - coord 


//Coord node
// PIPE: Coord passes to each sorter: a) the name of the file b) range of data c) which attr to use for sorting d)name of exec for sorting
// SIG: When receive SIGUSR2, gracefully release resources
////// Creates merger node

//Sorting node
// Odd-indexed workers use Heap Sort and even-indexed workers use Bubble Sort
// NAMED PIPE: Passes sorted data and time stats to merger node through name pipe
// SIG: Sends SIGUSR1 to the root before termination

//Merger node
// NAMED PIPE: Receives(reads) ordered partial results from all pipes and creates outcome of entire sorting process
// Prints out stats including time for each sorter to sort data
// SIG: Sends SIGUSR2 to coord to show work completed
////// Created by coord

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
        int fd[2];
        char ans3[20] = "print exec";
        int ans1 = 49;

        
        // Create pipe in ROOT (parent)
        if (pipe(fd) == -1) {
            fprintf(stderr, "Pipe failed.\n");
            return 1;
        }

      

        //Create coord node
        coordpid = fork();

      
        if (coordpid == -1) {
            perror ( " Failed to fork " );
            exit (1) ;
            }
        //Parent (ROOT) writes to pipe

      

        // Close reading end 
        close(fd[READ_END]); 

     

        // Wait for child process to complete
        wait(NULL); 

        // Write sum (ans) to pipe
        write(fd[WRITE_END], &ans1, sizeof(int)); 

        // Close write end of pipe
        close(fd[WRITE_END]);

        //In coord (if coordpid == 0, in child of root = coord)
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

            // Close Write end
            close(fd[WRITE_END]);

            int ans2;

            // Read ans from root node 
            read(fd[READ_END], &ans2, sizeof(int)); 

            // Close reading end
            close(fd[READ_END]);

            printf("Print statement after pipes %s",ans2);

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