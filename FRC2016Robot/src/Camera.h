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
	void Start();
	void Stop();
	void Loop();

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;

	// usb camera variables
	Image *m_frame;
	IMAQdxSession m_front;
	IMAQdxSession m_rear;
	IMAQdxSession m_current;

	// direction variables
	bool m_prevdir;

	// led variables
	Relay *m_led;
	bool m_prevled;
};


#endif /* CAMERA_H_ */
