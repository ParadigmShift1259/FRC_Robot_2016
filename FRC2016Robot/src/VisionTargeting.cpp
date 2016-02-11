/*
 * VisionTargeting.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: Developer
 */
#include "visiontargeting.h"

VisionTargeting::VisionTargeting(Drivetrain *drivetrain, OperatorInputs *operatorinputs)
{
	drive = drivetrain;
	inputs = operatorinputs;

}
VisionTargeting::~VisionTargeting()
{

}
double VisionTargeting::PickBestContour(double width1, double width2, double width3)
{
	return 1.0;

}
