// robot.cpp


#include "WPILib.h"
#include "OperatorInputs.h"
#include "const.h"
#include "drivetrain.h"
#include "camera.h"
#include "compressor.h"
#include "Picker.h"
#include "Shooter.h"
#include "Portcullis.h"
#include "Climber.h"
#include "Autonomous.h"
#include "VisionTargeting.h"


using namespace std;


class Robot: public IterativeRobot
{
private:
	// live window variables
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const string autoNameDefault = "Default";
	const string autoNameCustom = "My Auto";
	string autoSelected;

	// class variables
	OperatorInputs *inputs;
	Drivetrain *drivetrain;
	Camera *camera;
	Picker *picker;
	Shooter *shooter;
	Portcullis *portcullis;
	Climber *climber;
	Autonomous *autonomous;
	VisionTargeting *vision;

	// variables
	Compressor *compressor;


void RobotInit()
{
	// live window inits
	chooser = new SendableChooser();
	chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
	chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
	SmartDashboard::PutData("Auto Modes", chooser);

	// class inits
	inputs = new OperatorInputs();
	drivetrain = new Drivetrain(inputs, &m_ds);
	camera = new Camera(inputs, drivetrain);
	picker = new Picker(inputs);
	shooter = new Shooter(inputs);
	portcullis = new Portcullis(inputs);
	climber = new Climber(inputs);
	autonomous = new Autonomous(inputs, drivetrain);
	vision = new VisionTargeting(inputs, drivetrain);

	// variable inits
	compressor = new Compressor(PCM_COMPRESSOR_SOLENOID);
	camera->Init();
}


/*
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
	drivetrain->Init();
	camera->Start();
	compressor->Start();
	autonomous->Init();

/*
	autoSelected = *((string*)chooser->GetSelected());
	//string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
	cout << "Auto selected: " << autoSelected << endl;

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


void AutonomousPeriodic()
{
	autonomous->Loop();
	camera->Loop();
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
	drivetrain->Init();
	camera->Start();
	compressor->Start();
}


void TeleopPeriodic()
{
	if (!vision->Targeting())
	{
		drivetrain->setPower();
		drivetrain->childProofShift();
		camera->Loop();
		picker->Loop();
		shooter->Loop();
		portcullis->Loop();
		climber->Loop();
	}
	vision->Loop();
}


void TestInit()
{
	drivetrain->Init();
	camera->Start();
	compressor->Start();
}


void TestPeriodic()
{
	drivetrain->setPower();
	drivetrain->childProofShift();
	camera->Loop();
	picker->Loop();
	shooter->Loop();
	portcullis->Loop();
	climber->Loop();
	vision->Loop();
}


void DisabledInit()
{
	drivetrain->setGearLow();
	camera->Stop();
	compressor->Stop();
}


};


START_ROBOT_CLASS(Robot)
