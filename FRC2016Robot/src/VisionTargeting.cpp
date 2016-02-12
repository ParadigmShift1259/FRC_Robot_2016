// visiontargeting.cpp


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
	if (width1 >= width2 && width1 >= width3)
	{
		return width1;
	}

	if (width2 >= width1 && width2 >= width3)
	{
		return width1;
	}

	if (width3 >= width2 && width3 >= width1)
	{
		return width1;
	}

	return width1;
}

/*void VisionTargeting::FindCorners(double x,double y,double height,double width);
{
	x1 = x-width;
	y1 = y+height;

}*/

void VisionTargeting::DetectDirectionNeeded()
{

}
