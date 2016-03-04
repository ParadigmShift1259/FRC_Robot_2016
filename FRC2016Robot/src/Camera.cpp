// camera.cpp


#include "Camera.h"
#include "const.h"
#include "Vision/VisionAPI.h"

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
	// the camera name (ex "cam0") can be found through the roborio web interface
	if (m_front == 0)
	{
		// front camera "cam4"
		imaqerror = IMAQdxOpenCamera(USB_CAMERA_FRONT, IMAQdxCameraControlModeController, &m_front);
		if (imaqerror != IMAQdxErrorSuccess)
		{
			DriverStation::ReportError("Front IMAQdxOpenCamera error: " + std::to_string((long)imaqerror) + "\n");
			m_front = 0;
		}
	}
	if (m_rear == 0)
	{
		// rear camera "cam2"
		imaqerror = IMAQdxOpenCamera(USB_CAMERA_REAR, IMAQdxCameraControlModeController, &m_rear);
		if (imaqerror != IMAQdxErrorSuccess)
		{
			DriverStation::ReportError("Rear IMAQdxOpenCamera error: " + std::to_string((long)imaqerror) + "\n");
			m_rear = 0;
		}
	}

	// assign initial session
	if (m_current == 0)
	{
		imaqerror = IMAQdxConfigureGrab(m_front);
		if (imaqerror != IMAQdxErrorSuccess)
			DriverStation::ReportError("Front IMAQdxConfigureGrab error: " + std::to_string((long)imaqerror) + "\n");
		else
			m_current = m_front;
	}

	// acquire images
	if (m_current)
		IMAQdxStartAcquisition(m_current);
	IMAQdxEnumItem attribs[256];
	uInt32 size = 256;
	IMAQdxEnumerateAttributeValues(m_front, USB_CAMERA_FRONT, attribs, &size);

	//SmartDashboard::PutNumber("Camera Attributes",size);
	for (unsigned int i = 0; i < 256; i++)
	{
		DriverStation::ReportError(((string)attribs[i].Name) + ": " + std::to_string(attribs[i].Value) + "\n");
		//printf("%s: %lu\n", attribs[i].Name, attribs[i].Value);
	}
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
			Image *frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
			//frcCrop(frame, m_frame, {320,60,1280,960});
			//frcScale(m_frame,frame,4,4,IMAQ_SCALE_SMALLER);
			//imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(m_frame);
		}
	}
}

void Camera::Attributes()
{
/*	unsigned int width;
	bool32 writewidth;
	//IMAQdxAttributeInformation_struct attributes;
	IMAQdxGetAttribute(m_current,"Width",IMAQdxValueTypeU32,(void*)(&width));
	SmartDashboard::PutNumber("Camera Width",width);
	IMAQdxIsAttributeWritable(m_current,"Width",&writewidth);
	SmartDashboard::PutNumber("Camera Width Write",writewidth);
	IMAQdxSetAttribute(m_current,"Width",IMAQdxValueTypeU32,(void*)(&width));*/
}


void Camera::Stop()
{
	// stop acquire images
	if (m_current)
		IMAQdxStopAcquisition(m_current);
}
