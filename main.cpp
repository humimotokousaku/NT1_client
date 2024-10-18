#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "WSock32.lib")

/// <summary>
/// クライアント側のプログラム
/// </summary>
/// <returns></returns>
int main() {
	WSADATA wsaData;
	// 初期化エラー
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		printf("WinSockの初期化に失敗しました\n");
		return 1;
	}

	u_short uport = 8000;
	printf("ポート番号を入力してください:");
	scanf_s("%hd", &uport);


	char szServer[1024];
	printf("IPアドレスを入力してください:");
	scanf_s("%s", szServer, 1024);
	fflush(stdin);

	// ソケット作成
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		printf("ソケットの作成に失敗しました\n");
		return 1;
	}

	HOSTENT* ipHost;
	ipHost = gethostbyname(szServer);

	SOCKADDR_IN saddr;
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = ipHost->h_addrtype;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = *((u_long*)ipHost->h_addr);

	if (connect(sock, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("サーバーと接続できませんでした\n");
		closesocket(sock);
		return 0;
	}

	printf("\nチャット開始\n");
	printf("サーバーにメッセージを送ってください\n");

	while (1) {
		int nRcv;
		char szBuf[1024];

		printf("送信--> ");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(sock, szBuf, (int)strlen(szBuf), 0);

		nRcv = recv(sock, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\0';
		printf("受信--> %s\n", szBuf);
	}
	closesocket(sock);
	WSACleanup();

	return 0;
}