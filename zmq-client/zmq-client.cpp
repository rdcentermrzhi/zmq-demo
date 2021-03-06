#include "stdafx.h"

int main(int argc, char * argv[])
{
	void * pCtx = NULL;
	void * pSocket = NULL;

	const char * pAddres = "tcp://127.0.0.1:7766";

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
	if (zmq_connect(pSocket, pAddres) < 0)
	{
		zmq_close(pSocket);
		zmq_ctx_destroy(pCtx);
		return 0;
	}

	for (;;)
	{
		static int i = 0;
		char szMsg[1024] = { 0 };
		snprintf(szMsg, sizeof(szMsg), "hello world :%3d", i++);
		printf("Enter to send...\n");

		if (zmq_send(pSocket, szMsg, sizeof(szMsg), 0) < 0)
		{
			fprintf(stderr, "send message faild\n");
			continue;
		}

		printf("send message : [%s] succeed\n", szMsg);
		getchar();

	}


	return 0;

}