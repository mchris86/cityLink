/**
 * @mainpage CityLink
 * @author Michalis Christodoulou
 * 
 * The program reads from an input file a Adjacent Table that shows the routes between cities and
 * prints this table to the console. From the given table, it creates a list of the connected cities, 
 * and from this list creates the Transitive Closure Table. Using the transitive Closure Table, the program
 * can find if there is a path for a given route and print it to the screen. It can also print the Transitive Closure
 * to the console, or write it in an output file depending on the user's preferences.
 * 
 * compile: cityLink.c -o cityLink
 * 
 * executable: cityLink
 * 
 * -i <filename> : name of the input file (mandatory)
 * -r <source_city>,<destination_city> : determine the source city and destination city to find if there is a route between them
 * -p : print the transitive closure to the console
 * -o : write the transitive closure in a file with name out-<filename>.txt
 * 
 * Execution Example: ./cityLink -i cities1.txt -r 0,1 -p
 *                    ./cityLink -i cities1.txt -opr 0,1 
 * 
 * 
 * @bug No known bugs 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>
/**
 * @brief Reads the Neighbor Table and its size from a file given from the command line.
 * 
 * The function reads the Neighbor Table(A) and its size from a file specified in the command line
 * and stores it in a dynamic allocated 2D array.
 * 
 * @param A Array of pointers that will store the Neighbour Table.
 * @param ivalue The input file, which is given from the command line, after option-character 'i'.
 * @param N Pointer to the variable that will store the size of the neihbour table A.
 * @return true Return true if the file was succesfully read.
 * @return false Return false if function was unable to read file or allocate memory for the dynamic array A.
 */
bool readFromFile(int ***A, char *ivalue, int *N);
/**
 * @brief Creates a list of neighbor pairs from the neighbor table.
 * 
 * The function creates a list, stored as a dynamically allocated 1D Array
 * of neighbour pairs from the neighbour table and returns it.
 * Each pair is stored in consecutive places in the array with the start city always having
 * an even index and target city an odd index.
 * 
 * @param A The neighbour table.
 * @param N The dimensions of neighbor table.
 * @param sizeOfR Pointer to the variable that will store the size of the RList.
 * @return int* Returns a dynamically allocated array that contains neighbor pairs(RList).
 */
int *createRList(const int **A, int N, int *sizeOfR);

/**
 * @brief Create a transitive closure (R*) from the list of neighbor pairs.
 * 
 * This function extends the previous Neighbor Pairs array to a transitive
 * closure array. Dynamic memory allocation is used to extend the array.
 * 
 * @param R Array that contains the list of neighbor pairs.
 * @param sizeOfR Pointer to the variable that stores the size of R.
 * @return int*  A dynamiccaly allocated array that contains the transitive closure (R*).
 */
int *createTransClosure(int *R, int *sizeOfR);

/**
 * @brief Finds the path between two cities.
 * 
 * This function finds and prints a path between two cities from start to target if found.
 * 
 * @param R The transitive closure (R*) table.
 * @param sizeOfR The size of the transitive closure array.
 * @param startCity The source city.
 * @param targetCity The target city.
 * @return true Returns true if path exists.
 * @return false Returns false if path does not exist, or the function is unable to allocate memory for the path array.
 */
bool findPath(const int *R, int sizeOfR, int startCity, int targetCity);

/**
 * @brief Reads commands line arguments and sets flags for all the option-characters.
 * 
 * This function uses <getopt.h> library to parse command line arguments, and set flags 
 * for all the possible option-characters. It alse handles cases where the user chooses 
 * non-legitimate character options.
 * 
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @param iflag Pointer to the input file flag.
 * @param rflag Pointer to the route pair flag that is to be checked.
 * @param pflag Pointer to the print flag.
 * @param oflag Pointer to the output to file flag.
 * @param ivalue Pointer to the input file name.
 * @param rvalue Pointer to the route pair specification.
 * @return true If the arguements are valid.
 * @return false If the arguments are not valid.
 */
bool readArguments(int argc, char **argv, int *iflag, int *rflag, int *pflag, int *oflag, char **ivalue, char **rvalue);

/**
 * @brief Prints the neighbour table.
 * 
 * The function prints the neighbor table to the standard output.
 * 
 * @param A The neighbor table.
 * @param N The size of the neighbor table.
 */
void printNeighbTable(int **A, int N);

/**
 * @brief Prints the transitive closure table (R*).
 * 
 * The funtion prints the transitive closure table(R*) to the standard output.
 * 
 * @param R The transitive closure (R*).
 * @param sizeOfR The size of the transitive closure array.
 */
void printTransClosure(const int *R, const int sizeOfR);

/**
 * @brief Write the transitive closure table to an output file.
 * 
 * The function writes the transitive closure table (R*) to a file with name out-<filename>.txt 
 * where <filename> is the name of input file, given from the user in a command line argument.
 * 
 * @param R Transitive closure array (R*).
 * @param sizeOfR The size of the (R*) table.
 * @param inFileName The name of the input file, needed to specify the output file name.
 * @return true Returns true if the table is succesfully written to the output file, otherwise returns false.
 */
bool writeFile(int *R, int sizeOfR, char *inFileName);

/**
 * @brief The main function of the program.
 * 
 * The main function organises the execution of various actions, such as reading input arguments,
 * reading from input file, processing the neighbor table, creating the transitive closure table,
 * finding paths and writing output files.
 * 
 * @param argc The number of command line arguments.
 * @param argv An array of strings representing the command line arguments.
 * @return int returns 0 after successful program execution.
 */
int main(int argc, char **argv){

    int **A = NULL, *R = NULL, N, sizeOfR=0;
    int iflag = 0, rflag = 0, pflag = 0, oflag = 0;
    char *ivalue = NULL, *rvalue = NULL;

    if(!readArguments(argc, argv, &iflag, &rflag, &pflag, &oflag, &ivalue, &rvalue))
        return -1;                      // Exit program in case of argument reading failure

    if(!readFromFile(&A, ivalue, &N))
        return -1;                      // Exit program in case of issue reading from input file
    printNeighbTable(A,N);              // Print the neighbor table A

    if((R = createRList((const int **)A, N, &sizeOfR)) == NULL)
        return -1;                      // Exit program if creating the RList of pairs fails
    if((R = createTransClosure(R, &sizeOfR)) == NULL)
        return -1;                      // Exit program if creating the transitive closure fails

    if(pflag)
        printTransClosure(R,sizeOfR);   // Print the transitive closure if the 'p' flag is set to 1

    if(rflag){                          // Find and print path if 'r' flag is set to 1 and the path exists.
        int startCity = rvalue[0]-'0';  // Read start city and convert from char to integer 
        int targetCity = rvalue[2]-'0'; // Read target city and convert from char to integer
        findPath(R, sizeOfR, startCity, targetCity); // Call findPath method
    }

    if(oflag)                           // Write the transitive closure table to output file if 'o' flag is set to 1
        if(!writeFile(R, sizeOfR, ivalue))
            return -1;                  // Exit program if writing the R* table to an output file fails
    int i;
    for(i = 0; i < N; i++)              // Free memory
        free(A[i]);
    free(A);
    free(R);

    return 0;                           
}

bool readFromFile(int ***A, char *ivalue, int *N){  // Reads the Neibhour table and its size from a file given from the command line
    FILE *fp;                                       // File Pointer for reading input from file
    *N = 0;                                         // Size of the array
    if((fp = fopen(ivalue,"r")) == NULL){           // Open file, if unable to open print error message and return false
        printf("Input file can not be read!\n");
        return false;
    }
    *N = fgetc(fp)-'0';                             // Read character size and convert to integer by subtracting '0'

    (*A) = (int **) malloc(sizeof(int *) * (*N));   // Allocate memory for array of pointers
    if((*A) == NULL){                               // If unable to allocate memory print error message and return false
        printf("Error: Unable to allocate enough memory\n");
        return false;
    }
    int i, j;                                       // Loop counters
    for(i = 0; i < (*N); i++){                      // Allocate memory for A[i]
        (*A)[i] = (int *) malloc(sizeof(int) * (*N));
        if((*A) == NULL){                           // If unable to allocate memory print error message and return false
            printf("Error: Unable to allocate enough memory\n");
            return false;
        }   
    }
    // Read neihbour table
    fgetc(fp);                                      // Ignore new line after size(N)
    for(i = 0; i< (*N); i++){
        for(j = 0; j< (*N); j++){
            int k = fgetc(fp);                      // Ignore space
            (*A)[i][j] = k-'0';
            fgetc(fp);
        }  
    }
    return true;                                    // Return true if file was successfully read
}

int *createRList(const int **A, int N, int *sizeOfR){
    int *p = NULL;                                  // Will be used to initialize RList Array
    int *RList = NULL, neihbourCount = 0;

    p = (int *) malloc(sizeof(int) * 2*(N*N-N));    // Allocate memory for RList array
    if(p == NULL){                                  // If malloc failed print error message and returns false to exit the program
        printf("Error: malloc failed\n");
        return NULL;
    }
    RList = p;
    
    // Each '1' in the Neighbour Table is one pair, so we need to count pairs
    // and initialize the RList with our pairs.
    int i,j;                                        // Loop Counters
    for(i = 0; i < N; i++){                         
        for(j = 0; j < N; j++){
            if(A[i][j] == 1){       
                neihbourCount++;
                *p = i;
                p++;
                *p = j;
                p++;
            }
        }
    }
    *sizeOfR = 2*neihbourCount;                     // Size of the dynamic array is double the amount of neighbours 
                                                    // because it holds 2 elements for each pair
    
    int *tmp = realloc(RList, sizeof(int) * (*sizeOfR));  // Change the size of RList array, so it only takes tha memory it needs
    if(tmp == NULL){                                // If realloc failed print error message and return false to exit program in main
        printf("Error: realloc failed\n");
        return NULL;
    }
    RList = tmp;                                    

    return RList;
}

int *createTransClosure(int *R, int *sizeOfR){
    bool changed = true;                           // Variable to control when the while loop stops
                                                   // At the start of each while loop it is set to false but each time
                                                   // a change is made to R, it is set to true, so the while loop continues
    while(changed){
        changed = false;
        int initialSize = *sizeOfR;                 // Save the initial size of R, because in the loop it will be changed and and its needed as a limit in our loops
        int i,j,k;
        for(i = 0; i < initialSize; i+=2){ 
            int u = R[i];
            int v = R[i+1];
            for(j = 0; j < initialSize; j+=2){

                int y = R[j];
                int w = R[j+1];
                
                bool alreadyExists = false;
                  
                if(y == v && u != w ){
                    if(u!=y && v!=w){                          // Don't check itself
                        for(k = 0; k < *sizeOfR; k+=2)  
                            if(R[k] == u && R[k+1] == w){      // Check for duplicates
                                alreadyExists = true;
                            }
                    }
                    if(!alreadyExists){
                        int *tmp = (int *)realloc(R,sizeof(int)*(*sizeOfR+2)); // Increase the size of the array so that it fits 2 more elements
                        if(tmp == NULL){
                            printf("Error: realloc failed\n");
                            return NULL;
                        }
                        R = tmp;                              // Assign tmp with the renewed memory to pointer R
                        R[*sizeOfR] = u;                      // Add link from city u
                        R[*sizeOfR+1] = w;                    // to city w
                        *sizeOfR+=2;
                        changed = true;                       // Run another while loop since the list is still changing
                    }
                }
            }
        }
    }
    return R;
}

bool findPath(const int *R, int sizeOfR, int startCity, int targetCity){
    int *Path = NULL;                                         // Dynamic array to store the path
    int startIndex = 0, targetIndex = 0, pathSize = 0;        // Variables for tracking the path and its size
    bool found = false;                                       // Flag variable to indicate if the path is found
    int i;
    for(i = 0; i < sizeOfR; i+=2){                            // Traverse through the array to find if the path exists
        if(R[i] == startCity && R[i+1] == targetCity)
            found = true;      
    }
    if(!found){                                               // If no path is found, print an error message and return false.
        printf("No Path Exists!\n");
        return false;
    }
    bool pathComplete = false;                                // Flag variable to indicate if the path is complete
    // Find the start of the path and allocate memory in Path array to store it
    for(i = 0; i < sizeOfR; i+=2){ 
        if(R[i] == startCity){
            Path = (int *) malloc(sizeof(int));              // Allocate memory for an integer in the array
            if(Path == NULL){                                // If unable to allocate memory print error message and return false.
                printf("Error: malloc failed in findPath\n");
                return false;
            }
            Path[pathSize] = startCity;                      // The path array holds the value of start city
            pathSize++;                                      // Increment the size of the path
            startCity = R[i+1];                              // Update the startCity its target pair, its following element in the array
            //Check for direct connection
            if(startCity == targetCity){                     // If startCity is equal to targetCity the path is complete and we need to store targetCity to the path, as the final destination.
                    int *tmp = (int *) realloc(Path, sizeof(int)* (pathSize+1)); // Allocate memory in the path array
                    if(tmp == NULL){                         // If unable to allocate memory print error message and return false
                        printf("Error: realloc failed\n");
                        return false;
                    }
                    Path = tmp;
                    Path[pathSize] = targetCity;             // Store target city to path
                    pathSize++;                              // Increment path size
                    pathComplete = true;                     // Set pathComplete flag to true, to not continue the checks.
                    break;
                }
            break;
        }
    }
    while(!pathComplete){                                    // Repeat until path is complete
        for(i = 0; i < sizeOfR; i+=2){
            bool nextTargetAlreadyInPath = false;            // Flag variable to indicate if the route that that we are adding to the path is already in it,
                                                             // to not add it again, and skip and search for another path.
            if(R[i] == startCity){                           
                int j;
                for(j = 0; j < pathSize; j++){               // Check if the element following the startCity is already in the path
                    if(R[i+1]==Path[j])                      // If it is, set the flag to true
                        nextTargetAlreadyInPath = true;      
                }
                if(!nextTargetAlreadyInPath){                // If the element following the start city is not already in the path
                    int *tmp = (int *) realloc(Path, sizeof(int)* (pathSize+1)); // Allocate memory to store another route in the path.
                    if(tmp == NULL){                         // If unable to allocate memory print error message and return false
                        printf("Error: realloc failed\n");
                        return false;
                    }
                    Path = tmp;
                    Path[pathSize] = startCity;        // The path array holds the value of start city
                    pathSize++;
                    startCity = R[i+1];                // Start city is set to the element that follows it, to search for it in the next loop
                    if(startCity == targetCity){       // If startCity is equal to targetCity the path is complete and we need to store targetCity to the path, as the final destination.
                        int *tmp = (int *) realloc(Path, sizeof(int)* (pathSize+1)); // Allocate memory in the path array
                        if(tmp == NULL){                 // If unable to allocate memory print error message and return false
                            printf("Error: realloc failed\n");
                            return false;
                        }
                        Path = tmp;
                        Path[pathSize] = targetCity;
                        pathSize++;
                        pathComplete = true;
                        break;
                    }
                }
            }
        }
    }

    // Print the Path to the standard output
    printf("Yes path exists!\n");
    for(i = 0; i < pathSize; i++){
        printf("%d",Path[i]);
        if(i<pathSize-1)
            printf(" => ");
    }
    printf("\n");
    free(Path);   // Once path is printed, allocated memory for the path array is freed

    return true;
}

bool readArguments(int argc, char **argv, int *iflag, int *rflag, int *pflag, int *oflag, char **ivalue, char **rvalue){
    int index;
    int c;

    opterr = 0;        // Disable getopt error messages
        if(argc == 1){ // If no command line arguements are given, print error message and return false.
            printf("No command line arguments given!\n"
                "Usage: <executable> -i <inputfile> [-r <source >,<destination> -p -o]\n");
                return false;
        }
        while ((c = getopt(argc, argv, "i:r:po")) != -1)
            switch (c) {
                case 'i':
                    *iflag = 1;       // Set the input file flag to 1
                    *ivalue = optarg; // Store the arguement of input file option.
                    break;
                case 'r':
                    *rflag = 1;       // Set the route pair flag to 1
                    *rvalue = optarg; // Store the argument of the rout pair option, the rout pair specification
                    break;
                case 'p':       
                    *pflag = 1;       // Set the print flag to 1
                    break;
                case 'o':
                    *oflag = 1;       // Set the output to file flag to 1
                    break;
                case '?':             // Handle invalid command line arguments
                    if (optopt == 'i'){
                        fprintf(stderr, "/.cityLink: option requires an argument -- '%c'\n"
                            "Usage: <executable> -i <inputfile> [-r <source >,<destination> -p -o]\n", optopt);
                            return false;
                    }
                    else if (optopt == 'r'){
                        fprintf(stderr, "/.cityLink: option requires an argument -- '%c'\n"
                            "Usage: <executable> -i <inputfile> [-r <source >,<destination> -p -o]\n", optopt);
                        return false;
                    }
                    else if (isprint (optopt)){
                        fprintf (stderr, "/.cityLink: invalid option -- '%c'\n"
                            "Usage: <executable> -i <inputfile> [-r <source >,<destination> -p -o]\n", optopt);
                        return false;
                    }
                    else {
                        fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                        return false;
                    }
                    return 1;
                default:
                    abort ();
        }
        if(!iflag){
            printf("No input file given!\n");
            return false;
        }
        for (index = optind; index < argc; index++){
            printf ("Non-option argument %s\n", argv[index-1]);
            return false;
        }
    return true;
}

void printNeighbTable(int **A, int N){
    int i,j;
    printf("Neighbor table\n");
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++)
            printf("%d ",A[i][j]);
        printf("\n");
    }
    printf("\n");
}

void printTransClosure(const int *R, const int sizeOfR){
    int i;
    printf("\nR* Table\n");
    for(i = 0; i < sizeOfR; i++){
        if(i%2 == 0 && i>0)
            printf("\n");
        else if(i>0)
            printf(" -> ");
        printf("%d",R[i]);
    }
    printf("\n");
}

bool writeFile(int *R, int sizeOfR, char *inFileName){
    FILE *fpout = NULL;                                                  // File pointer for output file.
    char *str = "out-";                                                  // This string literal will be added to the output file name.
    char *outFileName = NULL;
    // Generate the output file name by adding "out-" before the input file name           
    outFileName = (char *) malloc(strlen(str) + strlen(inFileName) +1);  // Allocate enough space for the output file name, to combine the 2 strings and the null character
    if(outFileName == NULL){                                             // If unable to allocate memory print error message and return false
        printf("Error: Can't write in out-%s",inFileName);
        return false;
    }

    strcpy(outFileName, str);                                             // Copy "out-" to the output file name.
    strcat(outFileName, inFileName);                                      // Append the input file name to the output file name.
    if((fpout = fopen(outFileName, "w")) == NULL){                        // If file open fails,
        printf("Error opening file\n");                                   // Print eror message and return false.
        return false;                                               
    }
    // Write the contents of R* table to the output file
    int i;
    fprintf(fpout, "R* Table\n");
    for(i = 0; i < sizeOfR; i++){
        if(i%2 == 0 && i>0)
            fprintf(fpout, "\n");
        else if(i>0)
            fprintf(fpout, " -> ");
        fprintf(fpout, "%d",R[i]);
    }
    fprintf(fpout, "\n\n");

    printf("Saving %s...\n",outFileName);                                 // Print message indicating that the file is being saved.

    free(outFileName);
    fclose(fpout);                                                        // Close the output file
    return true;                                                          // Return true to indicate that the table is written succesfully to the output file.
}

