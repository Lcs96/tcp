#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>


#define SERVER_PORT 8888
int main(int argc, char **argv)
{
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	int iRet;
	int iSendLen;
	char cClientBuffer[1000];
	if (argc != 2)
		{
			printf("Usage: %s <server_addr>\n", argv[0]);
			return -1;
		}
	iSocketClient = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == iSocketClient)
		{
			printf("socket error\n");
			return -1;
		}
	tSocketServerAddr.sin_family      = AF_INET;//协议族
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* 指定端口 */
 	//tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY一般为0，让内核去选择ip地址，只有IPv4这样赋值
	if (0 == inet_aton(argv[1], &tSocketServerAddr.sin_addr))
		{
			printf("addr r=error\n");
			return -1;
		}
	memset(tSocketServerAddr.sin_zero, 0, 8);
	iRet = connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (iRet == -1)
		{
			printf("connect error\n");
			return -1;
		}
	while (1)
		{
			if (fgets(cClientBuffer, 999, stdin))
				{
					iSendLen = send(iSocketClient, cClientBuffer, strlen(cClientBuffer), 0);
					if (iSendLen <= 0)
						{
							printf("client over\n");
							close(iSocketClient);
							return -1;
						}
				}
		}
	return 0;
}
















