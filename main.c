#include <stdio.h>
    
int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Rerun the program and enter input as follows: ./%s ./file location", argv[0]);
        return 1;
    }
    
    char *filepath = argv[1];
    printf("hmm, file is getting parsed.");
    
    FILE *file = fopen(filepath, "r");
    if(file == NULL)
    {
        perror("file doesn't open, try again.");
        return 1;
    }
    
    
    
    
    fclose(file);
    return 0;
}
