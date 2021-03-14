
#include<stdio.h>
#include<stdlib.h>
#include "myhie.h"

//Make multiple children using fork: https://www.geeksforgeeks.org/create-n-child-process-parent-process-using-fork-c/
//Writing to child using pipe https://stackoverflow.com/questions/60104813/using-two-pipes-to-communicate-between-parent-process-and-child-process
//Signals - https://www.tutorialspoint.com/c_standard_library/c_function_signal.htm
// Pipe function - https://linuxhint.com/using_pipe_function_c_language/


// Data record: resident-id, first name, last name, number of dependents, income and postal code

//Root node: myhie program
// TODO - 5. Reports
//      a) time each sorter took to sort its batch
//      a) time the merger took to complete its job
//      a) total time
//      a) number of SIGUSR1 and SIGUSR2 signals the root has caught/seen before end of exec.
////// // 1. Creates single coordinator node - coord 
// DEBUG - // 2. PIPE: Root passes coord: a) file of unsorted data records b) num workers c) numeric attribute of sorting (0,3,4,5) d) executable to be used for sorting
// DEBUG - 3. SIG: Root receives SIGUSR1/SIGUSR2 signals from other processes 
// DEBUG - // 4. Root prints number of SIGUSR signals caught before root terminates


//Coord node
// TODO - PIPE: Coord passes to each sorter: a) the name of the file b) range of data c) which attr to use for sorting d)name of exec for sorting
// TODO - SIG: When receive SIGUSR2, gracefully release resources
////// Creates merger node

//Sorting node
// TODO - dd-indexed workers use Heap Sort and even-indexed workers use Bubble Sort
// TODO - NAMED PIPE: Passes sorted data and time stats to merger node through name pipe
// DEBUG - SIG: Sends SIGUSR1 to the root before termination

//Merger node
// TODO - NAMED PIPE: Receives(reads) ordered partial results from all pipes and creates outcome of entire sorting process
// TODO - Prints out stats including time for each sorter to sort data
// DEBUG - SIG: Sends SIGUSR2 to coord to show work completed
////// Created by coord

// ./myhie -i InputFile -k NumOfWorkers -r -a AttributeNumber -o Order -s OutputFile
//AttributeNumber is a valid numeric-id that designates the field on which sorting is to be carried out
// Order is either ascending(a) or descending (d)
// OutFile is the file in which the outcome of myhie could be saved


//Notes
//signal sets a function to handle signal (signal handler with signal number SIG)
//BUG - named pipe won't open
//Attempts to read from a pipe that is currently empty block until at least one byte has been written to the pipe.

void signalHandler(int signum){
    printf("Caught signal %d!\n",signum);
	if (signum==SIGUSR1) {
		printf("got SIGUSR1\n");
        //count++
		wait(NULL);
	}
    if (signum==SIGUSR2) {
		printf("got SIGUSR2\n");
        //count++
        //Gracefull release resources
		wait(NULL);
    }
}

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

    int pipe1[2];
    int pipe2[2];
    char printtest[20] = "print exec";
    int ans1 = 49;
    int status;
    // char fromRoot[100];
    char fromRoot[1025];
    // char fromRoot;
    char str1[LENGTH], str2[LENGTH]; 
    int fdnamed;
    char arr1[LENGTH];
    int rootpid;
    int p;
    char *tester;
    int w;

    //***Handle command line arguments***
    char argarr[1000];
    
    strcat(argarr,argv[1]); //inputfile
    // printf("Argarr: %s\n", argarr);
    int size1 = strlen(argarr);
    // printf("Size1: %d\n", size1);

    strcat(argarr,argv[2]); //numworkers
    // printf("Argarr: %s\n", argarr);
    int size2 = strlen(argarr) - size1;
    // printf("Size2: %d\n", size2);

    strcat(argarr,argv[3]); //0,3,4,5
    // printf("Argarr: %s\n", argarr);
    int size3 = strlen(argarr) - size2;

    strcat(argarr,argv[4]);//order
    // printf("Argarr: %s\n", argarr);
    int size4 = strlen(argarr) - size3;

     strcat(argarr,argv[5]); //outputfile 
    // printf("Argarr: %s\n", argarr);
    int size5 = strlen(argarr) - size4;

    int sizefinal = strlen(argarr);

    // FIFO file path 
    //Need to delete file every time... mkfifo worked after changed name
    char * myfifo5 = "myfifo5"; 
    int mkfifo2;
  
    mkfifo2 = mkfifo(myfifo5, 0777); 

    if (mkfifo2 == 0)
    {
        printf("Myfifo success\n");
    }

    if (mkfifo2 == -1)
    {
        if (errno != EEXIST)
        {
        perror("Error: couldn't create myfifo pipe");
        exit(0);
        }
    }

    //Signal handling
    signal(SIGUSR1,signalHandler);
    signal(SIGUSR2,signalHandler);

    //Create pipe1
    if ( pipe(pipe1) == -1 )
    {
        printf("Pipe1 failed \n");
        return 0;

    };

    if ( pipe(pipe2) == -1 )
    {
        printf("Pipe2 failed \n");
        return 0;

    };

    
    //Create coord node
    coordpid = fork();

    

    if (coordpid == -1) {
        perror ( " Failed to fork " );
        exit (1) ;
        }

    // In Root Node
    if (coordpid > 0) 
    {

        //Create pipe2 - BAD FILE DESCRIPTOR HERE
  
        // rootpid = getpid();

        // Create pipe in root
        // if (pipe(fd) == -1) 
        // {
        //     fprintf(stderr, "Pipe failed.\n");
        //     return 1;
        // }

       
        //Check if pipe failed
        // if (pipe(pipe1) != 0) 
        // {
        //     fprintf(stderr, "pipes failed!\n");
        //     return EXIT_FAILURE;
        // }

        tester = "tester";

    

        // close unwanted pipe ends by parent
        close(pipe1[0]);
        printf("%d: Root: writing to pipe '%s'\n", getpid(), tester);
        w = write(pipe1[1], tester, strlen(tester)+1);
        if (w >= 0)
        {
            printf("Write successful: %d bytes \n ", w);
        }
        else
        {
            perror("write");
        }
        close(pipe1[1]); 
    }
    //In Coord Node
    if (coordpid == 0) {

            

 
        //Make merger node
        // mergerpid = fork();
        // // wait(NULL);
       
        // if (mergerpid == -1){
        //     printf("Merger node failed \n");
        // } 
        // //In merger node
        // if (mergerpid == 0){
        //     wait(NULL); //Wait makes the program not stop
        //     printf("I am merger node \n");

        //     // Open FIFO for Read only 
        //     //Changed O_RDONLY to O_RDWR - bug was that wasn't opened at write end at same time
        //     fdnamed = open(myfifo5, O_RDWR); 

        //     if ( fdnamed < 0) 
        //     {
        //         perror("File can't open to read.");
        //         return 0;
        //     }
          
        //      printf("Opened fd: %d\n", fdnamed);
    
        //     // THIS IS THE BUG !! Read from FIFO - STUCK HERE NOW
        //     read(fdnamed, arr1, sizeof(arr1)); 
    
        //     // Print the read message 
        //     printf("Merger node reading: %s\n", arr1); 
        //     close(fdnamed);
        //     printf("Merger node pid: %d, Parent pid: %d \n", getpid(), getppid());

        //     printf("Merger sending SIGUSR2 to coord...\n");
        //     printf("Mergerpid = 0: Pid is: %d\n", getpid());
        //     kill(getppid(),SIGUSR2); /*send SIGUSR1 signal to parent*/
        //     //Doesn't print this...
            
        //     exit(0);
        //     //How to get child to terminate
        // }
        //In coord node
        // if (mergerpid > 0)
        // {
            // printf("Mergerpid > 0: Pid is: %d\n", getpid());
            // printf("I am coord node %d \n", getpid());

            // pipe(pipe1);
                
            //Should this be parent_fds1 or parent_fds
            // Create pipe in coord (parent)
            // if (pipe(parent_fds1) == -1) {
            //     fprintf(stderr, "Pipe failed.\n");
            //     return 1;
            // }

            // // Check if pipe failed
            // if (pipe(parent_fds1) != 0)
            // {
            //     fprintf(stderr, "pipes failed!\n");
            //     return EXIT_FAILURE;
            // }



        
            // Close Write end of pipe1
            close(pipe1[1]);

            // Read from read end of pipe 1
            int n = read(pipe1[0], fromRoot, 1024);
            if (n >= 0){
                fromRoot[n] = 0;  //terminate the string
                printf("%d: Coord read %d bytes from pipe1: %s\n", getpid(), n, fromRoot);
            }
            else
            {
                 perror("read");
            }
            close(pipe1[0]);


            
            // close unwanted pipe ends by parent
            close(pipe2[0]);
            printf("%d: Coord: writing to pipe2 '%s'\n", getpid(), fromRoot);
            w = write(pipe2[1], fromRoot, strlen(fromRoot));
            //write() only blocks when the write operation would overflow the (system) buffer
            if (w == 0)
            {
                printf("Write successful: %d bytes \n ", w);
            }
            else
            {
                perror("write");
            }
            close(pipe2[1]); 

            //Send SIGUSR1 before termination
            //Debug - make sure sends to root
            // printf("Sorter sending SIGUSR1 to root...\n");
            // kill(rootpid,SIGUSR1); /*send SIGUSR1 signal to parent*/

            

       
            // Coord makes k sorter nodes
            for(int i = 0; i < k; i++){ 

                sorterpid = fork();
            
               
                // wait(NULL);
                
                //Check if fork failed
                if (sorterpid == -1)
                {
                    perror("Failed to fork");
                    exit(1);
                }
        
                // In Coord node
                // NOT EXECUTING (BUG!)
                // if (sorterpid > 0){

                    
                
                // }
                

            //In sorter node
            //TODO - Get the args to come from parent pipe!!
            if(sorterpid == 0) { 
                
                p = getpid();
                printf("%d: I am sorter node \n", p);
                
                //Even pid use Bubble Sort
                if ( p % 2 == 0)
                {
                    printf("Using bubble sort...\n");
                    execlp("./sorter1","sorter1", argv[3], argv[4], NULL); 
                }
                //Odd pid use Insertion Sort
                else 
                {
                    printf("Using insertion sort...\n");
                    execlp("./sorter2","sorter2", argv[3], argv[4], NULL); 
                }
 
                // // close unwanted pipe ends by child
                // fflush(stdout);
                // close(parent_fds1[1]);

                // // read from parent pipe
                // char fromCoord[100];
                // int n = read(parent_fds1[0], fromCoord, sizeof(fromCoord) - 1);
                // fromCoord[n] = '\0';
                // printf("%d: Sorter: read from coord pipe- %s \n", (int)getpid(), fromCoord);
                // close(parent_fds[0]);

              
                // // Open FIFO for write only 
                // fdnamed = open(myfifo5, O_WRONLY); 
        
                // // Take an input from user. 
                // // LENGTH 200 is maximum length 
                // // fgets(arr2, LENGTH, stdin); 
        
                // // Write the input arr2 on FIFO 
                // // and close it 
                // write(fdnamed, "fifotest", strlen("fifotest")+1);  //+1 to account for \0 at the end of strings in C
                // close(fdnamed); 
    
                exit(0); 
            }
         
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

// }
}  
}  
