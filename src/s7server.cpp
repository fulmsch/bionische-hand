#include "s7server.h"

void S7API EventCallBack(void *usrPtr, PSrvEvent PEvent, int Size) {
	printf("%s\n", SrvEventText(PEvent).c_str());
}

S7Server::S7Server() {
	RegisterArea(srvAreaDB, 1, &DB_Send.a, sizeof(DB_Send.a));
	RegisterArea(srvAreaDB, 2, &DB_Recv.a, sizeof(DB_Recv.a));
	SetEventsCallback(EventCallBack, NULL);
}
