// camera.h


#ifndef CAMERA_H_
#define CAMERA_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include "Relay.h"


class Camera
{
public:
	Camera(OperatorInputs *operatorinputs, Drivetrain *drivetrain);
	~Camera();
	void Init();
	void Loop();
	void Stop();

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;

	// usb camera variables
	Image *m_frame;
	IMAQdxSession m_front;
	IMAQdxSession m_rear;
	IMAQdxSession m_current;

	// led variables
	Relay *m_led;
};


#endif /* CAMERA_H_ */
