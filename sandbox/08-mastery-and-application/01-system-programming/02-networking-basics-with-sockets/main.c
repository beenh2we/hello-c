#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

// Flag for graceful termination
volatile sig_atomic_t keep_running = 1;

// Signal handler
void handle_signal(int sig)
{
    keep_running = 0;
}

// Function to make a socket non-blocking
int make_nonblocking(int sockfd)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl F_GETFL");
        return -1;
    }

    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl F_SETFL O_NONBLOCK");
        return -1;
    }

    return 0;
}

// TCP Server implementation
void tcp_server(int port)
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket creation failed");
        return;
    }

    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        close(server_fd);
        return;
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket to specified port
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        return;
    }

    // Start listening for connections
    if (listen(server_fd, 5) < 0)
    {
        perror("listen failed");
        close(server_fd);
        return;
    }

    printf("TCP Server listening on port %d...\n", port);

    // Set up signal handler for graceful termination
    signal(SIGINT, handle_signal);

    // Make server socket non-blocking
    make_nonblocking(server_fd);

    // Set up file descriptor set for select()
    fd_set read_fds, master_fds;
    int max_fd = server_fd;

    FD_ZERO(&master_fds);
    FD_SET(server_fd, &master_fds);

    // Server main loop
    while (keep_running)
    {
        // Copy master set to read_fds for select()
        read_fds = master_fds;

        // Set timeout for select (100ms)
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

        // Wait for activity on any of the sockets
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

        if (activity < 0 && errno != EINTR)
        {
            perror("select");
            break;
        }

        // Check for new connections on server socket
        if (FD_ISSET(server_fd, &read_fds))
        {
            client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_len);
            if (client_fd < 0)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    perror("accept");
                }
            }
            else
            {
                printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                // Add new client to master set
                FD_SET(client_fd, &master_fds);
                if (client_fd > max_fd)
                {
                    max_fd = client_fd;
                }

                // Send welcome message
                const char *welcome = "Welcome to the TCP server!\n";
                send(client_fd, welcome, strlen(welcome), 0);
            }
        }

        // Check existing connections for data
        for (int i = 0; i <= max_fd; i++)
        {
            if (i != server_fd && FD_ISSET(i, &read_fds))
            {
                memset(buffer, 0, sizeof(buffer));
                int bytes_read = recv(i, buffer, sizeof(buffer) - 1, 0);

                if (bytes_read <= 0)
                {
                    // Connection closed or error
                    if (bytes_read == 0)
                    {
                        printf("Client disconnected\n");
                    }
                    else
                    {
                        perror("recv");
                    }

                    close(i);
                    FD_CLR(i, &master_fds);
                }
                else
                {
                    // Process received data
                    buffer[bytes_read] = '\0';
                    printf("Received from client %d: %s", i, buffer);

                    // Echo back the message
                    char response[1100];
                    snprintf(response, sizeof(response), "Server echo: %s", buffer);
                    send(i, response, strlen(response), 0);
                }
            }
        }
    }

    // Clean up - close all sockets
    for (int i = 0; i <= max_fd; i++)
    {
        if (FD_ISSET(i, &master_fds))
        {
            close(i);
        }
    }

    printf("\nTCP Server shut down\n");
}

// TCP Client implementation
void tcp_client(const char *server_ip, int port)
{
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("socket creation failed");
        return;
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Convert IP address from text to binary
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("invalid address");
        close(sock_fd);
        return;
    }

    // Connect to server
    if (connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect failed");
        close(sock_fd);
        return;
    }

    printf("Connected to server at %s:%d\n", server_ip, port);

    // Set up signal handler
    signal(SIGINT, handle_signal);

    // Receive initial welcome message
    memset(buffer, 0, sizeof(buffer));
    int bytes_read = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read > 0)
    {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }

    // Main communication loop
    while (keep_running)
    {
        printf("Enter message (or 'exit' to quit): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Check for exit
        if (strncmp(buffer, "exit", 4) == 0)
        {
            break;
        }

        // Send message to server
        ssize_t bytes_sent = send(sock_fd, buffer, strlen(buffer), 0);
        if (bytes_sent < 0)
        {
            perror("send failed");
            break;
        }

        // Receive response
        memset(buffer, 0, sizeof(buffer));
        bytes_read = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0)
        {
            if (bytes_read == 0)
            {
                printf("Server closed the connection\n");
            }
            else
            {
                perror("recv failed");
            }
            break;
        }

        buffer[bytes_read] = '\0';
        printf("Server response: %s", buffer);
    }

    // Clean up
    close(sock_fd);
    printf("\nDisconnected from server\n");
}

// UDP Server implementation
void udp_server(int port)
{
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    // Create UDP socket
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0)
    {
        perror("socket creation failed");
        return;
    }

    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        close(server_fd);
        return;
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket to specified port
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        return;
    }

    printf("UDP Server listening on port %d...\n", port);

    // Set up signal handler
    signal(SIGINT, handle_signal);

    // Server loop
    while (keep_running)
    {
        // Receive datagram
        memset(buffer, 0, sizeof(buffer));
        int bytes_read =
            recvfrom(server_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *) &client_addr, &client_len);

        if (bytes_read < 0)
        {
            perror("recvfrom");
            continue;
        }

        buffer[bytes_read] = '\0';
        printf("Received from %s:%d: %s", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

        // Send response
        char response[1100];
        snprintf(response, sizeof(response), "UDP Server echo: %s", buffer);
        sendto(server_fd, response, strlen(response), 0, (struct sockaddr *) &client_addr, client_len);
    }

    // Clean up
    close(server_fd);
    printf("\nUDP Server shut down\n");
}

// UDP Client implementation
void udp_client(const char *server_ip, int port)
{
    int sock_fd;
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    char buffer[1024];

    // Create UDP socket
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        perror("socket creation failed");
        return;
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Convert IP address from text to binary
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("invalid address");
        close(sock_fd);
        return;
    }

    printf("UDP Client ready to send to %s:%d\n", server_ip, port);

    // Set up signal handler
    signal(SIGINT, handle_signal);

    // Main communication loop
    while (keep_running)
    {
        printf("Enter message (or 'exit' to quit): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Check for exit
        if (strncmp(buffer, "exit", 4) == 0)
        {
            break;
        }

        // Send datagram to server
        ssize_t bytes_sent = sendto(sock_fd, buffer, strlen(buffer), 0, (struct sockaddr *) &server_addr, server_len);
        if (bytes_sent < 0)
        {
            perror("sendto failed");
            continue;
        }

        // Receive response with timeout
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(sock_fd, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int select_result = select(sock_fd + 1, &read_fds, NULL, NULL, &timeout);

        if (select_result < 0)
        {
            perror("select");
            continue;
        }
        else if (select_result == 0)
        {
            printf("Timeout: No response from server\n");
            continue;
        }

        // Receive response
        memset(buffer, 0, sizeof(buffer));
        int bytes_read =
            recvfrom(sock_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *) &server_addr, &server_len);

        if (bytes_read < 0)
        {
            perror("recvfrom");
            continue;
        }

        buffer[bytes_read] = '\0';
        printf("Server response: %s", buffer);
    }

    // Clean up
    close(sock_fd);
    printf("\nUDP Client shut down\n");
}

// Function to resolve a hostname to IP address
void resolve_hostname(const char *hostname)
{
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // AF_INET or AF_INET6 for IPv4/IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return;
    }

    printf("\nIP addresses for %s:\n\n", hostname);

    for (p = res; p != NULL; p = p->ai_next)
    {
        void *addr;
        const char *ipver;

        // Get pointer to address
        if (p->ai_family == AF_INET)
        {  // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else if (p->ai_family == AF_INET6)
        {  // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }
        else
        {
            continue;
        }

        // Convert IP to string
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("  %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res);
}

int main(int argc, char *argv[])
{
    printf("=== Socket Programming Demonstration ===\n");

    // Check if we have enough arguments
    if (argc < 2)
    {
        printf("Usage: %s [option]\n", argv[0]);
        printf("Options:\n");
        printf("  tcpserver <port>       - Run a TCP server on specified port\n");
        printf("  tcpclient <ip> <port>  - Run a TCP client connecting to specified IP and port\n");
        printf("  udpserver <port>       - Run a UDP server on specified port\n");
        printf("  udpclient <ip> <port>  - Run a UDP client connecting to specified IP and port\n");
        printf("  resolve <hostname>     - Resolve hostname to IP addresses\n");
        return 1;
    }

    // Parse arguments and run appropriate function
    if (strcmp(argv[1], "tcpserver") == 0)
    {
        if (argc < 3)
        {
            printf("Error: Port number required for TCP server\n");
            return 1;
        }
        tcp_server(atoi(argv[2]));
    }
    else if (strcmp(argv[1], "tcpclient") == 0)
    {
        if (argc < 4)
        {
            printf("Error: IP address and port required for TCP client\n");
            return 1;
        }
        tcp_client(argv[2], atoi(argv[3]));
    }
    else if (strcmp(argv[1], "udpserver") == 0)
    {
        if (argc < 3)
        {
            printf("Error: Port number required for UDP server\n");
            return 1;
        }
        udp_server(atoi(argv[2]));
    }
    else if (strcmp(argv[1], "udpclient") == 0)
    {
        if (argc < 4)
        {
            printf("Error: IP address and port required for UDP client\n");
            return 1;
        }
        udp_client(argv[2], atoi(argv[3]));
    }
    else if (strcmp(argv[1], "resolve") == 0)
    {
        if (argc < 3)
        {
            printf("Error: Hostname required for resolution\n");
            return 1;
        }
        resolve_hostname(argv[2]);
    }
    else
    {
        printf("Unknown option: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
