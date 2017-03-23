//#include <gtkmm.h>
//#include <glib.h>
#include "main.h"
#include "hmi.h"
#include "leapmotion.h"
#include "s7server.h"
//#include "main.h"

volatile int retStatus = 0;

S7Server *server;
Hmi *hmi;

int main(int argc, char **argv)
{
	//Snap7 server
	server = new S7Server;
	setAngles(zeichen_grund);
	server->StartTo("192.168.0.100");

	//LeapMotion
	HandListener listener;
	Leap::Controller controller;
	controller.addListener(listener);

	//Gtk application
	hmi = new Hmi;
	hmi->run();

	controller.removeListener(listener);

	server->Stop();
	delete server;
	return retStatus;
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
