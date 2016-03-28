#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mailboxapp_application.h"
#include "mailboxapp_process.h"

int do_mailboxapp(int argc, char* argv[])
{
    if (argc < 8)
    {
        fprintf(stderr, "Usage: %s %s <process name>, <key>, <window size>, <max delay>, <timeout>, <rate>\r\n",
                        argv[0], argv[1]);
        return -1;
    }

    // Initialize basic information and write it to a file.
    if (mailboxapp_init(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6])) < 0)
    {
        fprintf(stderr, "Failed in mailboxapp_init.\r\n");
        return -1;
    }

    char role[MAX_NAME_LEN];
    char receiver[MAX_NAME_LEN];
    char data[MAX_SIZE];

    while(1)
    {
        printf("\nRole (sender/receiver): ");
        scanf("%s", role);

        if (strcmp(role, "sender") == 0)
        {
            // Get the receiver name and the data.
            printf("\nReceiver name: ");
            scanf("%s", receiver);
            printf("Data: ");
            scanf("%s", data);
            if (mailboxapp_send_message(receiver, data) < 0)
            {
                printf("Failed sending data to %s\n", receiver);
            }
        }
        else if (strcmp(role, "receiver") == 0)
        {
            if (mailboxapp_receive_message(data) >= 0)
            {
                printf("Message: %s\n", data);
            }
        }
        else
        {
            printf("Invalid role: %s\n", role);
        }
    }
    return 0;
}
