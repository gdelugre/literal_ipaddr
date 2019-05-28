#include <stdio.h>
#include "ipaddr.h"

int main() 
{
    int s = socket(AF_INET, SOCK_STREAM, 0);

    const struct sockaddr_in dst {
        .sin_family = AF_INET,
        .sin_port   = 1234_ipport,
        .sin_addr   = "127.0.0.1"_ipaddr
    };

    connect(s, (struct sockaddr *)&dst, sizeof(dst));

    return 0;
}
