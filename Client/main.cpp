#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>
#define MAX 10000
#define SERVERPORT 9876

using namespace std;

void ShowErrorMsg(string message) {
	cout << "This is an Error : " << message << "\n";
	system("pause");
	exit(1);
}

/*
client side
socket
bind
connet
recv & send

*/

int main() {
	
	//winsock 초기화
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ShowErrorMsg("WSAStartup open");

	SOCKET cSocket;
	SOCKADDR_IN sSocketAddr;

	//서버 소켓 주소 설정
	memset(&sSocketAddr, 0, sizeof(sSocketAddr));
	sSocketAddr.sin_family = AF_INET;
	sSocketAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sSocketAddr.sin_port = htons(SERVERPORT);

	//클라이언트 소켓 열기
	cSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (cSocket == INVALID_SOCKET)
		ShowErrorMsg("socket");

	//connect
	if (connect(cSocket, (sockaddr*)&sSocketAddr, sizeof(sSocketAddr)) == SOCKET_ERROR)
		ShowErrorMsg("connect");
	cout << "Network setting is finished" << '\n';

	//send & receive
	string sendMsg;
	char recvMsg[MAX];
	while (1) {
		cout << "Msg :";
		getline(cin, sendMsg);
		cout << "send message to client : " << sendMsg << "\n";
		send(cSocket, sendMsg.c_str(), sendMsg.length(), 0);

		int len = recv(cSocket, recvMsg, sizeof(recvMsg), 0);
		recvMsg[len] = NULL;
		cout << "received from server : " << recvMsg << "\n";
		if (strcmp(recvMsg, "exit") == 0) {
			cout << "server shut down" << "\n";
			break;
		}

	}

	//closesocket
	closesocket(cSocket);

	//WSACleanup
	WSACleanup();
	system("pause");
	return 0;
}