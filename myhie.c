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
#include <fcntl.h> 
#include <sys/stat.h> 

#define N 101
#define READ_END 0
#define WRITE_END 1
#define LENGTH 200


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

while(1){

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
    pid_t pid;
    int fd[2];
    int parent_fds[2];
    int fd1[2];
    int parent_fds1[2];
    char printtest[20] = "print exec";
    int ans1 = 49;
    int status;
    char fromRoot[100];
    char str1[LENGTH], str2[LENGTH]; 
    int fdnamed;
    char arr1[LENGTH];

    // FIFO file path 
    char * myfifo = "myfifo"; 
  
    mkfifo(myfifo, 0777); 
  
    //Create coord node
    coordpid = fork();

    if (coordpid == -1) {
        perror ( " Failed to fork " );
        exit (1) ;
        }
    // In Root Node
    if (coordpid > 0) {
        
        // Create pipe in root
        if (pipe(fd) == -1) {
            fprintf(stderr, "Pipe failed.\n");
            return 1;
        }
        //Check if pipe failed
        if (pipe(parent_fds) != 0) 
        {
            fprintf(stderr, "pipes failed!\n");
            return EXIT_FAILURE;
        }

        printf("I am the root process");
        fflush(stdout);

        // close unwanted pipe ends by parent
        close(parent_fds[0]);

        // write from terminal to parent pipe FOR child to read
        printf("%d: Root: writing to pipe '%s'\n", (int)getpid(), argv[1]);
        write(parent_fds[1],  argv[1], strlen(argv[1]));
        close(parent_fds[1]); 
    }
    //In Coord Node
    if (coordpid == 0) {
          //Make merger node
        mergerpid = fork();
        //If in merger node
        if (mergerpid == -1){
            printf("Merger node failed \n");
        }
        if (mergerpid > 0){
            printf("I am coord node (merger) \n");
        }
        if (mergerpid == 0){
            // Open FIFO for Read only 
            fdnamed = open(myfifo, O_RDONLY); 
    
            // Read from FIFO 
            read(fdnamed, arr1, sizeof(arr1)); 
    
            // Print the read message 
            printf("Merger node reading: %s\n", arr1); 
            close(fdnamed);
            //printf("Merger node pid: %d, Parent pid: %d \n", getpid(), getppid());
            exit(0);
        }
        // Coord makes k sorter nodes
        for(int i = 0; i < k; i++){ 

            sorterpid = fork();
            
            //Check if fork failed
            if (sorterpid == -1)
            {
                perror("Failed to fork");
                exit(1);
            }
    
            // Sorterpid == 1 (In Coord)
            if (sorterpid > 0){
               
                // Create pipe in coord (parent)
                if (pipe(fd1) == -1) {
                    fprintf(stderr, "Pipe failed.\n");
                    return 1;
                }

                // Check if pipe failed
                if (pipe(parent_fds1) != 0)
                {
                    fprintf(stderr, "pipes failed!\n");
                    return EXIT_FAILURE;
                }

                printf("%d: I reached the coord :)\n", (int)getpid());

                fflush(stdout);
                // close unwanted pipe ends by child
                close(parent_fds[1]);

                // read from parent pipe
                int n = read(parent_fds[0], fromRoot, sizeof(fromRoot) - 1);
                fromRoot[n] = '\0';
                printf("%d: Coord: read from root pipe- %s\n", (int)getpid(), fromRoot);
                close(parent_fds[0]);

                // close unwanted pipe ends by parent
                fflush(stdout);
                close(parent_fds1[0]);

                // write from terminal to parent pipe FOR child to read
                printf("%d: Coord: writing to pipe '%s'\n", (int)getpid(), fromRoot);
                write(parent_fds1[1], fromRoot, strlen(fromRoot));
                close(parent_fds1[1]);

              
            }

            //In sorter node
            if(sorterpid == 0) { 
            //     printf("%d: I reached the sorter :)\n", (int)getpid());

                // close unwanted pipe ends by child
                fflush(stdout);
                close(parent_fds1[1]);

                // read from parent pipe
                char fromCoord[100];
                int n = read(parent_fds1[0], fromCoord, sizeof(fromCoord) - 1);
                fromCoord[n] = '\0';
                printf("%d: Sorter: read from coord pipe- %s \n", (int)getpid(), fromCoord);
                close(parent_fds[0]);

                // Open FIFO for write only 
                fdnamed = open(myfifo, O_WRONLY); 
        
                // Take an input from user. 
                // LENGTH 200 is maximum length 
                // fgets(arr2, LENGTH, stdin); 
        
                // Write the input arr2 on FIFO 
                // and close it 
                write(fdnamed, fromCoord, strlen(fromCoord)+1);  //+1 to account for \0 at the end of strings in C
                close(fdnamed); 
    

                // printf("\n[sorter] pid %d from [parent] pid %d\n",getpid(),getppid());
                exit(0); 
            }
         
        }  
        //Still in coord node (coordpid = 0)
        // Sorterpid > 0 - coordnode waits for each sorter to finish
        // for(int i=0;i<k;i++){
        //     //Wait for sorter to finish
        //     wait(NULL);   
        // }

        // Coordpid == 1 (In Root)
    
    
    return 0;

}
}  
}  

      