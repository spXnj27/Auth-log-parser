#include <stdio.h>
#include <string.h>
    
int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Rerun the program and enter input as follows: ./%s ./file location", argv[0]);
        return 1;
    }
    
    char *filepath = argv[1];
    printf("hmm, file is getting parsed.\n");
    
    FILE *file = fopen(filepath, "r");
    if(file == NULL)
    {
        perror("file doesn't open, try again.");
        return 1;
    }
    
    char line[512];
    
    int successful_login = 0;
    int failed_login = 0;
    
    while (fgets(line, sizeof(line), file))
    {
        if(strstr(line, "Accepted password"))
        {
            successful_login++;
        }
        else if(strstr(line, "Failed password"))
        {
            failed_login++;
        }
    }
    
    
    
    
    fclose(file);
    
    printf("====AUTH LOG SUMMARY====\n");
    printf("Successful logins: %d\n", successful_login);
    printf("Failed logins    : %d\n", failed_login);
    
    
    return 0;
}
