#include <stdio.h>
#include <string.h>

void header(void) 
{ 
    printf("\n%-16s %-6s %-12s %-10s %-16s %-15s %-5s\n", "TIMESTAMP", "PID", "EVENT TYPE", "STATUS", "USERNAME", "SOURCE IP", "PORT");  
}

int main(int argc, char *argv[])
{
    if(argc < 2) return printf("Rerun the program and enter input as follows: ./%s ./file location\n", argv[0]), 1;

    // char *filepath = argv[1];
    printf("hmm, file is getting parsed, kindly wait a moment...\n");

    FILE *f = fopen(argv[1], "r");
    if(!f) return perror("file doesn't open, try again."), 1;

    char line[512];

    int ok = 0;
    int fail = 0;

    header();

    while (fgets(line, sizeof(line), f))
    {
        if(strstr(line, "Accepted password") || strstr(line, "Failed password"))
        {
            char *status = NULL;
            char username[64] = "NULL", ip[64] = "NULL", timestamp[16] = "NULL";
            int port = 0, pid = 0;
            char *etype = "SSH Auth";

            char *check = strstr(line, "Accepted password");

            strncpy(timestamp, line, 15); 
            timestamp[15] = '\0';

            check ? ok++ : fail++;

            char *p;
            if ((p = strstr(line, "[")))     sscanf(p + 1, "%d", &pid);
            if ((p = strstr(line, "from "))) sscanf(p + 5, "%63s port %d", ip, &port);
            if ((p = strstr(line, "for ")))  sscanf(p + (strncmp(p + 4, "invalid user ", 13) ? 4 : 17), "%63s", username);

            printf("%-16s %-6d %-12s %-10s %-16s %-15s %-5d\n", timestamp, pid, etype, check ? "Success" : "Failure", username, ip, port);
        }
    }    

    fclose(f);

    printf("\n====AUTH LOG SUMMARY====\n");
    printf("Successful logins    : %d\n", ok);
    printf("Failed logins        : %d\n", fail);
    printf("Total login attempts : %d\n", ok + fail);

    return 0;
}
