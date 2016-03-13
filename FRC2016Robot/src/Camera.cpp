// camera.cpp


#include "Camera.h"
#include "const.h"
#include <regex>


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
	bool imaqerror;

	// camera inits
	// the camera name (ex "cam0") can be found through the roborio web interface
	if (m_front == 0)
	{
		// front camera "cam4"
		imaqerror = true;
		SAFE_IMAQ_CALL_BARE(IMAQdxOpenCamera,USB_CAMERA_FRONT, IMAQdxCameraControlModeController, &m_front)
		else
			imaqerror=false;
		if (imaqerror)
			m_front = 0;
	}
	if (m_rear == 0)
	{
		// rear camera "cam2"
		imaqerror = true;
		SAFE_IMAQ_CALL_BARE(IMAQdxOpenCamera,USB_CAMERA_REAR, IMAQdxCameraControlModeController, &m_rear)
		else
			imaqerror=false;
		if (imaqerror)
			m_rear = 0;
	}

	// assign initial session
	if (m_current == 0)
	{
		SAFE_IMAQ_CALL_BARE(IMAQdxConfigureGrab,m_front)
		else
			m_current = m_front;
	}

	// acquire images
	if (m_current)
		IMAQdxStartAcquisition(m_current);
}


void Camera::Loop()
{
	IMAQdxError imaqerror;

	bool dirbutton = false;//m_inputs->xBoxR3();
	bool ledbutton = false;//m_inputs->button7();

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
		SAFE_IMAQ_CALL_BARE(IMAQdxGrab,m_current, m_frame, true, NULL)
		else
		{
			//imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(m_frame);
		}
	}
}


void Camera::Stop()
{
	// stop acquire images
	if (m_current)
		IMAQdxStopAcquisition(m_current);
}


void Camera::UpdateSettings(IMAQdxSession id) {
	priority_recursive_mutex m_mutex;
	unsigned int m_width = 320;
	unsigned int m_height = 240;
	double m_fps = 30;
	string m_whiteBalance = AUTO;
	bool m_whiteBalanceValuePresent;
	unsigned int m_whiteBalanceValue;
	unsigned int m_exposureValue = 50;
	bool m_exposureValuePresent;
	bool m_useJpeg;
	string m_exposure = MANUAL;
	unsigned int m_brightness = 80;
	std::lock_guard<priority_recursive_mutex> lock(m_mutex);
	//bool wasActive = m_active;

	//if (wasActive) StopCapture();
	//if (m_open) CloseCamera();
	//OpenCamera();

	uInt32 count = 0;
	uInt32 currentMode = 0;
	SAFE_IMAQ_CALL(IMAQdxEnumerateVideoModes, id, nullptr, &count, &currentMode);
	auto modes = std::make_unique<IMAQdxVideoMode[]>(count);
	SAFE_IMAQ_CALL(IMAQdxEnumerateVideoModes, id, modes.get(), &count, &currentMode);

	// Groups are:
	// 0 - width
	// 1 - height
	// 2 - format
	// 3 - fps
	std::regex reMode("([0-9]+)\\s*x\\s*([0-9]+)\\s+(.*?)\\s+([0-9.]+)\\s*fps");
	IMAQdxVideoMode* foundMode = nullptr;
	IMAQdxVideoMode* currentModePtr = &modes[currentMode];
	double foundFps = 1000.0;

	// Loop through the modes, and find the match with the lowest fps
	for (unsigned int i = 0; i < count; i++) {
		std::cmatch m;
		if (!std::regex_match(modes[i].Name, m, reMode)) continue;
		unsigned int width = (unsigned int)std::stoul(m[1].str());
		unsigned int height = (unsigned int)std::stoul(m[2].str());
		if (width != m_width) continue;
		if (height != m_height) continue;
		double fps = atof(m[4].str().c_str());
		if (fps < m_fps) continue;
		if (fps > foundFps) continue;
		bool isJpeg =
				m[3].str().compare("jpeg") == 0 || m[3].str().compare("JPEG") == 0;
		if ((m_useJpeg && !isJpeg) || (!m_useJpeg && isJpeg)) continue;
		foundMode = &modes[i];
		foundFps = fps;
	}
	if (foundMode != nullptr) {
		if (foundMode->Value != currentModePtr->Value) {
			SAFE_IMAQ_CALL(IMAQdxSetAttribute, id, IMAQdxAttributeVideoMode,
					IMAQdxValueTypeU32, foundMode->Value);
		}
	}
	if (m_whiteBalance.compare(AUTO) == 0) {
		SAFE_IMAQ_CALL(IMAQdxSetAttribute, id, ATTR_WB_MODE,
				IMAQdxValueTypeString, AUTO);
	} else {
		SAFE_IMAQ_CALL(IMAQdxSetAttribute, id, ATTR_WB_MODE,
				IMAQdxValueTypeString, MANUAL);
		if (m_whiteBalanceValuePresent)
			SAFE_IMAQ_CALL(IMAQdxSetAttribute, id, ATTR_WB_VALUE,
					IMAQdxValueTypeU32, m_whiteBalanceValue);
	}
	if (m_exposure.compare(AUTO) == 0) {
		SAFE_IMAQ_CALL(IMAQdxSetAttribute, id, ATTR_EX_MODE,
				IMAQdxValueTypeString,
				std::string("AutoAperaturePriority").c_str());
	} else {
		SAFE_IMAQ_CALL(IMAQdxSetAttribute, id, ATTR_EX_MODE,
				IMAQdxValueTypeString, MANUAL);
		if (m_exposureValuePresent) {
			double minv = 0.0;
			double maxv = 0.0;
			SAFE_IMAQ_CALL(IMAQdxGetAttributeMinimum, id, ATTR_EX_VALUE,
					IMAQdxValueTypeF64, &minv);
			SAFE_IMAQ_CALL(IMAQdxGetAttributeMaximum, id, ATTR_EX_VALUE,
					IMAQdxValueTypeF64, &maxv);
			double val = minv + ((maxv - minv) * ((double)m_exposureValue / 100.0));
			SAFE_IMAQ_CALL(IMAQdxSetAttribute, id, ATTR_EX_VALUE,
					IMAQdxValueTypeF64, val);
		}
	}
	SAFE_IMAQ_CALL(IMAQdxSetAttribute, id, ATTR_BR_MODE, IMAQdxValueTypeString,
			MANUAL);
	double minv = 0.0;
	double maxv = 0.0;
	SAFE_IMAQ_CALL(IMAQdxGetAttributeMinimum, id, ATTR_BR_VALUE,
			IMAQdxValueTypeF64, &minv);
	SAFE_IMAQ_CALL(IMAQdxGetAttributeMaximum, id, ATTR_BR_VALUE,
			IMAQdxValueTypeF64, &maxv);
	double val = minv + ((maxv - minv) * ((double)m_brightness / 100.0));
	SAFE_IMAQ_CALL(IMAQdxSetAttribute, id, ATTR_BR_VALUE, IMAQdxValueTypeF64,
			val);
	//if (wasActive) StartCapture();
}
