#include <iostream>
#include <string>
#include <WinSock2.h>
#define MAX 10000
#define SERVERPORT 9876
using namespace std;

void ShowErrorMsg(string message) {
	cout << "This is an Error : " << message << '\n';
	system("pause");
	exit(1);
}

/*
server side
**windows 는 socket api가 있어서 이걸 열고 닫아줘야함
WSAStartup()	: WindowsSocketApiStartup
WSAClose()		: WindowsSocketApiClose

socket : socket을 연다
bind : socket을 network interface로 묶음
listen : 몇개의 socket을 연결할거냐
accept : socket 간 연결 server - client 

send & receive

close : socket을 닫는다.
*/
int main() {

	//winsock 초기화
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ShowErrorMsg("WSAStartup open");

	SOCKET sSocket, cSocket;
	SOCKADDR_IN sSocketAddr, cSocketAddr;
	
	//서버 소켓 주소 설정
	memset(&sSocketAddr, 0, sizeof(sSocketAddr));
	sSocketAddr.sin_family = AF_INET;
	sSocketAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); /*S_addr 는 sin_addr의 member 였으나 형식이 다른 게 추가 되어서 공용체로 S_un 에 포함되었나봄*/
	sSocketAddr.sin_port = htons(SERVERPORT);

	//서버 소켓 열기
	sSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP = 0
	if (sSocket == INVALID_SOCKET)
		ShowErrorMsg("socket");

	//bind
	if(bind(sSocket, (SOCKADDR*)&sSocketAddr,sizeof(sSocketAddr)) == SOCKET_ERROR)
		ShowErrorMsg("bind");

	//listen
	if (listen(sSocket, 5) == SOCKET_ERROR) //대기 메세지 큐의 개수
		ShowErrorMsg("listen");

	cout << "Network setting is finished" << '\n';
	//accept
	int sizeofcSocketAddr = sizeof(cSocketAddr);
	cSocket = accept(sSocket, (SOCKADDR*)&cSocketAddr, &sizeofcSocketAddr);
	if (cSocket == INVALID_SOCKET)
		ShowErrorMsg("accept");

	//send
	char recvMsg[MAX];
	while (1) {
		int len = recv(cSocket, recvMsg, MAX, 0);
		recvMsg[len] = NULL;
		cout << "received message from client : " << recvMsg << "\n";
		cout << "This is echo server, so we just send same message" << "\n";
		cout << "send to client : " << recvMsg << "\n";
		send(cSocket, recvMsg, len, 0);
		if (strcmp(recvMsg, "exit") == 0) {
			cout << "server shut down" << "\n";
			break;
		}
	}

	//closesocjet
	closesocket(sSocket);
	closesocket(cSocket);

	//WSACleanup
	WSACleanup();
	system("pause");
	return 0;

}