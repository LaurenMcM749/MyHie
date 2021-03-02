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
//Writing to child using pipe https://stackoverflow.com/questions/60104813/using-two-pipes-to-communicate-between-parent-process-and-child-process

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

   
        
        pid_t coordpid;
        pid_t sorterpid;
        pid_t mergerpid;
        int fd[2];
        char printtest[20] = "print exec";
        int ans1 = 49;
        int parent_fds[2];

        //This prints 3 times
        // printf("%s\n",ans3);

        // Create pipe in ROOT (parent)
        if (pipe(fd) == -1) {
            fprintf(stderr, "Pipe failed.\n");
            return 1;
        }

        if (pipe(parent_fds) != 0)  /* || not && */
        {
            fprintf(stderr, "pipes failed!\n");
            return EXIT_FAILURE;
        }

        //Create coord node
        coordpid = fork();

        if (coordpid == -1) {
            perror ( " Failed to fork " );
            exit (1) ;
            }
        //Parent (ROOT) writes to pipe

        // printf("%s\n",ans3);

        //In coord (if coordpid == 0, in child of root = coord)
        if (coordpid == 0) {

             printf("%d: I reached the child :)\n", (int)getpid());
            // close unwanted pipe ends by child
          
            close(parent_fds[1]);

             // read from parent pipe
            char fromParent[100];
            int n = read(parent_fds[0], fromParent, sizeof(fromParent) - 1);
            fromParent[n] = '\0';
            printf("%d: Child: read from parent pipe- %s\n", (int)getpid(), fromParent);
            close(parent_fds[0]);
           
            // // Close Write end
            // close(fd[WRITE_END]);

            // int ans2; 

            // // Read ans from root node 
            // read(fd[READ_END], &ans2, sizeof(int));

            // // Won't print this
            // // printf("%s\n",printtest);

            // // Close reading end
            // close(fd[READ_END]);

            // printf("The pipe answer is %d\n", ans2);

            // printf("Print statement after pipes %d",ans2);

            // //Make merger node
            // mergerpid = fork();
            // //If in merger node
            // if(mergerpid == 0){
            //     printf("Merger node pid: %d, Parent pid: %d", getpid(), getppid());
            //     exit(0);
            // }
            // if(mergerpid == 1){
            //     printf("Still in coord node - hasn't made merger");
            // }

            
            // // Coord makes k sorter nodes
            // for(int i=0;i<k;i++){ 
            //     sorterpid = fork();
            //     //In sorter node
            //     if(sorterpid == 0) 
            //     { 
            //         printf("\n[sorter] pid %d from [parent] pid %d\n",getpid(),getppid());
            //         exit(0); 
            //     }
            //     //If failed to create sorter node
            //     if (sorterpid == -1)
            //     {
            //         perror("Failed to fork");
            //         exit(1);
            //     }
            // }  
            // //Still in coord node (coordpid = 0)
            // // Sorterpid > 0 - coordnode waits for each sorter to finish
            // for(int i=0;i<k;i++){
            //     wait(NULL);   
            // }
        }
        // Coordpid == 1 (In Root)
        else if (coordpid > 0) {

            // close unwanted pipe ends by parent
            close(parent_fds[0]);

            // write from terminal to parent pipe FOR child to read
            printf("%d: Parent: writing to pipe '%s'\n", (int)getpid(), argv[1]);
            write(parent_fds[1],  argv[1], strlen(argv[1]));
            close(parent_fds[1]);

            // // Close reading end 
            // close(fd[READ_END]); 

            // // Wait for child process to complete
            // wait(NULL); 

            // int ans2;
            // ans2 = 70;

            // write(fd[WRITE_END], &ans2, sizeof(int)); //read left half sum from pipe
        
            // close(fd[WRITE_END]);// Close the reading end of the pipe

            // printf("The pipe answer is %d\n", ans1);
        

        }
      


        
    return 0;
       
        
    
}