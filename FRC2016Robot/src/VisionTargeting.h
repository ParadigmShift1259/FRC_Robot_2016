/*
 * VisionTargeting.h
 *
 *  Created on: Feb 8, 2016
 *      Author: Developer
 */

#ifndef SRC_VISIONTARGETING_H_
#define SRC_VISIONTARGETING_H_

#include "Drivetrain.h"
#include "OperatorInputs.h"

class VisionTargeting
{
public:
	VisionTargeting(Drivetrain *drivetrain, OperatorInputs *operatorinputs);
	~VisionTargeting();
	double PickBestContour(double width1, double width2, double width3);
	void DetectDirectionNeeded();
	void Movement();

	Drivetrain *drive;
	OperatorInputs *inputs;



protected:

private:

};



#endif /* SRC_VISIONTARGETING_H_ */
