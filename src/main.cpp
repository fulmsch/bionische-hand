//#include <gtkmm.h>
//#include <glib.h>
#include "hmi.h"
#include "leapmotion.h"
#include "s7server.h"
//#include "main.h"

S7Server *server;

int main(int argc, char **argv)
{
	//Snap7 server
	server = new S7Server;
	setAngles(zeichen_grund);
	server->Start();

	//LeapMotion
	HandListener listener;
	Leap::Controller controller;
	controller.addListener(listener);

	//Gtk application
	Hmi hmi;
	hmi.run();

	controller.removeListener(listener);

	server->Stop();
	delete server;
	return 0;
}

void setAngles(unsigned char *source) {
	server->LockArea(srvAreaDB, 1);
	memcpy(server->DB_Send.s.winkel, source, 14);
	server->UnlockArea(srvAreaDB, 1);
}

unsigned char zeichen_grund[14] = {0, 0,
                                   0, 0, 0,
                                   0, 0, 0,
                                   0, 0, 0,
                                   0, 0, 0};

unsigned char zeichen_faust[14] = {90, 90,
                                   90, 90, 90,
                                   90, 90, 90,
                                   90, 90, 90,
                                   90, 90, 90};

unsigned char zeichen_peace[14] = {90, 90,
                                   0, 0, 0,
                                   0, 0, 0,
                                   90, 90, 90,
                                   90, 90, 90};

unsigned char zeichen_daumenhoch[14] = {0, 0,
                                        90, 90, 90,
                                        90, 90, 90,
                                        90, 90, 90,
                                        90, 90, 90};