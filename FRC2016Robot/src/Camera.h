// camera.h


#ifndef CAMERA_H_
#define CAMERA_H_
#define SAFE_IMAQ_CALL(funName, ...) \
{ \
	unsigned int error = funName(__VA_ARGS__); \
	if (error != IMAQdxErrorSuccess) \
	{ \
		string report1 = #funName; \
		string report2 = #__VA_ARGS__; \
		DriverStation::ReportError(report1 + "(" + report2 + "): " + std::to_string((long)error) + "\n"); \
	} \
}
#define SAFE_IMAQ_CALL_BARE(funName, ...) \
unsigned int error = funName(__VA_ARGS__); \
if (error != IMAQdxErrorSuccess) \
{ \
	string report1 = #funName; \
	string report2 = #__VA_ARGS__; \
	DriverStation::ReportError(report1 + "(" + report2 + "): " + std::to_string((long)error) + "\n"); \
}

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
	void UpdateSettings(IMAQdxSession id);

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

private:
	static constexpr char const *ATTR_WB_MODE =
			"CameraAttributes::WhiteBalance::Mode";
	static constexpr char const *ATTR_WB_VALUE =
			"CameraAttributes::WhiteBalance::Value";
	static constexpr char const *ATTR_EX_MODE =
			"CameraAttributes::Exposure::Mode";
	static constexpr char const *ATTR_EX_VALUE =
			"CameraAttributes::Exposure::Value";
	static constexpr char const *ATTR_BR_MODE =
			"CameraAttributes::Brightness::Mode";
	static constexpr char const *ATTR_BR_VALUE =
			"CameraAttributes::Brightness::Value";

	// Constants for the manual and auto types
	static constexpr char const* AUTO = "Auto";
	static constexpr char const* MANUAL = "Manual";
};


#endif /* CAMERA_H_ */
