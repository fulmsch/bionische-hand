#include "leapmotion.h"

#include <iostream>

void HandListener::onInit(const Leap::Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void HandListener::onConnect(const Leap::Controller& controller) {
  std::cout << "Connected" << std::endl;
}

void HandListener::onFrame(const Leap::Controller& controller) {
	unsigned char winkel[14];
	const Leap::Frame frame = controller.frame();
	int handCount = frame.hands().count();
	printf("Hands: %d\n", handCount);
	if (handCount == 0) {
		//Fehlermeldung, keine Hand erkannt
	} else if (handCount == 1) {
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
	} else {
		//Fehlermeldung, es darf nur eine Hand im Blickfeld sein
	}
}
