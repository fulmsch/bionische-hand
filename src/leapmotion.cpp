#include "leapmotion.h"

#include <iostream>
#include "main.h"

volatile t_leap_status leap_status;
extern volatile bool leap_aktiv;

void HandListener::onInit(const Leap::Controller& controller) {
	std::cout << "LeapMotion initialisiert" << std::endl;
}

void HandListener::onConnect(const Leap::Controller& controller) {
	std::cout << "LeapMotion verbunden" << std::endl;
	leap_status = CONNECTED;
}

void HandListener::onFrame(const Leap::Controller& controller) {
	unsigned char winkel[14];
	const Leap::Frame frame = controller.frame();
	int handCount = frame.hands().count();
	switch (handCount) {
		case 0:
			//Fehlermeldung, keine Hand erkannt
			leap_status = NOHAND;
			break;
		case 1:
			leap_status = TRACKING;
			if (leap_aktiv) {
				winkelBerechnen(frame.hands()[0], winkel);
				setAngles(winkel);
			}
			break;
		default:
			//Fehlermeldung, es darf nur eine Hand im Blickfeld sein
			leap_status = TOOMANYHANDS;
			break;
	}
}

void HandListener::winkelBerechnen(const Leap::Hand& hand, unsigned char *winkel) {
	float rad;
	const Leap::FingerList fingers = hand.fingers();

	// Daumen
	const Leap::Finger daumen = fingers.fingerType(Leap::Finger::TYPE_THUMB)[0];
	rad = daumen.bone(Leap::Bone::TYPE_PROXIMAL).direction().angleTo(daumen.bone(Leap::Bone::TYPE_INTERMEDIATE).direction());
	winkel[0] = rad * 180.0 / M_PI;
	rad = daumen.bone(Leap::Bone::TYPE_INTERMEDIATE).direction().angleTo(daumen.bone(Leap::Bone::TYPE_DISTAL).direction());
	winkel[1] = rad * 180.0 / M_PI;

	// Zeigefinger
	const Leap::Finger zeigefinger = fingers.fingerType(Leap::Finger::TYPE_INDEX)[0];
	rad = zeigefinger.bone(Leap::Bone::TYPE_METACARPAL).direction().angleTo(zeigefinger.bone(Leap::Bone::TYPE_PROXIMAL).direction());
	winkel[2] = rad * 180.0 / M_PI;
	rad = zeigefinger.bone(Leap::Bone::TYPE_PROXIMAL).direction().angleTo(zeigefinger.bone(Leap::Bone::TYPE_INTERMEDIATE).direction());
	winkel[3] = rad * 180.0 / M_PI;
	rad = zeigefinger.bone(Leap::Bone::TYPE_INTERMEDIATE).direction().angleTo(zeigefinger.bone(Leap::Bone::TYPE_DISTAL).direction());
	winkel[4] = rad * 180.0 / M_PI;

	// Mittelfinger
	const Leap::Finger mittelfinger = fingers.fingerType(Leap::Finger::TYPE_MIDDLE)[0];
	rad = mittelfinger.bone(Leap::Bone::TYPE_METACARPAL).direction().angleTo(mittelfinger.bone(Leap::Bone::TYPE_PROXIMAL).direction());
	winkel[5] = rad * 180.0 / M_PI;
	rad = mittelfinger.bone(Leap::Bone::TYPE_PROXIMAL).direction().angleTo(mittelfinger.bone(Leap::Bone::TYPE_INTERMEDIATE).direction());
	winkel[6] = rad * 180.0 / M_PI;
	rad = mittelfinger.bone(Leap::Bone::TYPE_INTERMEDIATE).direction().angleTo(mittelfinger.bone(Leap::Bone::TYPE_DISTAL).direction());
	winkel[7] = rad * 180.0 / M_PI;

	// Ringfinger
	const Leap::Finger ringfinger = fingers.fingerType(Leap::Finger::TYPE_RING)[0];
	rad = ringfinger.bone(Leap::Bone::TYPE_METACARPAL).direction().angleTo(ringfinger.bone(Leap::Bone::TYPE_PROXIMAL).direction());
	winkel[8] = rad * 180.0 / M_PI;
	rad = ringfinger.bone(Leap::Bone::TYPE_PROXIMAL).direction().angleTo(ringfinger.bone(Leap::Bone::TYPE_INTERMEDIATE).direction());
	winkel[9] = rad * 180.0 / M_PI;
	rad = ringfinger.bone(Leap::Bone::TYPE_INTERMEDIATE).direction().angleTo(ringfinger.bone(Leap::Bone::TYPE_DISTAL).direction());
	winkel[10] = rad * 180.0 / M_PI;

	// Kleiner Finger
	const Leap::Finger kleinerfinger = fingers.fingerType(Leap::Finger::TYPE_PINKY)[0];
	rad = kleinerfinger.bone(Leap::Bone::TYPE_METACARPAL).direction().angleTo(kleinerfinger.bone(Leap::Bone::TYPE_PROXIMAL).direction());
	winkel[11] = rad * 180.0 / M_PI;
	rad = kleinerfinger.bone(Leap::Bone::TYPE_PROXIMAL).direction().angleTo(kleinerfinger.bone(Leap::Bone::TYPE_INTERMEDIATE).direction());
	winkel[12] = rad * 180.0 / M_PI;
	rad = kleinerfinger.bone(Leap::Bone::TYPE_INTERMEDIATE).direction().angleTo(kleinerfinger.bone(Leap::Bone::TYPE_DISTAL).direction());
	winkel[13] = rad * 180.0 / M_PI;
}
