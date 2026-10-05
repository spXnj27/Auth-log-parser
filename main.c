#include <stdio.h>
#include <string.h>

void header(void)
{
    printf("\n%-16s %-6s %-12s %-10s %-16s %-15s %-5s\n", "TIMESTAMP", "PID", "EVENT TYPE", "STATUS", "USERNAME", "SOURCE IP", "PORT");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Rerun the program and enter input as follows: ./%s ./file location\n", argv[0]);
        return 1;
    }

    printf("hmm, file is getting parsed, kindly wait a moment...\n");

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("file doesn't open, try again");
        return 1;
    }

    FILE *csv = fopen("auth_events.csv", "w");
    if (csv)
        fprintf(csv, "timestamp,pid,event,status,username,ip,port\n");

    char line[512];
    int ok = 0;
    int fail = 0;

    #define MAX 200

    char user_names[MAX][64];
    int  user_counts[MAX];
    int  n_users = 0;

    char ip_names[MAX][64];
    int  ip_counts[MAX];
    int  n_ips = 0;

    header();

    while (fgets(line, sizeof(line), f)) {

        if (!strstr(line, "Accepted password") && !strstr(line, "Failed password"))
            continue;

        char timestamp[32] = "Nothing, atm";
        char username[64]  = "Nothing, atm";
        char ip[64]        = "Nothing, atm";
        int  pid           = 0;
        int  port          = 0;
        const char *status = strstr(line, "Accepted") ? "Success" : "Failure";
        const char *etype  = "SSH Auth";

        strncpy(timestamp, line, 15);
        timestamp[15] = '\0';

        char *p = strstr(line, "[");
        if (p)
            sscanf(p + 1, "%d", &pid);

        p = strstr(line, "for ");
        if (p) {
            p += 4;
            if (strncmp(p, "invalid user ", 13) == 0)
                p += 13;
            sscanf(p, "%63s", username);
        }

        p = strstr(line, "from ");
        if (p)
            sscanf(p + 5, "%63s port %d", ip, &port);

        if (strstr(line, "Accepted"))
            ok++;
        else
            fail++;

        int found = 0;
        for (int i = 0; i < n_users; i++) {
            if (strcmp(user_names[i], username) == 0) {
                user_counts[i]++;
                found = 1;
                break;
            }
        }
        if (!found && n_users < MAX) {
            strcpy(user_names[n_users], username);
            user_counts[n_users] = 1;
            n_users++;
        }

        found = 0;
        for (int i = 0; i < n_ips; i++) {
            if (strcmp(ip_names[i], ip) == 0) {
                ip_counts[i]++;
                found = 1;
                break;
            }
        }
        if (!found && n_ips < MAX) {
            strcpy(ip_names[n_ips], ip);
            ip_counts[n_ips] = 1;
            n_ips++;
        }

        if (csv)
            fprintf(csv, "\"%s\",%d,%s,%s,%s,%s,%d\n",timestamp, pid, etype, status, username, ip, port);

        printf("%-16s %-6d %-12s %-10s %-16s %-15s %-5d\n",timestamp, pid, etype, status, username, ip, port);
    }

    fclose(f);
    if (csv)
        fclose(csv);

    printf("\n==== AUTH LOG SUMMARY ====\n");
    printf("Successful logins    : %d\n", ok);
    printf("Failed logins        : %d\n", fail);
    printf("Total login attempts : %d\n", ok + fail);

    printf("\n==== TOP TARGETED USERNAMES ====\n");
    for (int i = 0; i < n_users; i++)
        printf("%-20s : %d\n", user_names[i], user_counts[i]);

    printf("\n==== TOP SOURCE IPs ====\n");
    for (int i = 0; i < n_ips; i++)
        printf("%-20s : %d\n", ip_names[i], ip_counts[i]);

    return 0;
}
