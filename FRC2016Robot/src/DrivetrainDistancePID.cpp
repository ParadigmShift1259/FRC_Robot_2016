/*
 * DrivetrainDistancePID.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: John
 */

#include <DrivetrainDistancePID.h>

namespace std {

#define DRIVE_DISTANCE_P 1.0
#define DRIVE_DISTANCE_I 0
#define DRIVE_DISTANCE_D 5

DrivetrainDistancePID::DrivetrainDistancePID(Drivetrain *dt) : PIDSubsystem(DRIVE_DISTANCE_P,DRIVE_DISTANCE_I,DRIVE_DISTANCE_D){
	m_drivetrain = dt;
}

double DrivetrainDistancePID::ReturnPIDInput() {
	return m_drivetrain->GetDistance();
}
void DrivetrainDistancePID::UsePIDOutput(double output) {
	m_drivetrain->DriveMagnitude(output);
	m_drivetrain->DriveAuto();
}
void DrivetrainDistancePID::Enable() {
	m_drivetrain->DisableAnglePID();
	this->GetPIDController()->Enable();

}
void DrivetrainDistancePID::Disable() {
	m_drivetrain->DisableDistancePID();
	this->GetPIDController()->Disable();
}

DrivetrainDistancePID::~DrivetrainDistancePID() {
}

} /* namespace std */
