// camera.h


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include "Relay.h"


#ifndef CAMERA_H_
#define CAMERA_H_


class Camera
{
public:
	Camera(OperatorInputs *operinputs, Drivetrain *drtrain);
	~Camera();
	void Init();
	void Start();
	void Stop();
	void Loop();

protected:
	OperatorInputs *inputs;
	Drivetrain *drivetrain;

	// usb camera variables
	Image *imaqframe;
	IMAQdxError imaqerror;
	IMAQdxSession imaqfront;
	IMAQdxSession imaqrear;
	IMAQdxSession imaqcurrent;

	// direction variables
	bool prevdir;

	// led variables
	Relay *relay_led;
	bool prevLED;
};


#endif /* CAMERA_H_ */
