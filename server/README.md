# C++ TCP Server Explanation

This document provides a detailed explanation of the C++ TCP server application (`server/src/server.cpp`), which is designed to listen for incoming network connections, accept them, and handle basic message exchange.

## Server's Role

The server's primary role in a TCP communication is to be a passive listener, waiting for clients to initiate a connection. It sets up a specific "address" (IP address and port) on which it will listen. When a client attempts to connect, the server accepts the connection, establishing a dedicated communication channel for that client. It then handles requests from the client and sends back responses.

## Key Components and Flow (`server/src/server.cpp`)

The server's code performs the following main steps:

1.  **Socket Creation:** The server first creates a socket, which acts as the main listening point for incoming connections.

    ```cpp
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        // Error handling
    }
    ```

2.  **Binding to Address and Port:** The newly created socket is then bound to a specific IP address and port. `INADDR_ANY` is used to allow the server to listen on all available network interfaces.

    ```cpp
    sockaddr_in address;
    int port = 8080;
    // ... (setting up address details)
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        // Error handling
    }
    ```

3.  **Listening for Connections:** The server puts its socket into a listening state, indicating that it's ready to accept incoming client connections. A backlog queue size is specified to handle multiple pending connections.

    ```cpp
    if (listen(server_fd, 3) < 0) {
        // Error handling
    }
    std::cout << "Server listening on port " << port << std::endl;

    while (true) { // Main server loop to continuously accept connections
        // 4. Accept a client connection
        int client_socket;
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("accept");
            continue; // Continue listening for other connections
        }
        std::cout << "Client connected!" << std::endl;

        // 5. Receive data from the client
        char buffer[1024] = {0};
        long valread = read(client_socket, buffer, 1024);
        std::string client_message(buffer); // Convert char array to std::string

        std::cout << "Client message: " << client_message << std::endl;

        // 6. Send a response back to the client based on the message
        std::string response;
        if (client_message.find("GET /hello") != std::string::npos) {
            response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, World!\n";
        } else if (client_message.find("GET /bye") != std::string::npos) {
            response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nGoodbye!\n";
        } else {
            response = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\n\nUnknown endpoint\n";
        }

        send(client_socket, response.c_str(), response.length(), 0);
        std::cout << "Response sent to client." << std::endl;

        // 7. Close the client socket (connection to this specific client)
        close(client_socket);
        std::cout << "Client disconnected." << std::endl;
    }

    // The server_fd will typically be closed only when the server application is shut down.
    close(server_fd);
```

## Server Flow Diagram

```mermaid
graph TD
    A[Start Server Application] --> B{Create Socket};
    B -- Success --> C{Bind Socket to IP & Port};
    B -- Failure --> E[Handle Socket Creation Error & Exit];
    C -- Success --> D{Listen for Connections};
    C -- Failure --> F[Handle Bind Error & Exit];
    D -- Success --> G[Server Listening];
    D -- Failure --> H[Handle Listen Error & Exit];
    G --> I{Accept Client Connection (creates new client socket)};
    I -- Success --> J[Client Connected];
    I -- Failure --> K[Handle Accept Error & Exit];
    J --> L[Receive Data from Client (on client socket)];
    L --> M[Send Response to Client (on client socket)];
    M --> N[Close Client Socket];
    N --> O[Close Server Socket];
    O --> P[End Server Application];
```
