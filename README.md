# C++ TCP Client-Server with Docker

This project demonstrates a basic TCP client-server communication implemented in C++ and containerized using Docker. It provides a foundational understanding of network programming with TCP and how to deploy interconnected applications in a Dockerized environment.

## Table of Contents

*   [Networking Fundamentals](#networking-fundamentals)
    *   [The OSI Model (Simplified)](#the-osi-model-simplified)
    *   [Why TCP?](#why-tcp)
    *   [How TCP Works (Simplified Handshake)](#how-tcp-works-simplified-handshake)
*   [Project Structure](#project-structure)
*   [Prerequisites](#prerequisites)
*   [Building the Docker Images](#building-the-docker-images)
*   [Running with Docker Compose](#running-with-docker-compose)
*   [Running the Applications (Manual Docker Commands)](#running-the-applications-manual-docker-commands)
*   [How it Works (Docker Networking)](#how-it-works-docker-networking)
*   [Cleanup Docker Resources](#cleanup-docker-resources)
*   [Troubleshooting](#troubleshooting)

## Networking Fundamentals

Computer networking is the backbone of modern computing, enabling diverse devices to communicate and share resources. To grasp how communication unfolds, we often refer to the OSI (Open Systems Interconnection) model, a conceptual framework that standardizes communication functions of a telecommunication or computing system into seven distinct layers.

### The OSI Model (Simplified)

For the scope of this project, we'll focus on the layers most relevant to our TCP communication:

*   **Application Layer:** This is the closest layer to the end user. It facilitates communication for end-user processes and applications. Examples include HTTP for web browsing, FTP for file transfer, and SMTP for email. In our C++ applications, the `read` and `send` operations are where our application interacts with the network stack at this conceptual level.

*   **Transport Layer:** This layer provides end-to-end communication services for applications. It handles the segmentation of data from the application layer and reassembles it at the receiving end. Key protocols here are TCP (Transmission Control Protocol) and UDP (User Datagram Protocol). TCP, which we are using, ensures reliable, ordered, and error-checked delivery of a stream of bytes.

*   **Network Layer:** Responsible for logical addressing (IP addresses) and routing data packets across different networks. The Internet Protocol (IP) operates at this layer, determining the best path for data to travel from source to destination.

*   **Data Link Layer:** Deals with physical addressing (MAC addresses) and provides error-free transfer of data frames from one node to another over the physical layer. This is where local network communication occurs.

*   **Physical Layer:** The lowest layer, concerned with the physical transmission of raw bit streams over a physical medium (e.g., Ethernet cables, Wi-Fi signals). It defines the electrical and mechanical specifications of the data connection.

### Why TCP?

TCP is chosen for this project because it provides reliable, ordered, and error-checked delivery of a stream of bytes. This reliability is crucial for applications where data integrity is paramount (e.g., file transfers, web browsing, email). Here's a deeper dive into its advantages:

1.  **Connection-Oriented:** Before any data exchange, TCP establishes a dedicated connection between the sender and receiver through a process called the "three-way handshake." This ensures both parties are ready and synchronized for communication, guaranteeing a stable channel.

2.  **Reliable Data Transfer:** TCP ensures that all data arrives at its destination uncorrupted, in the correct order, and without any loss. It achieves this through a combination of mechanisms:
    *   **Sequencing:** Each data segment transmitted is assigned a sequence number. This allows the receiver to reassemble segments in the correct order, even if they arrive out of sequence, and to detect any missing segments.
    *   **Acknowledgments (ACKs):** Upon receiving data, the receiver sends an acknowledgment back to the sender. If an ACK is not received within a specified timeout, the sender assumes the data was lost and retransmits it.
    *   **Error Detection:** TCP uses a checksum to detect any corruption of data that may occur during transmission. If an error is detected, the segment is discarded, and a retransmission is requested.
    *   **Flow Control:** This mechanism prevents a fast sender from overwhelming a slower receiver. The receiver advertises a "receive window" indicating how much data it can currently buffer. The sender adjusts its transmission rate to stay within this window.
    *   **Congestion Control:** TCP dynamically adjusts the data transmission rate to avoid overwhelming the network itself. It uses algorithms to detect network congestion and reduce the sending rate, preventing bottlenecks and improving overall network efficiency.

3.  **Byte Stream Delivery:** TCP treats data as a continuous stream of bytes, abstracting away the underlying packet structure. This simplifies application development, as developers can focus on sending and receiving data as a continuous flow rather than managing individual packets.

### How TCP Works (Simplified Handshake)

The establishment of a TCP connection begins with a three-way handshake:

1.  **SYN (Synchronize):** The client initiates the connection by sending a TCP segment with the SYN flag set (SYN=1) to the server. This segment includes a randomly generated initial sequence number (ISN) from the client.

2.  **SYN-ACK (Synchronize-Acknowledgment):** Upon receiving the client's SYN, the server responds with a SYN-ACK segment. This segment has both the SYN and ACK flags set (SYN=1, ACK=1). The ACK acknowledges the client's ISN (by sending ISN + 1), and the server also sends its own ISN. 

3.  **ACK (Acknowledgment):** Finally, the client sends an ACK segment to the server. This segment acknowledges the server's ISN (by sending server's ISN + 1). Once this ACK is received by the server, the three-way handshake is complete, and a full-duplex connection is established. Both the client and server can now send and receive data.

## Project Structure

The project is organized into a clear and intuitive directory structure:

```
networking/
├── server/
│   ├── src/
│   │   └── server.cpp        # C++ source code for the TCP server
│   └── Dockerfile            # Dockerfile to build the server image
├── client/
│   ├── src/
│   │   └── client.cpp        # C++ source code for the TCP client
│   └── Dockerfile            # Dockerfile to build the client image
├── docker-compose.yml       # Defines and runs the multi-container Docker application
├── Makefile                 # Defines commands for building, running, and managing Docker resources
└── README.md                 # This documentation file
```

*   `networking/`: The root directory of the project.
*   `docker-compose.yml`: Defines and runs the multi-container Docker application.
*   `Makefile`: Defines commands for building, running, and managing Docker resources.
*   `server/`: Contains all files related to the TCP server application.
*   `client/`: Contains all files related to the TCP client application.

## Prerequisites

Before you begin, ensure you have the following installed on your system:

*   **Docker:** Used to build and run the containerized C++ applications. You can download Docker Desktop from [https://www.docker.com/products/docker-desktop](https://www.docker.com/products/docker-desktop).
*   **Make (Optional for Windows):** The `Makefile` provides convenient commands for managing Docker services. If you are on Windows and encounter an error like "make : The term 'make' is not recognized...", you may need to install it. It's commonly included with [Git for Windows](https://git-scm.com/download/win) (select "Use Git from the Windows Command Prompt" during installation) or can be installed via package managers like [Chocolatey](https://chocolatey.org/packages/make).
    Alternatively, you can directly use the `docker compose` commands shown in the "Building the Docker Images", "Running with Docker Compose", and "Cleanup Docker Resources" sections.

## Building the Docker Images

To build the Docker images for both the server and client, navigate to the root of the `networking` directory in your terminal and run:

```bash
make build
```
This command uses `docker compose build` internally to build both `server` and `client` images.

## Running with Docker Compose

Docker Compose allows you to define and run multi-container Docker applications. With a single command, you can bring up the entire environment. We've provided convenient Makefile targets to simplify these operations.

1.  **Start the Services:**

    Navigate to the root of the `networking` directory in your terminal (where `Makefile` and `docker-compose.yml` are located) and run:

    ```bash
    make up
    ```
    This command uses `docker compose up -d` internally to build (if not already built), create, start, and attach to containers for all services defined in `docker-compose.yml` in detached mode.

2.  **View Logs:**

    To view the logs from all running services:

    ```bash
    make logs
    ```
    This command internally runs `docker compose logs`.

## Running the Applications (Manual Docker Commands)

This section details how to run the applications using individual `docker run` commands for a deeper understanding of Docker networking. For a simpler approach, refer to the [Running with Docker Compose](#running-with-docker-compose) section.

1.  **Create a Docker Network:**

    First, create a custom bridge network. This allows containers connected to it to communicate with each other using their container names as hostnames.

    ```bash
    docker network create my-tcp-network
    ```

2.  **Run the Server Container:**

    Run the server container, attaching it to the `my-tcp-network` and naming it `tcp-server-container`. We will remove the port publishing since the inter-container communication is done via the network.

    ```bash
    docker run -d --network my-tcp-network --name tcp-server-container networking-server
    ```
    *   `-d`: Runs the container in detached mode (in the background).
    *   `--network my-tcp-network`: Connects the container to the custom network.
    *   `--name tcp-server-container`: Assigns a readable name to the container.

3.  **Run the Client Container:**

    Now, run the client container. The client will connect to `tcp-server-container` (the name of our server container) to send multiple requests. **Note:** The client currently establishes a new connection for each request.

    ```bash
    docker run --network my-tcp-network --name tcp-client-container --rm networking-client
    ```
    *   `--rm`: Automatically removes the container once it exits.

    You should see the client's output directly in your terminal, showing it connected to the server, sending `GET /hello`, receiving a response, then reconnecting, sending `GET /bye`, and receiving another response.
    To observe the server's behavior and confirm client connections (including their IP addresses and ports), view the server logs:
    ```bash
    docker logs tcp-server-container
    ```

## How it Works (Docker Networking)

When you create a custom bridge network (e.g., `my-tcp-network`), Docker provides a simple DNS service for containers connected to that network. This means that instead of using IP addresses, containers can resolve each other by their assigned names.

In our setup:

1.  The `tcp-server-container` is running and listening on port 8080.
2.  The `tcp-client-container` attempts to connect to a hostname `tcp-server-container` on port 8080.
3.  Docker's internal DNS resolves `tcp-server-container` to the correct internal IP address of the server container within the `my-tcp-network`.
4.  The TCP connection is established, and the client and server communicate as designed.

This approach simplifies communication between services within Docker, as you don't need to hardcode IP addresses, which can change.

## Cleanup Docker Resources

To stop and remove all services, containers, and networks created by Docker Compose, navigate to the root of the `networking` directory and run:

```bash
make down
```
This command internally runs `docker compose down`.

To stop and remove all Docker resources created by the project, including images and volumes, run:

```bash
make clean
```
This command internally runs `docker compose down --rmi all --volumes --remove-orphans`.

For manual cleanup of resources created without Docker Compose, use the following commands:

```bash
docker stop tcp-server-container tcp-client-container
docker rm tcp-server-container tcp-client-container
docker network rm my-tcp-network
```

## Troubleshooting

*   **"Address already in use" error when running server:**
    This usually means the port (8080) is already being used by another process on your host machine. Ensure no other applications are using this port, or change the port in `server.cpp` and `Dockerfile`.

*   **"Connection refused" error from client:**
    *   Ensure the server container is running: `docker ps`.
    *   Verify the server container's name (`tcp-server-container`) and port (8080) are correct in the client's code and in the `docker run` command.
    *   Check if both containers are on the same Docker network: `docker inspect my-tcp-network`.

*   **Server or client immediately exits:**
    *   Check the logs: `docker logs tcp-server-container` or `docker logs tcp-client-container` to see if there are any C++ runtime errors.
    *   Ensure the C++ code compiles successfully within the Dockerfile (`RUN g++ ...`).
