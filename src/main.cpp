#include "main.h"
#include "hmi.h"
#include "leapmotion.h"
#include "s7server.h"

volatile int retStatus = 0;

S7Server *server;
Hmi *hmi;

int main(int argc, char **argv)
{
	//Snap7 server starten
	server = new S7Server;
	setWinkel(zeichen_grund);
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

void setWinkel(unsigned char *source) {
	//Winkel im DB setzen
	server->LockArea(srvAreaDB, 1);
	memcpy(server->DB_Send.s.winkel, source, 14);
	server->UnlockArea(srvAreaDB, 1);
}

//Vorgespeicherte Handzeichen
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

unsigned char zeichen_mittelfinger[14] = {90, 90,
                                          90, 90, 90,
                                          0, 0, 0,
                                          90, 90, 90,
                                          90, 90, 90};

unsigned char zeichen_metal[14] = {90, 90,
                                   0, 0, 0,
                                   90, 90, 90,
                                   90, 90, 90,
                                   0, 0, 0};

//Eigene Handzeichen
unsigned char zeichen1[14] = {0};
unsigned char zeichen2[14] = {0};
unsigned char zeichen3[14] = {0};
unsigned char zeichen4[14] = {0};
unsigned char zeichen5[14] = {0};
