#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "WSock32.lib")

/// <summary>
/// �N���C�A���g���̃v���O����
/// </summary>
/// <returns></returns>
int main() {
	WSADATA wsaData;
	// �������G���[
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		printf("WinSock�̏������Ɏ��s���܂���\n");
		return 1;
	}

	u_short uport = 8000;
	printf("�|�[�g�ԍ�����͂��Ă�������:");
	scanf_s("%hd", &uport);


	char szServer[1024];
	printf("IP�A�h���X����͂��Ă�������:");
	scanf_s("%s", szServer, 1024);
	fflush(stdin);

	// �\�P�b�g�쐬
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		printf("�\�P�b�g�̍쐬�Ɏ��s���܂���\n");
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
		printf("�T�[�o�[�Ɛڑ��ł��܂���ł���\n");
		closesocket(sock);
		return 0;
	}

	printf("\n�`���b�g�J�n\n");
	printf("�T�[�o�[�Ƀ��b�Z�[�W�𑗂��Ă�������\n");

	while (1) {
		int nRcv;
		char szBuf[1024];

		printf("���M--> ");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(sock, szBuf, (int)strlen(szBuf), 0);

		nRcv = recv(sock, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\0';
		printf("��M--> %s\n", szBuf);
	}
	closesocket(sock);
	WSACleanup();

	return 0;
}