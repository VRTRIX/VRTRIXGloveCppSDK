//COMMANDS.h
//
//Listing of the command numbers to send to the server
//constants.h
//
//Contains any #define's that will be used in the application

#pragma once
#define PORT_TCP   11002
#define PORT_UDP   11003
#define MAX_BUFF    2048
#define MAX_SEND_BUFF 2048

//commands that can be issued by the client app
#define CMD_STOP						0
#define CMD_TriggerHaptics				1
#define CMD_ToggleHaptics				2
#define CMD_HardwareCalibration			3
#define CMD_TPoseCalibration			4
#define CMD_SetAdvancedMode				5
#define CMD_SetHardwareVersion			6
#define CMD_SetRadioLimit				7
#define CMD_ChannelHopping				8
#define CMD_AlgorithmTuning				9
#define CMD_SetRotationFormat			10
#define CMD_OKPoseCalibration			11
#define CMD_SetRefPose					12