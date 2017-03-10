#ifndef LEAPMOTION_H
#define LEAPMOTION_H

#include "Leap.h"

typedef enum {
	MISSING,
	CONNECTED,
	NOHAND,
	TRACKING,
	TOOMANYHANDS
} t_leap_status;

class HandListener : public Leap::Listener {
	public:
		virtual void onInit(const Leap::Controller&);
		virtual void onConnect(const Leap::Controller&);
//		virtual void onDisconnect(const Controller&);
//		virtual void onExit(const Controller&);
		virtual void onFrame(const Leap::Controller&);
//		virtual void onFocusGained(const Controller&);
//		virtual void onFocusLost(const Controller&);
//		virtual void onDeviceChange(const Controller&);
//		virtual void onServiceConnect(const Controller&);
//		virtual void onServiceDisconnect(const Controller&);
//		virtual void onServiceChange(const Controller&);
//		virtual void onDeviceFailure(const Controller&);
//		virtual void onDiagnosticEvent(const Controller&, const char*);
	private:
		void winkelBerechnen(const Leap::Hand&, unsigned char *winkel);
};

#endif
