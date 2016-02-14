/*
 * Position.cpp
 *
 *  Created on: Feb 13, 2016
 *      Author: Owner
 */

#include "Position.h"
#include "math.h"
#define PI 3.14159265


	Position::Position():
	mxp(I2C::Port::kMXP, 40)
	{
		mxp.ZeroYaw();
		xPos = Constants::xStartPos;
		yPos = Constants::yStartPos;
		angle = mxp.GetYaw();
		pitch = mxp.GetPitch();
		encoderTicks = 0;
	}

	inline float Position::GetAngle() {
		return (mxp.GetYaw() + Constants::gyroOffset) * PI / 180;
	}

	inline float Position::GetPitch() {
		return mxp.GetPitch() * PI / 180;
	}

	void Position::TrackX(bool movingForward) {
		float distance;
		float xDistance;
		angle = GetAngle();
		pitch = GetPitch();
		distance = encoderTicks / Constants::ticksPerRotation * 2 * PI * wheelRadius;
		xDistance = distance * cos(angle) * cos(pitch);
		/*
		 * you're basically just rotating the 2D plane to be using the Z and X axes, so using cosine of pitch
		 * will work to give you the correct value. I can draw it out for anyone who wants
		 */
		if (movingForward) {
			xPos = xPos + xDistance;
		} else {
			xPos = xPos - xDistance;
		}
	}

	void Position::TrackY(bool movingForward) {
		float distance;
		float yDistance;
		angle = GetAngle();
		pitch = GetPitch();
		distance = encoderTicks / Constants::ticksPerRotation * 2 * PI * wheelRadius;
		yDistance = distance * sin(angle) * cos(pitch);
		/*
		 * you're basically just rotating the 2D plane to be using the Z and Y axes, so using the cosine of pitch
		 * will work to give you the correct value. I can draw it out for anyone who wants
		 */
		if (movingForward) {
			yPos = yPos + yDistance;
		} else {
			yPos = yPos - yDistance;
		}
	}

	void Position::Update(bool movingForward) {
		TrackX(movingForward);
		TrackY(movingForward);
	}

	int Position::NearestObstacle() {
		double obstacleDistance [5] = {
				sqrt(pow(xPos - obstacleXPos[0], 2) + pow(yPos - obstacleYPos[0], 2)),
				sqrt(pow(xPos - obstacleXPos[1], 2) + pow(yPos - obstacleYPos[1], 2)),
				sqrt(pow(xPos - obstacleXPos[2], 2) + pow(yPos - obstacleYPos[2], 2)),
				sqrt(pow(xPos - obstacleXPos[3], 2) + pow(yPos - obstacleYPos[3], 2)),
				sqrt(pow(xPos - obstacleXPos[4], 2) + pow(yPos - obstacleYPos[4], 2))
		};
		int nearestObstacle = 0;
		int minDistance = obstacleDistance[0];
		for (int i = 0; i < 5; i++) {
			if (obstacleDistance[i] < minDistance) {
				minDistance = obstacleDistance[i];
				nearestObstacle = i;
			}
		}
		return nearestObstacle;
	}

	float Position::GetX() {
		return xPos;
	}

	float Position::GetY() {
		return yPos;
	}

	float Position::AngleToTower() {
		float theta = mxp.GetAngle();
		float xToTower = Constants::towerX - xPos;
		float yToTower = Constants::towerY - yPos;
		float dotProduct;
		float uLength;
		float vLength;
		float angleToTower;
		dotProduct = (-1 * xPos) * (xToTower) + (-1 * xPos * tan(90 - theta)) * (yToTower);
		uLength = sqrt(pow(-1 * xPos, 2) + pow(-1 * xPos * tan(90 - theta), 2));
		vLength = sqrt(pow(xToTower, 2) + pow(yToTower, 2));
		angleToTower = acos(dotProduct/(uLength * vLength)); //linear algebra
		return angleToTower;
	}

	float Position::DistanceToTower() {
		float xPart;
		float yPart;
		float distance;
		xPart = Constants::towerX - xPos;
		yPart = Constants::towerY - yPos;
		distance = sqrt(pow(xPart, 2) + pow(yPart, 2));
		return distance;
	}

	void Position::Calibrate() {
		int nearestObstacle = NearestObstacle();
		xPos = obstacleXPos[nearestObstacle];
		yPos = obstacleYPos[nearestObstacle];
	}
