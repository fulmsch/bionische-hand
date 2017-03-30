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
		virtual void onFrame(const Leap::Controller&);
	private:
		void winkelBerechnen(const Leap::Hand&, unsigned char *winkel);
};

#endif
