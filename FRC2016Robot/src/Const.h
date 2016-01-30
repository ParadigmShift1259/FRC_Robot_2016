// const.h


#ifndef SRC_CONST_H_
#define SRC_CONST_H_


//OperatorInputs
//	Controllers
#define JOYSTICK_NUMBER 0
#define XBOX_NUMBER 1


//	XBox Controller
//		Buttons
#define A_BUTTON  1
#define B_BUTTON  2
#define X_BUTTON  3
#define Y_BUTTON  4
#define RIGHT_BUMPER  6
#define LEFT_BUMPER  5
#define BACK_BUTTON  7
#define START_BUTTON  8


//		XBox Triggers -- Changed for 2016, previously XBOX triggers were both on a single axis
#define XBOX_RIGHT_TRIGGER_AXIS  3
#define XBOX_LEFT_TRIGGER_AXIS  2
#define RIGHT_TRIGGER_MIN  0.5
#define RIGHT_TRIGGER_MAX  1.0
#define LEFT_TRIGGER_MIN  0.5
#define LEFT_TRIGGER_MAX  1.0


//	Controller Dead Zones
#define DEADZONE_Y  0.17
#define DEADZONE_X  0.17
#define DEADZONE_Z  0.17


//  Drivetrain
//	Talons
#define LEFT_PORT 0
#define SECOND_LEFT_PORT 2
#define RIGHT_PORT 1
#define SECOND_RIGHT_PORT 3


//	Shifter
#define SHIFT_PORT_LOW 0
#define SHIFT_MODULE 0
#define CHILD_PROOF_SPEED 0.75
#define FLIP_HIGH_GEAR true


//	Ramping
#define RAMPING_RATE 0.5


//	Encoders
#define ENCODER_TOP_SPEED 0.6
#define ENCODER_WAIT_TIME 168
#define DISTANCE_PER_PULSE 0.0006708


//Pickup
#define PICKER_DEPLOY 1
#define PICKER_VENT 2
#define PICKER_VENT_DELAY 50
#define INGEST_MOTOR 0


//Opener
#define SPARK_OPENER 1
#define OPENER_TIMEOUT 250			// 250 = ~5 seconds


//Shooter
#define ACTUATOR_SOLENOID 2
#define SPARK_SHOOTER 2


//Climber
#define PISTON 3
#define SPARK_WINCH 3


#endif /* SRC_CONST_H_ */
