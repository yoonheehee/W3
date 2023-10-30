#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <winsock2.h>

int main() {
    //Winsock 초기화
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        perror("Winsock 초기화 실패");
        return 1;
    }

    //서버 소켓 생성
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("서버 소켓 생성 성공");
        return 1;
    }

    //서버 소켓을 특정 IP 주소 및 포트에 바인딩
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        perror("서버 소켓 바인딩 실패");
        return 1;
    }

    //서버 소켓 연결
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        perror("연결 실패");
        return 1;
    }

    printf("HTTP 서버가 포트 8080에서 수신 중\n");

    while (1) {
        //클라이언트 연결 수락
        struct sockaddr_in client_address;
        int client_address_len = sizeof(client_address);
        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket == INVALID_SOCKET) {
            perror("클라이언트 연결 거절");
            break;
        }

        printf("클라이언트 연결 수락\n");

        //HTTP 요청 읽기 및 응답
        char request[1024];
        int bytesread = recv(client_socket, request, sizeof(request), 0);
        if (bytesread > 0) {
            request[bytesread] = '\0';
            printf("요청받은 HTTP:\n%s\n", request);

            const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
            send(client_socket, response, strlen(response), 0);
        }

        //클라이언트 소켓 종료
        closesocket(client_socket);
    }

    //서버 소켓 종료
    closesocket(server_socket);
    WSACleanup();

    return 0;
}