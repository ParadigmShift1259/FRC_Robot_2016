#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_
#include <SpeedController.h>
#include <Solenoid.h>
#include "OperatorInputs.h"


class Shooter
{
public:
	Shooter(OperatorInputs* inputs);
	~Shooter();
	void ShootBall();




protected:
	SpeedController *wheelMotor;
	Solenoid *arm;
	OperatorInputs *input;




};
#endif
