// autonomous.h


#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include <AnalogGyro.h>
#include <BuiltinAccelerometer.h>


class Autonomous
{
public:
	Autonomous(OperatorInputs *operatorinputs, Drivetrain *drivetrain);
	~Autonomous();
	void Init();
	void Loop();

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	AnalogGyro *m_gyro;
	Accelerometer *m_accel;
	double m_angle;
	int m_counter;
};


#endif /* AUTONOMOUS_H_ */
