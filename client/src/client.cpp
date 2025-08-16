#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <netdb.h> // For getaddrinfo
#include <cstring> // For memset

int main() {
    int sock = 0;
    struct addrinfo hints, *res, *p;
    int status;
    int port = 8080;
    std::string server_name = "tcp-server-container"; // Use the server container name

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(server_name.c_str(), std::to_string(port).c_str(), &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = res; p != NULL; p = p->ai_next) {
        if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock);
            perror("client: connect");
            continue;
        }

        break; // if we get here, we successfully connected
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    freeaddrinfo(res); // all done with this structure

    std::cout << "Connected to server." << std::endl;

    // Send GET /hello request
    std::string hello_message = "GET /hello\n";
    send(sock, hello_message.c_str(), hello_message.length(), 0);
    std::cout << "Message sent: " << hello_message << std::endl;

    // Receive response for /hello
    char buffer_hello[1024] = {0};
    read(sock, buffer_hello, 1024);
    std::cout << "Server response for /hello: " << buffer_hello << std::endl;

    // Close the first socket after the first request
    close(sock);
    std::cout << "Closed first connection to server." << std::endl;

    // Re-establish connection for GET /bye request
    // Re-declare variables for the new connection
    int sock_bye = 0;
    struct addrinfo hints_bye, *res_bye, *p_bye;
    int status_bye;

    memset(&hints_bye, 0, sizeof hints_bye);
    hints_bye.ai_family = AF_UNSPEC;
    hints_bye.ai_socktype = SOCK_STREAM;

    if ((status_bye = getaddrinfo(server_name.c_str(), std::to_string(port).c_str(), &hints_bye, &res_bye)) != 0) {
        fprintf(stderr, "getaddrinfo for /bye: %s\n", gai_strerror(status_bye));
        return 1; // Exit if resolution fails for second request
    }

    for(p_bye = res_bye; p_bye != NULL; p_bye = p_bye->ai_next) {
        if ((sock_bye = socket(p_bye->ai_family, p_bye->ai_socktype, p_bye->ai_protocol)) == -1) {
            perror("client /bye: socket");
            continue;
        }

        if (connect(sock_bye, p_bye->ai_addr, p_bye->ai_addrlen) == -1) {
            close(sock_bye);
            perror("client /bye: connect");
            continue;
        }
        break;
    }

    if (p_bye == NULL) {
        fprintf(stderr, "client /bye: failed to connect\n");
        return 2; // Exit if connection fails for second request
    }
    freeaddrinfo(res_bye);
    std::cout << "Re-connected to server for /bye request." << std::endl;

    // Send GET /bye request on the new connection
    std::string bye_message = "GET /bye\n";
    send(sock_bye, bye_message.c_str(), bye_message.length(), 0);
    std::cout << "Message sent: " << bye_message << std::endl;

    // Receive response for /bye
    char buffer_bye[1024] = {0};
    read(sock_bye, buffer_bye, 1024);
    std::cout << "Server response for /bye: " << buffer_bye << std::endl;

    // Close the second socket
    close(sock_bye);
    std::cout << "Closed second connection to server." << std::endl;

    return 0;
}
