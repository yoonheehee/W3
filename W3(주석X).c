#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <winsock2.h>

int main() {
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        perror("Winsock �ʱ�ȭ ����");
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("���� ���� ���� ����");
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        perror("���� ���� ���ε� ����");
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        perror("���� ����");
        return 1;
    }

    printf("HTTP ������ ��Ʈ 8080���� ���� ��\n");

    while (1) {
        struct sockaddr_in client_address;
        int client_address_len = sizeof(client_address);
        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket == INVALID_SOCKET) {
            perror("Ŭ���̾�Ʈ ���� ����");
            break;
        }

        printf("Ŭ���̾�Ʈ ���� ����\n");

        char request[1024];
        int bytesread = recv(client_socket, request, sizeof(request), 0);
        if (bytesread > 0) {
            request[bytesread] = '\0';
            printf("��û���� HTTP:\n%s\n", request);

            const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
            send(client_socket, response, strlen(response), 0);
        }

        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();

    return 0;
}