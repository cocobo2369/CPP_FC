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
**windows �� socket api�� �־ �̰� ���� �ݾ������
WSAStartup()	: WindowsSocketApiStartup
WSAClose()		: WindowsSocketApiClose

socket : socket�� ����
bind : socket�� network interface�� ����
listen : ��� socket�� �����Ұų�
accept : socket �� ���� server - client 

send & receive

close : socket�� �ݴ´�.
*/
int main() {

	//winsock �ʱ�ȭ
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ShowErrorMsg("WSAStartup open");

	SOCKET sSocket, cSocket;
	SOCKADDR_IN sSocketAddr, cSocketAddr;
	
	//���� ���� �ּ� ����
	memset(&sSocketAddr, 0, sizeof(sSocketAddr));
	sSocketAddr.sin_family = AF_INET;
	sSocketAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); /*S_addr �� sin_addr�� member ������ ������ �ٸ� �� �߰� �Ǿ ����ü�� S_un �� ���ԵǾ�����*/
	sSocketAddr.sin_port = htons(SERVERPORT);

	//���� ���� ����
	sSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP = 0
	if (sSocket == INVALID_SOCKET)
		ShowErrorMsg("socket");

	//bind
	if(bind(sSocket, (SOCKADDR*)&sSocketAddr,sizeof(sSocketAddr)) == SOCKET_ERROR)
		ShowErrorMsg("bind");

	//listen
	if (listen(sSocket, 5) == SOCKET_ERROR) //��� �޼��� ť�� ����
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