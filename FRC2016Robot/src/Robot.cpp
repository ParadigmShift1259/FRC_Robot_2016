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
	bool prevPress;
	int hasCalibrated = false;
	IMAQdxSession session;
	Image *frame;
	IMAQdxError imaqError;
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;
	Drivetrain *drivetrain;
	OperatorInputs *inputs;
	Compressor *compressor;
	Climber *climber;
	Picker *picker;
	Shooter *shooter;
	Relay *relay;
	Portcullis *portcullis;
	int counter = 0;


void RobotInit()
{
	chooser = new SendableChooser();
	chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
	chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
	SmartDashboard::PutData("Auto Modes", chooser);
	inputs = new OperatorInputs();
	drivetrain = new Drivetrain( inputs,&m_ds);
	compressor = new Compressor(0);
	picker = new Picker(inputs);
	climber = new Climber(inputs);
	shooter = new Shooter(inputs);
	relay = new Relay(0);
	portcullis = new Portcullis(inputs);
	/*
	// create an image
	frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
	//the camera name (ex "cam0") can be found through the roborio web interface
	imaqError = IMAQdxOpenCamera("cam4", IMAQdxCameraControlModeController, &session);
	if(imaqError != IMAQdxErrorSuccess) {
	DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)imaqError) + "\n");
	}
	imaqError = IMAQdxConfigureGrab(session);
	if(imaqError != IMAQdxErrorSuccess) {
	DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");

	}
	**/
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

	if (autoSelected == autoNameCustom){
		//Custom Auto goes here
	} else {
		//Default Auto goes here
	}
}


void AutonomousPeriodic()
{
	if (counter<4)
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
	if(autoSelected == autoNameCustom){
		//Custom Auto goes here
	} else {
		//Default Auto goes here
	}
	*/
}


void TeleopInit()
{
	compressor->Start();
	drivetrain->Init();
	// acquire images
	IMAQdxStartAcquisition(session);
	// grab an image, draw the circle, and provide it for the camera server which will
	// in turn send it to the dashboard.

}

void TeleopPeriodic()
{
	drivetrain->setPower();
	drivetrain->childProofShift();
	picker->Loop();
	portcullis->Loop();
	/**IMAQdxGrab(session, frame, true, NULL);
	if(imaqError != IMAQdxErrorSuccess) {
		DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
	} else {
	//	imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
		CameraServer::GetInstance()->SetImage(frame);
	}*/
	LEDToggle();
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
	IMAQdxStopAcquisition(session);
}


void LEDToggle()
{
	if (inputs->button7() == true && prevPress == false)
	{
		if (relay->Get() == Relay::Value::kOff)
		{
			relay->Set(Relay::Value::kForward);
		}
		else
		{
			relay->Set(Relay::Value::kOff);
		}
			prevPress = true;
	}
		if (inputs->button7() == false)
		{
			prevPress = false;
		}
}

void ChangeDirection()
{
	if (inputs->xBoxR3() == true && prevPress == false)
		{
			drivetrain->ChangeDirection();
				prevPress = true;
		}
			if (inputs->xBoxR3() == false)
			{
				prevPress = false;
			}
}
};


START_ROBOT_CLASS(Robot)
