// robot.cpp


#include "WPILib.h"
#include "OperatorInputs.h"
#include "drivetrain.h"
#include "compressor.h"
#include "Climber.h"
#include "Picker.h"
#include "Shooter.h"
#include "relay.h"
#include "Portcullis.h"


class Robot: public IterativeRobot
{
private:
	// live window variables
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	// main class variables
	OperatorInputs *inputs;
	Drivetrain *drivetrain;
	Compressor *compressor;
	Climber *climber;
	Picker *picker;
	Shooter *shooter;
	Portcullis *portcullis;

	// camera variables
	Image *imaqframe;
	IMAQdxError imaqError;
	IMAQdxSession imaqfront;
	IMAQdxSession imaqrear;
	IMAQdxSession imaqcurrent;

	// led variables
	bool prevLED;
	Relay *relay_led;

	// direction variables
	bool prevdir;

	// autonomous variables
	int counter = 0;


void RobotInit()
{
	// live window inits
	chooser = new SendableChooser();
	chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
	chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
	SmartDashboard::PutData("Auto Modes", chooser);

	// main class inits
	inputs = new OperatorInputs();
	drivetrain = new Drivetrain( inputs,&m_ds);
	compressor = new Compressor(0);
	picker = new Picker(inputs);
	climber = new Climber(inputs);
	shooter = new Shooter(inputs);
	portcullis = new Portcullis(inputs);

	// camera inits
	// create an image
	imaqframe = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
	imaqfront = 0;
	imaqrear = 0;
	// the camera name (ex "cam0") can be found through the roborio web interface
	// front camera "cam4"
	imaqError = IMAQdxOpenCamera("cam4", IMAQdxCameraControlModeController, &imaqfront);
	if (imaqError != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)imaqError) + "\n");
		imaqfront = 0;
	}
	else
	{
		imaqError = IMAQdxConfigureGrab(imaqfront);
		if (imaqError != IMAQdxErrorSuccess)
		{
			DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");
			imaqfront = 0;
		}
	}
	// rear camera "cam2"
	imaqError = IMAQdxOpenCamera("cam2", IMAQdxCameraControlModeController, &imaqrear);
	if (imaqError != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)imaqError) + "\n");
		imaqrear = 0;
	}
	else
	{
		imaqError = IMAQdxConfigureGrab(imaqrear);
		if (imaqError != IMAQdxErrorSuccess)
		{
			DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");
			imaqrear = 0;
		}
	}
	// assign initial session
	imaqcurrent = imaqfront;

	// led inits
	prevLED = false;
	relay_led = new Relay(0);

	// direction inits
	prevdir = false;
}


/**
 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
 * below the Gyro
 *
 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
 * If using the SendableChooser make sure to add them to the chooser code above as well.
 */
void AutonomousInit()
{
	counter = 0;
	autoSelected = *((std::string*)chooser->GetSelected());
	//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
	std::cout << "Auto selected: " << autoSelected << std::endl;

	if (autoSelected == autoNameCustom)
	{
		//Custom Auto goes here
	}
	else
	{
		//Default Auto goes here
	}
}


void AutonomousPeriodic()
{
	if (counter < 4)
	{
		if (drivetrain->getIsDoneDriving() == false)
		{
			drivetrain->driveDistance(2.0);
		}
		if (drivetrain->getIsDoneDriving() == true)
		{
			drivetrain->setAngle(90);
		}
		if (drivetrain->getIsTurning() == true && drivetrain->getIsDoneDriving() == true)
		{
			//drivetrain->turnAngle();
		}
		if (drivetrain->getIsTurning() == false && drivetrain->getIsDoneDriving() == true)
		{
			counter++;
			drivetrain->driveDistance(2);
		}
	}

	/*
	if (autoSelected == autoNameCustom)
	{
		//Custom Auto goes here
	}
	else
	{
		//Default Auto goes here
	}
	*/
}


void TeleopInit()
{
	compressor->Start();
	drivetrain->Init();

	// acquire images
	if (imaqcurrent)
		IMAQdxStartAcquisition(imaqcurrent);
}

void TeleopPeriodic()
{
	drivetrain->setPower();
	drivetrain->childProofShift();
	picker->Loop();
	portcullis->Loop();
	LEDToggle();

	// process camera frame
	if (imaqcurrent)
	{
		imaqError = IMAQdxGrab(imaqcurrent, imaqframe, true, NULL);
		if (imaqError != IMAQdxErrorSuccess)
		{
			DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
		}
		else
		{
			//imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(imaqframe);
		}
	}
}


void TestInit()
{
	drivetrain->Init();
}


void TestPeriodic()
{
	drivetrain->TestLoop();
}


void DisabledInit()
{
	compressor->Stop();
	drivetrain->setGearLow();
	if (imaqcurrent)
		IMAQdxStopAcquisition(imaqcurrent);
}


void LEDToggle()
{
	if (inputs->button7() && !prevLED)
	{
		if (relay_led->Get() == Relay::Value::kOff)
		{
			relay_led->Set(Relay::Value::kForward);
		}
		else
		{
			relay_led->Set(Relay::Value::kOff);
		}
		prevLED = true;
	}
	if (!inputs->button7())
	{
		prevLED = false;
	}
}


void ChangeDirection()
{
	if (inputs->xBoxR3() && !prevdir)
	{
		drivetrain->ChangeDirection();
		prevdir = true;
	}
	if (!inputs->xBoxR3())
	{
		prevdir = false;
	}
}


};


START_ROBOT_CLASS(Robot)
