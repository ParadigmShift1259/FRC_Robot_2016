// camera.cpp


#include "Camera.h"
#include "const.h"


Camera::Camera(OperatorInputs *operinputs, Drivetrain *drtrain)
{
	inputs = operinputs;
	drivetrain = drtrain;

	imaqframe = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
	imaqerror = IMAQdxError::IMAQdxErrorSuccess;
	imaqfront = 0;
	imaqrear = 0;
	imaqcurrent = 0;

	prevdir = false;

	relay_led = new Relay(0);
	prevLED = false;
}


Camera::~Camera()
{
	delete relay_led;
}


void Camera::Init()
{
	// camera inits
	// create an image
	imaqfront = 0;
	imaqrear = 0;
	// the camera name (ex "cam0") can be found through the roborio web interface
	// front camera "cam4"
	imaqerror = IMAQdxOpenCamera(CAMERA_FRONT, IMAQdxCameraControlModeController, &imaqfront);
	if (imaqerror != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("Front IMAQdxOpenCamera error: " + std::to_string((long)imaqerror) + "\n");
		imaqfront = 0;
	}
	// rear camera "cam2"
	imaqerror = IMAQdxOpenCamera(CAMERA_REAR, IMAQdxCameraControlModeController, &imaqrear);
	if (imaqerror != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("Rear IMAQdxOpenCamera error: " + std::to_string((long)imaqerror) + "\n");
		imaqrear = 0;
	}
	// assign initial session
	if (imaqfront)
	{
		imaqerror = IMAQdxConfigureGrab(imaqfront);
		if (imaqerror != IMAQdxErrorSuccess)
			DriverStation::ReportError("Front IMAQdxConfigureGrab error: " + std::to_string((long)imaqerror) + "\n");
		else
			imaqcurrent = imaqfront;
	}
}


void Camera::Start()
{
	// acquire images
	if (imaqcurrent)
		IMAQdxStartAcquisition(imaqcurrent);
}


void Camera::Stop()
{
	// stop acquire images
	if (imaqcurrent)
		IMAQdxStopAcquisition(imaqcurrent);
}


void Camera::Loop()
{
	bool dirbutton = inputs->xBoxR3();
	bool ledbutton = inputs->button7();

	if (dirbutton && !prevdir)
	{
		bool forward = drivetrain->ChangeDirection();

		IMAQdxStopAcquisition(imaqcurrent);
		imaqcurrent = 0;
		if (forward)
		{
			imaqerror = IMAQdxConfigureGrab(imaqfront);
			if (imaqerror != IMAQdxErrorSuccess)
				DriverStation::ReportError("Front IMAQdxConfigureGrab error: " + std::to_string((long)imaqerror) + "\n");
			else
				imaqcurrent = imaqfront;
			relay_led->Set(Relay::Value::kOff);
		}
		else
		{
			imaqerror = IMAQdxConfigureGrab(imaqrear);
			if (imaqerror != IMAQdxErrorSuccess)
				DriverStation::ReportError("Rear IMAQdxConfigureGrab error: " + std::to_string((long)imaqerror) + "\n");
			else
			{
				imaqcurrent = imaqrear;
				relay_led->Set(Relay::Value::kForward);
			}
		}
	}
	prevdir = dirbutton;

	// incorporate this code into the camera swap in the future
	if (ledbutton && !prevLED)
	{
		if (relay_led->Get() == Relay::Value::kOff)
		{
			// led on when moving reverse
			relay_led->Set(Relay::Value::kForward);
		}
		else
		{
			// led off when moving forward
			relay_led->Set(Relay::Value::kOff);
		}
	}
	prevLED = ledbutton;

	// process camera frame
	if (imaqcurrent)
	{
		imaqerror = IMAQdxGrab(imaqcurrent, imaqframe, true, NULL);
		if (imaqerror != IMAQdxErrorSuccess)
		{
			DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqerror) + "\n");
		}
		else
		{
			//imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(imaqframe);
		}
	}
}
