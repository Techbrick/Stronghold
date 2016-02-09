#ifndef SRC_CONSTANTS_H
#define SRC_CONSTANTS_H

namespace Constants {
	//PWM Out Pins
	static constexpr int driveLeftPin	= 1;
	static constexpr int driveRightPin	= 0;
	
	//static constexpr int shooterLeftPin = 99;
	//static constexpr int shooterRightPin = 99;
	
	static constexpr int driveStickChannel = 1;
	static constexpr int shooterStickChannel = 0;
	
	static constexpr int driveLeftStickX = 0;
	static constexpr int driveLeftStickY = 1;
	static constexpr int driveL2 = 3;
	
	
	//Noah's addins for position class
	static constexpr float xStartPos = 0; //relative to origin
	static constexpr float yStartPos = 0; //relative to origin
	static constexpr float towerX = 0; //relative to origin
	static constexpr float towerY = 0; //relative to origin
}

#endif
