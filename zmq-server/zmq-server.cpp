// zmq-server.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int main(int argc, char * argv[])
{
	void * pCtx = NULL;
	void * pSocket = NULL;

	const char * pAddres = "tcp://*:7766";

	//创建context, zmq socket 需要在context 上创建
	if ((pCtx = zmq_ctx_new()) == NULL)
	{
		return 0;
	}

	//创建socket 
	if ((pSocket = zmq_socket(pCtx, ZMQ_DEALER)) == NULL)
	{
		zmq_ctx_destroy(pCtx);
		return 0;
	}

	//设置超时时间
	int iRcvTimeout = 5000;

	if ((zmq_setsockopt(pSocket, ZMQ_RCVTIMEO, &iRcvTimeout, sizeof(iRcvTimeout))) < 0)
	{
		zmq_close(pSocket);
		zmq_ctx_destroy(pCtx);
		return 0;
	}
	//绑定地址 tcp://*:7766
	if (zmq_bind(pSocket, pAddres) < 0)
	{
		zmq_close(pSocket);
		zmq_ctx_destroy(pCtx);
		return 0;
	}

	printf("bind at :%s\n", pAddres);

	for (;;)
	{
		char szMsg[1024] = { 0 };
		printf("waitting ....\n");

		errno = 0;

		if (zmq_recv(pSocket, szMsg, sizeof(szMsg), 0) < 0)
		{
			printf("error = %s\n", zmq_strerror(errno));
			continue;
		}

		printf("received message :%s \n", szMsg);
	}


	return 0;
}

