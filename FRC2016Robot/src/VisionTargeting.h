// visiontargeting.h


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

protected:
	Drivetrain *drive;
	OperatorInputs *inputs;
};


#endif /* SRC_VISIONTARGETING_H_ */
