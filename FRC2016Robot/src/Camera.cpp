// camera.cpp


#include "Camera.h"
#include "const.h"


Camera::Camera(OperatorInputs *operatorinputs, Drivetrain *drivetrain)
{
	m_inputs = operatorinputs;
	m_drivetrain = drivetrain;

	m_frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
	m_front = 0;
	m_rear = 0;
	m_current = 0;

	m_led = new Relay(RLY_CAMERA_LED);
}


Camera::~Camera()
{
	delete m_led;
}


void Camera::Init()
{
	IMAQdxError imaqerror;

	// camera inits
	// create an image
	m_front = 0;
	m_rear = 0;
	// the camera name (ex "cam0") can be found through the roborio web interface
	// front camera "cam4"
	imaqerror = IMAQdxOpenCamera(USB_CAMERA_FRONT, IMAQdxCameraControlModeController, &m_front);
	if (imaqerror != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("Front IMAQdxOpenCamera error: " + std::to_string((long)imaqerror) + "\n");
		m_front = 0;
	}
	// rear camera "cam2"
	imaqerror = IMAQdxOpenCamera(USB_CAMERA_REAR, IMAQdxCameraControlModeController, &m_rear);
	if (imaqerror != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("Rear IMAQdxOpenCamera error: " + std::to_string((long)imaqerror) + "\n");
		m_rear = 0;
	}
	// assign initial session
	if (m_front)
	{
		imaqerror = IMAQdxConfigureGrab(m_front);
		if (imaqerror != IMAQdxErrorSuccess)
			DriverStation::ReportError("Front IMAQdxConfigureGrab error: " + std::to_string((long)imaqerror) + "\n");
		else
			m_current = m_front;
	}
}


void Camera::Start()
{
	// acquire images
	if (m_current)
		IMAQdxStartAcquisition(m_current);
}


void Camera::Stop()
{
	// stop acquire images
	if (m_current)
		IMAQdxStopAcquisition(m_current);
}


void Camera::Loop()
{
	IMAQdxError imaqerror;

	bool dirbutton = m_inputs->xBoxR3();
	bool ledbutton = m_inputs->button7();

	if (dirbutton)
	{
		bool forward = m_drivetrain->ChangeDirection();

		IMAQdxStopAcquisition(m_current);
		m_current = 0;
		if (forward)
		{
			imaqerror = IMAQdxConfigureGrab(m_front);
			if (imaqerror != IMAQdxErrorSuccess)
				DriverStation::ReportError("Front IMAQdxConfigureGrab error: " + std::to_string((long)imaqerror) + "\n");
			else
				m_current = m_front;
			m_led->Set(Relay::Value::kOff);
		}
		else
		{
			imaqerror = IMAQdxConfigureGrab(m_rear);
			if (imaqerror != IMAQdxErrorSuccess)
				DriverStation::ReportError("Rear IMAQdxConfigureGrab error: " + std::to_string((long)imaqerror) + "\n");
			else
			{
				m_current = m_rear;
				m_led->Set(Relay::Value::kForward);
			}
		}
	}

	// incorporate this code into the camera swap in the future
	if (ledbutton)
	{
		if (m_led->Get() == Relay::Value::kOff)
		{
			// led on when moving reverse
			m_led->Set(Relay::Value::kForward);
		}
		else
		{
			// led off when moving forward
			m_led->Set(Relay::Value::kOff);
		}
	}

	// process camera frame
	if (m_current)
	{
		imaqerror = IMAQdxGrab(m_current, m_frame, true, NULL);
		if (imaqerror != IMAQdxErrorSuccess)
		{
			DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqerror) + "\n");
		}
		else
		{
			//imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(m_frame);
		}
	}
}
