#include "leapmotion.h"

#include <iostream>

volatile t_leap_status leap_status;

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
//	printf("Hands: %d\n", handCount);
	switch (handCount) {
		case 0:
			//Fehlermeldung, keine Hand erkannt
			leap_status = NOHAND;
			break;
		case 1:
			leap_status = TRACKING;
			/*
			const Leap::Hand hand = frame.hands()[0];
			const Leap::FingerList fingers = hand.fingers();

			for (Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
				const Leap::Finger finger = *fl;

				// Get finger bones
				for (int b = 0; b < 5; b++) {
					for (int i = 0; i < 4; i++) {
						float angle = finger.bone(static_cast<Leap::Bone::Type>(i)).direction().angleTo(finger.bone(static_cast<Leap::Bone::Type>(i+1)).direction()) * 180.0 / M_PI;
						winkel[5*b+i-1] = angle;
						printf("%d\n", winkel);
					}
				}
			}
			*/
			break;
		default:
			//Fehlermeldung, es darf nur eine Hand im Blickfeld sein
			leap_status = TOOMANYHANDS;
			break;
	}
}
