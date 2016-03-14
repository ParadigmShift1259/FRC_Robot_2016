/*
 * DrivetrainAnglePID.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: John
 */

#include <DrivetrainAnglePID.h>

namespace std {

#define DRIVE_ANGLE_P 1.0
#define DRIVE_ANGLE_I 0.001
#define DRIVE_ANGLE_D 10

DrivetrainAnglePID::DrivetrainAnglePID(Drivetrain *dt) :
		PIDSubsystem(DRIVE_ANGLE_P, DRIVE_ANGLE_I, DRIVE_ANGLE_D) {
	m_drivetrain = dt;
}

void DrivetrainDistancePID::Enable() {
	m_drivetrain->EnableDistancePID();
	this->GetPIDController()->Enable();

}
void DrivetrainDistancePID::Disable() {
	m_drivetrain->DisableDistancePID();
	this->GetPIDController()->Disable();
}

double DrivetrainAnglePID::ReturnPIDInput() {
	return m_drivetrain->GetAngle();
}

void DrivetrainAnglePID::UsePIDOutput(double output) {
	m_drivetrain->DriveAngle(output);
	m_drivetrain->DriveAuto();
}

DrivetrainAnglePID::~DrivetrainAnglePID() {
}

} /* namespace std */
