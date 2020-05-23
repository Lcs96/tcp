#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>


#define BACKLOG 10
#define SERVER_PORT 8888

int main(int argc, char **argv)
{
	int iSocketServer;
	int iSocketClient;
	int iClientNum = -1;
	int iRet;
	int len;
	int iRecvLen;
	struct sockaddr_in tSocketServerAddr;
	struct sockaddr_in tSocketClientAddr;
	char cRecvBuffer[1000];
	signal(SIGCHLD,SIG_IGN);
	iSocketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == iSocketServer)
	{
		printf("socket error!\n");
		return -1;
	}
	tSocketServerAddr.sin_family      = AF_INET;//协议族
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* 指定端口 */
 	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY一般为0，让内核去选择ip地址，只有IPv4这样赋值
	memset(tSocketServerAddr.sin_zero, 0, 8);
	iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}
	iRet = listen(iSocketServer, BACKLOG);
	if (-1 == iRet)
	{
		printf("listen error!\n");
		return -1;
	}
	while(1)
		{
			len = sizeof(struct sockaddr);
			iSocketClient = accept(iSocketServer, (struct sockaddr *)&tSocketClientAddr, &len);
			if (iSocketClient != -1)
				{
					iClientNum++;
					printf("this is the client%d, addr is %s\n", iClientNum, inet_ntoa(tSocketClientAddr.sin_addr));
					if (!fork())
						{
							close(iSocketServer);
							while(1)
								{
									iRecvLen = recv(iSocketClient, cRecvBuffer, 999, 0);
									if (iRecvLen <= 0)
										{
											printf("can't recv\n");
											close(iSocketClient);
											return -1;
										}
									else
										{
											cRecvBuffer[iRecvLen] = '\0';
											printf("%s\n",cRecvBuffer);
										}
								}
						}
					close(iSocketClient);
				}
		}
	close(iSocketServer);
	return 0;
}
