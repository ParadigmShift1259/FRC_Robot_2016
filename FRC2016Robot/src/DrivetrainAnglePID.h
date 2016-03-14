/*
 * DrivetrainAnglePID.h
 *
 *  Created on: Mar 7, 2016
 *      Author: John
 */

#ifndef SRC_DRIVETRAINANGLEPID_H_
#define SRC_DRIVETRAINANGLEPID_H_

#include <Commands/PIDSubsystem.h>
#include "Drivetrain.h"

namespace std {

class DrivetrainAnglePID: public PIDSubsystem {
public:
	DrivetrainAnglePID(Drivetrain *dt);
	~DrivetrainAnglePID();
	double ReturnPIDInput();
	void UsePIDOutput(double output);
	void Enable();
	void Disable();

protected:
	Drivetrain *m_drivetrain;
};


} /* namespace std */

#endif /* SRC_DRIVETRAINANGLEPID_H_ */
