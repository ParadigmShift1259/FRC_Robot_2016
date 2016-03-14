/*
 * DrivetrainDistancePID.h
 *
 *  Created on: Mar 8, 2016
 *      Author: John
 */

#ifndef SRC_DRIVETRAINDISTANCEPID_H_
#define SRC_DRIVETRAINDISTANCEPID_H_

#include <Commands/PIDSubsystem.h>
#include "Drivetrain.h"

namespace std {

class DrivetrainDistancePID: public PIDSubsystem {
public:
	DrivetrainDistancePID(Drivetrain *dt);
	virtual ~DrivetrainDistancePID();
	double ReturnPIDInput();
	void UsePIDOutput(double output);
	void Enable();
	void Disable();
private:
	Drivetrain *m_drivetrain;
};

} /* namespace std */

#endif /* SRC_DRIVETRAINDISTANCEPID_H_ */
