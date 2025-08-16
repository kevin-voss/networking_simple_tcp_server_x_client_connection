# C++ TCP Client Explanation

This document provides a detailed explanation of the C++ TCP client application (`client/src/client.cpp`), which is designed to initiate a network connection with a TCP server, send a message, and receive a response.

## Client's Role

The client's primary role in a TCP communication is to establish a connection with a known server. It's the active participant that seeks out the server to exchange data. Once connected, it can send requests and receive responses, acting as the initiator of the conversation.

## Key Components and Flow (`client/src/client.cpp`)

The client's code performs the following main steps:

1.  **Address Resolution:** The client needs to know where to connect. Instead of a hardcoded IP address, we use `getaddrinfo` to resolve the server's hostname (e.g., `"tcp-server-container"` in our Docker setup) into an actual network address. This is crucial for Dockerized environments where IP addresses are dynamic.

    ```cpp
    // Key part: Resolving server address by name
    std::string server_name = "tcp-server-container";
    int port = 8080;
    struct addrinfo hints, *res;
    // ... (setting up hints for getaddrinfo)
    if ((status = getaddrinfo(server_name.c_str(), std::to_string(port).c_str(), &hints, &res)) != 0) {
        // Error handling
    }
    // ... (looping through results and connecting)
    ```

2.  **Socket Creation:** A socket is created. This is the endpoint through which the client will communicate.

    ```cpp
    if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
        // Error handling
    }
    ```

3.  **Connection Establishment:** The client attempts to connect to the server using the resolved address and the created socket. This initiates the TCP three-way handshake.

    ```cpp
    if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
        // Error handling
    }
    std::cout << "Connected to server." << std::endl;
    ```

4.  **Sending Data:** Once connected, the client sends its messages to the server. Now, instead of a single message, it sends specific strings for `/hello` and `/bye`.

    ```cpp
    // Send GET /hello request
    std::string hello_message = "GET /hello\n";
    send(sock, hello_message.c_str(), hello_message.length(), 0);
    std::cout << "Message sent: " << hello_message << std::endl;

    // Receive response for /hello
    char buffer_hello[1024] = {0};
    read(sock, buffer_hello, 1024);
    std::cout << "Server response for /hello: " << buffer_hello << std::endl;

    // Send GET /bye request
    std::string bye_message = "GET /bye\n";
    send(sock, bye_message.c_str(), bye_message.length(), 0);
    std::cout << "Message sent: " << bye_message << std::endl;

    // Receive response for /bye
    char buffer_bye[1024] = {0};
    read(sock, buffer_bye, 1024);
    std::cout << "Server response for /bye: " << buffer_bye << std::endl;
    ```

5.  **Receiving Response:** The client then waits for and receives a response from the server.

    ```cpp
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    ```

6.  **Closing Connection:** Finally, the client closes its socket, terminating the connection.

    ```cpp
    close(sock);
    ```

## How the Client Connects to the Server (Docker Context)

In our Dockerized setup, the client connects to the server using its container name (`tcp-server-container`) rather than a fixed IP address like `127.0.0.1` (localhost). This is made possible by Docker's internal DNS resolution within a custom bridge network.

When you run both the client and server containers on the same Docker network (`my-tcp-network`):

1.  The `client` container, when calling `getaddrinfo("tcp-server-container", ...)`, queries Docker's embedded DNS server.
2.  Docker's DNS resolves `tcp-server-container` to the internal IP address assigned to the `tcp-server-container` within `my-tcp-network`.
3.  The client then uses this resolved IP address to establish a standard TCP connection (the three-way handshake) with the server.

This abstraction provided by Docker networking allows services to discover and communicate with each other easily without needing to manage dynamic IP addresses.

## Client Flow Diagram

```mermaid
graph TD
    A[Start Client Application] --> B{Resolve Server Address (using container name)};
    B -- Success --> C{Create Socket};
    C --> D{Attempt Connect to Server};
    D -- Success --> E[Connected];
    D -- Failure --> F[Retry Connection / Handle Error & Exit];
    E --> G[Send Message];
    G --> H[Receive Response];
    H --> I[Print Response];
    I --> J[Close Socket];
    J --> K[End Client Application];
```
