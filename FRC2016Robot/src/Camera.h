// camera.h


#ifndef CAMERA_H_
#define CAMERA_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include "Relay.h"
#include "USBCamera.h"


class Camera
{
public:
	Camera(OperatorInputs *operatorinputs, Drivetrain *drivetrain);
	~Camera();
	void Init();
	void Loop();
	void Attributes();
	void Stop();

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;

	// usb camera variables
	Image *m_frame;
	IMAQdxSession m_front;
	IMAQdxSession m_rear;
	IMAQdxSession m_current;
	//USBCamera* m_front;
	//USBCamera* m_rear;
	//USBCamera* m_current;

	// led variables
	Relay *m_led;
};


#endif /* CAMERA_H_ */
