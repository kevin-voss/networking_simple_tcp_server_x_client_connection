# C++ TCP Client Explanation

This document provides a detailed explanation of the C++ TCP client application (`client/src/client.cpp`), which is designed to initiate a network connection with a TCP server, send a message, and receive a response.

## Client's Role

The client's primary role in a TCP communication is to establish a connection with a known server. It's the active participant that seeks out the server to exchange data. Once connected, it can send requests and receive responses, acting as the initiator of the conversation.

## Key Components and Flow (`client/src/client.cpp`)

The client's code has been refactored to use a `fetch` function, which simplifies sending multiple HTTP GET requests to the server. The `fetch` function encapsulates the logic for establishing a connection, sending a request, and receiving a response.

Here's how the `fetch` function works:

```cpp
std::string fetch(const std::string& path, const std::string& server_address, int port) {
    // 1. Address Resolution: Resolves the server's hostname.
    // 2. Socket Creation: Creates a new socket.
    // 3. Connection Establishment: Connects to the server.
    // 4. Sending Data: Sends the GET request for the specified path.
    // 5. Receiving Response: Reads the server's response.
    // 6. Closing Connection: Closes the socket.
    // Returns the server's response string.
}
```

And here's how it's used in the `main` function to make requests to various endpoints:

```cpp
int main() {
    int port = 8080;
    std::string server_address = "server"; 

    fetch("/hello", server_address, port);
    fetch("/bye", server_address, port);
    fetch("/health", server_address, port);
    fetch("/metrics", server_address, port);

    return 0;
}
```

## How the Client Connects to the Server (Docker Context)

In our Dockerized setup, the client connects to the server using its service name (`server`) rather than a fixed IP address like `127.0.0.1` (localhost). This is made possible by Docker's internal DNS resolution within a custom bridge network.

When you run both the client and server containers on the same Docker network (`app_network`):

1.  The `client` container, when calling `getaddrinfo("server", ...)`, queries Docker's embedded DNS server.
2.  Docker's DNS resolves `server` to the internal IP address assigned to the `server` container within `app_network`.
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
