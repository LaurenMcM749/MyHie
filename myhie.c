
#include<stdio.h>
#include<stdlib.h>
#include "myhie.h"

//Make multiple children using fork: https://www.geeksforgeeks.org/create-n-child-process-parent-process-using-fork-c/
//Writing to child using pipe https://stackoverflow.com/questions/60104813/using-two-pipes-to-communicate-between-parent-process-and-child-process
//Signals - https://www.tutorialspoint.com/c_standard_library/c_function_signal.htm

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
    char printtest[20] = "print exec";
    int ans1 = 49;
    int status;
    // char fromRoot[100];
    char fromRoot;
    // char fromRoot;
    char str1[LENGTH], str2[LENGTH]; 
    int fdnamed;
    char arr1[LENGTH];
    int rootpid;

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

    //Create coord node
    coordpid = fork();

    // wait(NULL);

    if (coordpid == -1) {
        perror ( " Failed to fork " );
        exit (1) ;
        }
    // In Root Node
    if (coordpid > 0) {
        rootpid = getpid();
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

        // close unwanted pipe ends by parent
        close(parent_fds[0]);

        // write from terminal to parent pipe FOR child to read
    
        //TODO - Pass 1 Array that contains all arguments
        // Separate by bytes when you read
        
        printf("%d: Root: writing to pipe '%s'\n", (int)getpid(), argarr);
        //BUG - Does not go to this line - does not print W
        int w = write(parent_fds[1], &argarr, 100);
        printf("W: %d \n", w);
        close(parent_fds[1]); 
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
            printf("I am coord node %d \n", getpid());
                
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

            // fflush(stdout);
            // close unwanted pipe ends by child
            close(parent_fds[1]);

            // read from parent pipe
            int n = read(parent_fds[0], &fromRoot, sizeof(&fromRoot));
            printf("Bytes read %d \n", n);
            // fromRoot[n] = '\0';
            // Read X number of bytes from argument
            printf("%d: Coord: read from root pipe- %s\n", (int)getpid(), &fromRoot);
            close(parent_fds[0]);

            // close unwanted pipe ends by parent
            fflush(stdout);
            close(parent_fds1[0]);

            // write from terminal to parent pipe FOR child to read
            printf("%d: Coord: writing to pipe '%s'\n", (int)getpid(), &fromRoot);
            //These lines of code below are causing the bug to not get to sorter

            // BUG - Getting Stuck here
            // write(parent_fds1[1], &fromRoot, strlen(&fromRoot)); 
            // close(parent_fds1[1]);

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
            // EXECS
            if(sorterpid == 0) { 
                
                printf("%d: I am sorter node \n", getpid());
                execlp("./sorter1","sorter1", "0", "a", NULL); 
                

                //Exec here
                //If getpid % 2 == 0 -
                // char *args[]={"sorting1.c",NULL}; 
                // execve(args[0],NULL); 
                //Pass to merger

                 //If even-
                // char *args[]={"sorting2.c",NULL}; 
                // execve(args[0],NULL); 
 
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
    

                // printf("\n[sorter] pid %d from [parent] pid %d\n",getpid(),getppid());
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

// #include<stdio.h>
// #include<stdlib.h>
// #include "myhie.h"

// //Make multiple children using fork: https://www.geeksforgeeks.org/create-n-child-process-parent-process-using-fork-c/
// //Writing to child using pipe https://stackoverflow.com/questions/60104813/using-two-pipes-to-communicate-between-parent-process-and-child-process

// // Data record: resident-id, first name, last name, number of dependents, income and postal code

// //Root node: myhie program
// // 2. PIPE: Root passes coord: a) file of unsorted data records b) num workers c) numeric attribute of sorting (0,3,4,5) d) executable to be used for sorting
// // 3. SIG: Root receives SIGUSR1/SIGUSR2 signals from other processes 
// // 4. Root prints number of SIGUSR signals caught before root terminates
// // 5. Reports
// //      a) time each sorter took to sort its batch
// //      a) time the merger took to complete its job
// //      a) total time
// //      a) number of SIGUSR1 and SIGUSR2 signals the root has caught/seen before end of exec.
// ////// // 1. Creates single coordinator node - coord 


// //Coord node
// // PIPE: Coord passes to each sorter: a) the name of the file b) range of data c) which attr to use for sorting d)name of exec for sorting
// // SIG: When receive SIGUSR2, gracefully release resources
// ////// Creates merger node

// //Sorting node
// // Odd-indexed workers use Heap Sort and even-indexed workers use Bubble Sort
// // NAMED PIPE: Passes sorted data and time stats to merger node through name pipe
// // SIG: Sends SIGUSR1 to the root before termination

// //Merger node
// // NAMED PIPE: Receives(reads) ordered partial results from all pipes and creates outcome of entire sorting process
// // Prints out stats including time for each sorter to sort data
// // SIG: Sends SIGUSR2 to coord to show work completed
// ////// Created by coord

// // ./myhie -i InputFile -k NumOfWorkers -r -a AttributeNumber -o Order -s OutputFile
// //AttributeNumber is a valid numeric-id that designates the field on which sorting is to be carried out
// // Order is either ascending(a) or descending (d)
// // OutFile is the file in which the outcome of myhie could be saved

// int main(int argc, char*argv[]){

// while(1){

//     FILE *fpointer;
//     FILE *outfile;
//     int k;
//     char *atnum;
//     char *ord;
//     char *outfile_name;
//     outfile_name = argv[5];
//     fpointer = fopen(argv[1],"r"); 
//     outfile = fopen(outfile_name,"w");
//     k = atoi(argv[2]);
//     atnum = argv[3];
//     ord = argv[4];
//     pid_t coordpid;
//     pid_t sorterpid;
//     pid_t mergerpid;
//     pid_t pid;
//     int fd[2];
//     int fd1[2];
//     int root_pipe[2];
//     int root_pipe2[2];
//     int root_pipe3[2];
//     int root_pipe4[2];
//     int coord_pipe[2];
//     char printtest[20] = "print exec";
//     int ans1 = 49;
//     int status;
//     char fromRoot[100];
//     char str1[LENGTH], str2[LENGTH]; 
//     int fdnamed;
//     char arr1[LENGTH];

//     // FIFO file path 
//     char * myfifo = "myfifo"; 
  
//     mkfifo(myfifo, 0777); 
  
//     //Create coord node
//     coordpid = fork();

//     if (coordpid == -1) {
//         perror ( " Failed to fork " );
//         exit (1) ;
//         }
//     // In Root Node
//     if (coordpid > 0) {
        
//         // Create pipe in root
//         if (pipe(root_pipe) == -1) {
//             fprintf(stderr, "Pipe failed.\n");
//             return 1;
//         }
//         //Check if pipe failed
//         if (pipe(root_pipe) != 0) 
//         {
//             fprintf(stderr, "pipes failed!\n");
//             return EXIT_FAILURE;
//         }

//         printf("I am the root process");
//         fflush(stdout);

//         // close unwanted pipe ends by parent
//         close(root_pipe[0]);
//         // write from terminal to parent pipe FOR child to read
//         // Pass  a) file of unsorted data records b) num workers c) numeric attribute of sorting (0,3,4,5) d) executable to be used for sorting
//         printf("%d: Root: writing to pipe '%s'\n", (int)getpid(), argv[1]);
//         write(root_pipe[1],  argv[1], strlen(argv[1]));
//         close(root_pipe[1]); 
//     }
//     //In Coord Node
//     if (coordpid == 0) {
//           //Make merger node
//         mergerpid = fork();
//         //If in merger node
//         if (mergerpid == -1){
//             printf("Merger node failed \n");
//         }
//         if (mergerpid > 0){
//             printf("I am coord node (merger) \n");
//         }
//         if (mergerpid == 0){
//             // Open FIFO for Read only 
//             fdnamed = open(myfifo, O_RDONLY); 
    
//             // Read from FIFO 
//             read(fdnamed, arr1, sizeof(arr1)); 
    
//             // Print the read message 
//             printf("Merger node reading: %s\n", arr1); 
//             close(fdnamed);
//             //printf("Merger node pid: %d, Parent pid: %d \n", getpid(), getppid());
//             exit(0);
//         }
//         // Coord makes k sorter nodes
//         for(int i = 0; i < k; i++){ 

//             sorterpid = fork();
            
//             //Check if fork failed
//             if (sorterpid == -1)
//             {
//                 perror("Failed to fork");
//                 exit(1);
//             }
    
//             // Sorterpid == 1 (In Coord)
//             if (sorterpid > 0){
               
//                 // Create pipe in coord (parent)
//                 if (pipe(coord_pipe) == -1) {
//                     fprintf(stderr, "Pipe failed.\n");
//                     return 1;
//                 }

//                 // Check if pipe failed
//                 if (pipe(coord_pipe) != 0)
//                 {
//                     fprintf(stderr, "pipes failed!\n");
//                     return EXIT_FAILURE;
//                 }

//                 printf("%d: I reached the coord :)\n", (int)getpid());

//                 fflush(stdout);
//                 // close unwanted pipe ends by child
//                 close(root_pipe[1]);

//                 // // close unwanted pipe ends by child
//                 // close(root_pipe3[1]);

//                 // // close unwanted pipe ends by child
//                 // close(root_pipe4[1]);

//                 // read from parent pipe - arg1
//                 int n = read(root_pipe[0], fromRoot, sizeof(fromRoot) - 1);
//                 fromRoot[n] = '\0';
//                 printf("%d: Coord: read from root pipe arg1- %s \n", (int)getpid(), fromRoot);
//                 close(root_pipe[0]);

//                 // close unwanted pipe ends by parent
//                 fflush(stdout);
//                 close(coord_pipe[0]);

//                  // arg1 write from terminal to parent pipe FOR child to read
//                 printf("%d: Coord: writing to pipe '%s'\n", (int)getpid(), fromRoot);
//                 write(coord_pipe[1], fromRoot, strlen(fromRoot));
//                 close(coord_pipe[1]);

//                 // // close unwanted pipe ends by child
//                 // close(root_pipe2[1]);

//                 //  // read from parent pipe - arg2
//                 // n = read(root_pipe2[0], fromRoot, sizeof(fromRoot) - 1);
//                 // fromRoot[n] = '\0';
//                 // printf("%d: Coord: read from root pipe arg2- %s\n", (int)getpid(), fromRoot);
//                 // close(root_pipe2[0]);

//                 // // arg2 write from terminal to parent pipe FOR child to read
//                 // printf("%d: Coord: writing to pipe '%s'\n", (int)getpid(), fromRoot);
//                 // write(coord_pipe[1], fromRoot, strlen(fromRoot));
//                 // close(coord_pipe[1]);

               

//                 //  // read from parent pipe - arg3
//                 // int n = read(root_pipe3[0], fromRoot, sizeof(fromRoot) - 1);
//                 // fromRoot[n] = '\0';
//                 // printf("%d: Coord: read from root pipe arg3- %s\n", (int)getpid(), fromRoot);
//                 // close(root_pipe3[0]);

//                 //  // read from parent pipe - arg4
//                 // int n = read(root_pipe4[0], fromRoot, sizeof(fromRoot) - 1);
//                 // fromRoot[n] = '\0';
//                 // printf("%d: Coord: read from root pipe arg4- %s\n", (int)getpid(), fromRoot);
//                 // close(root_pipe4[0]);

            

//                 //  // close unwanted pipe ends by parent
//                 // fflush(stdout);
//                 // close(coord_pipe2[0]);

//                 //  // close unwanted pipe ends by parent
//                 // fflush(stdout);
//                 // close(coord_pipe3[0]);

//                 //  // close unwanted pipe ends by parent
//                 // fflush(stdout);
//                 // close(coord_pipe4[0]);

              

              
//             }

//             //In sorter node
//             if(sorterpid == 0) { 
//             //     printf("%d: I reached the sorter :)\n", (int)getpid());

//                 //exec()

//                 // close unwanted pipe ends by child
//                 fflush(stdout);
//                 close(coord_pipe[1]);

//                 // read from parent pipe
//                 char fromCoord[100];
//                 int n = read(coord_pipe[0], fromCoord, sizeof(fromCoord) - 1);
//                 fromCoord[n] = '\0';
//                 printf("%d: Sorter: read from coord pipe- %s \n", (int)getpid(), fromCoord);
//                 //TODO - Execute heapsort.c (print out executed)
//                 // TODO - Play with heapsort to use sorting struct
//                 char * argvarr[] = {"5","10","15"};
//                 execv("heapsort.c",argvarr);
//                 close(root_pipe[0]);

//                 // Open FIFO for write only 
//                 fdnamed = open(myfifo, O_WRONLY); 
        
//                 // Take an input from user. 
//                 // LENGTH 200 is maximum length 
//                 // fgets(arr2, LENGTH, stdin); 
        
//                 // Write the input arr2 on FIFO 
//                 // and close it 
//                 write(fdnamed, fromCoord, strlen(fromCoord)+1);  //+1 to account for \0 at the end of strings in C
//                 close(fdnamed); 
    

//                 // printf("\n[sorter] pid %d from [parent] pid %d\n",getpid(),getppid());
//                 exit(0); 
//             }
         
//         }  
//         //Still in coord node (coordpid = 0)
//         // Sorterpid > 0 - coordnode waits for each sorter to finish
//         // for(int i=0;i<k;i++){
//         //     //Wait for sorter to finish
//         //     wait(NULL);   
//         // }

//         // Coordpid == 1 (In Root)
    
    
//     return 0;

// }
// }  
// }  

      