// visiontargeting.h


#ifndef SRC_VISIONTARGETING_H_
#define SRC_VISIONTARGETING_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"


class VisionTargeting
{
public:
	VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain);
	~VisionTargeting();

	double PickBestContour(double width1, double width2, double width3);
	//void FindCorners(double x1,double y1,double height,double width);
	//void DetectDirectionNeeded();
	//void Movement();

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;

private:
/*	double x1;
	double x2;
	double x3;
	double x4;
	double y1;
	double y2;
	double y3;
	double y4;*/
};


#endif /* SRC_VISIONTARGETING_H_ */
