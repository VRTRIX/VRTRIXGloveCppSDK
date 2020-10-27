// VRTRIXDataGloveClientTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include "client/VRTRIXDataGloveClient.h"
#include "COMMANDS.h"
//#define DEBUGLOG


std::chrono::steady_clock::time_point LH_TimePoint;
std::chrono::steady_clock::time_point RH_TimePoint;
int LH_Count = 0;
int RH_Count = 0;
bool bLHInitialized = false;
bool bRHInitialized = false;

// Helper function to return joint name as a string by its index.
std::string GetJointNamebyIndex(int index)
{
	switch (static_cast<VRTRIX::Joint>(index)) {
	case VRTRIX::Pinky_Intermediate: return "Pinky_Intermediate";
	case VRTRIX::Ring_Intermediate: return "Ring_Intermediate";
	case VRTRIX::Middle_Intermediate: return "Middle_Intermediate";
	case VRTRIX::Index_Intermediate: return "Index_Intermediate";
	case VRTRIX::Thumb_Distal: return "Thumb_Distal";
	case VRTRIX::Wrist_Joint: return "Wrist_Joint";
	case VRTRIX::Pinky_Proximal: return "Pinky_Proximal";
	case VRTRIX::Ring_Proximal: return "Ring_Proximal";
	case VRTRIX::Middle_Proximal: return "Middle_Proximal";
	case VRTRIX::Index_Proximal: return "Index_Proximal";
	case VRTRIX::Thumb_Intermediate: return "Thumb_Intermediate";
	case VRTRIX::Pinky_Distal: return "Pinky_Distal";
	case VRTRIX::Ring_Distal: return "Ring_Distal";
	case VRTRIX::Middle_Distal: return "Middle_Distal";
	case VRTRIX::Index_Distal: return "Index_Distal";
	case VRTRIX::Thumb_Proximal: return "Thumb_Proximal";
	default: return "Unknown";
	}
}

class CVRTRIXIMUEventHandler :public VRTRIX::IVRTRIXIMUEventHandler
{
	/** OnReceivedNewPose event call back function implement
	*
	* @param pose: Pose struct returned by the call back function
	* @param pUserParam: user defined parameter
	* @returns void
	*/
	void OnReceivedNewPose(VRTRIX::Pose pose, void* pUserParam) {

#ifdef DEBUGLOG
		switch (pose.type)
		{
		case(VRTRIX::Hand_Left): {
			std::cout << "Left Hand Data Packet Received:" << std::endl;
			std::cout << "	Battery (Left Hand): " << pose.battery << "%" << std::endl;
			std::cout << "	Radio Strength (Left Hand): " << pose.radioStrength << "dB" << std::endl;
			std::cout << "	Cal Score (Left Hand): " << pose.calScore << std::endl;
			for (int i = 0; i < (sizeof(pose.imuData) / sizeof(*pose.imuData)); i++) {
				std::cout << "	" << GetJointNamebyIndex(i) << ": " << pose.imuData[i];
			}
			std::cout << std::endl;
		}
		break;
		case(VRTRIX::Hand_Right): {
			std::cout << "Right Hand Data Packet Received:" << std::endl;
			std::cout << "	Battery (Right Hand): " << pose.battery << "%" << std::endl;
			std::cout << "	Radio Strength (Right Hand): " << pose.radioStrength << "dB" << std::endl;
			std::cout << "	Cal Score (Right Hand): " << pose.calScore << std::endl;
			for (int i = 0; i < (sizeof(pose.imuData) / sizeof(*pose.imuData)); i++) {
				std::cout << "	" << GetJointNamebyIndex(i) << ": " << pose.imuData[i];
			}
			std::cout << std::endl;
		}
		break;
		default:
			break;
		}
#endif
		if (!bLHInitialized && pose.type == VRTRIX::Hand_Left) {
			VRTRIX::IVRTRIXDataGloveClient* pDataGlove = (VRTRIX::IVRTRIXDataGloveClient*)pUserParam;
			VRTRIX::EIMUError error = VRTRIX::IMUError_None;
			pDataGlove->AlgorithmTuning(error, VRTRIX::Thumb_Proximal, VRTRIX::AlgorithmConfig_ProximalSlerpDown, 0.4f);
			pDataGlove->AlgorithmTuning(error, VRTRIX::Thumb_Distal, VRTRIX::AlgorithmConfig_DistalSlerpDown, 0.7f);
			pDataGlove->AlgorithmTuning(error, VRTRIX::Wrist_Joint, VRTRIX::AlgorithmConfig_FingerSpacing, 5.f);
			pDataGlove->AlgorithmTuning(error, VRTRIX::Wrist_Joint, VRTRIX::AlgorithmConfig_FinalFingerSpacing, 2.f);
			bLHInitialized = true;
		}
		if (!bRHInitialized && pose.type == VRTRIX::Hand_Right) {
			VRTRIX::IVRTRIXDataGloveClient* pDataGlove = (VRTRIX::IVRTRIXDataGloveClient*)pUserParam;
			VRTRIX::EIMUError error = VRTRIX::IMUError_None;
			pDataGlove->AlgorithmTuning(error, VRTRIX::Thumb_Proximal, VRTRIX::AlgorithmConfig_ProximalSlerpDown, 0.4f);
			pDataGlove->AlgorithmTuning(error, VRTRIX::Thumb_Distal, VRTRIX::AlgorithmConfig_DistalSlerpDown, 0.7f);
			pDataGlove->AlgorithmTuning(error, VRTRIX::Wrist_Joint, VRTRIX::AlgorithmConfig_FingerSpacing, 5.f);
			pDataGlove->AlgorithmTuning(error, VRTRIX::Wrist_Joint, VRTRIX::AlgorithmConfig_FinalFingerSpacing, 2.f);
			bRHInitialized = true;
		}


		if (pose.type == VRTRIX::Hand_Left) {
			if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - LH_TimePoint).count() > 1000.f) {
				LH_TimePoint = std::chrono::steady_clock::now();
				std::cout << "Left Hand Glove Data Rate: " << LH_Count << std::endl;
				LH_Count = 0;
			}
			else {
				LH_Count++;
			}
		}
		else if (pose.type == VRTRIX::Hand_Right) {
			if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - RH_TimePoint).count() > 1000.f) {
				RH_TimePoint = std::chrono::steady_clock::now();
				std::cout << "Right Hand Glove Data Rate: " << RH_Count << std::endl;
				RH_Count = 0;
			}
			else {
				RH_Count++;
			}
		}
	}

	/** OnReceivedNewEvent event call back function implement
	*
	* @param event: Event struct returned by the call back function
	* @param pUserParam: user defined parameter
	* @returns void
	*/
	void OnReceivedNewEvent(VRTRIX::HandEvent event, void* pUserParam) {
		switch (event.stat) {
		case(VRTRIX::HandStatus_Connected): {
			if (event.type == VRTRIX::Hand_Left) {
				LH_TimePoint = std::chrono::steady_clock::now();
				std::cout << "Left Hand Data Glove Connected!" << std::endl;
			}
			else if (event.type == VRTRIX::Hand_Right) {
				RH_TimePoint = std::chrono::steady_clock::now();
				std::cout << "Right Hand Data Glove Connected!" << std::endl;
			}
			break;
		}
		case(VRTRIX::HandStatus_Disconnected): {
			if (event.type == VRTRIX::Hand_Left) {
				std::cout << "Left Hand Data Glove Disconnected!" << std::endl;
			}
			else if (event.type == VRTRIX::Hand_Right) {
				std::cout << "Right Hand Data Glove Disconnected!" << std::endl;
			}
			break;
		}
		default: break;
		}
	}
};

/** Thread function for initializing data gloves & performing data streaming
*
* @param type: Hand type enum
* @param pDataGlove: Data glove object pointer, returned after it is initialized.
* @returns void
*/
void InitDataGloveStreaming(VRTRIX::HandType type, VRTRIX::IVRTRIXDataGloveClient* &pDataGlove) {

	VRTRIX::EInitError eInitError;
	VRTRIX::EIMUError eIMUError;
	//Initialize event handler.
	VRTRIX::IVRTRIXIMUEventHandler* pEventHandler = new CVRTRIXIMUEventHandler();
	//Initialize data glove.
	pDataGlove = InitDataGlove(eInitError, VRTRIX::InitMode_Normal, VRTRIX::PRO);
	if (eInitError != VRTRIX::InitError_None) {
		std::cout << "Error: " << eInitError << std::endl;
		return;
	}
	//Register event call back and perform events handling/pose updating.
	pDataGlove->RegisterIMUDataCallback(pEventHandler, pDataGlove);
	//Prepare PortInfo struct and open the tcp connection.
	VRTRIX::PortInfo portInfo;
	portInfo.IP = "127.0.0.1";
	portInfo.port = std::to_string(PORT_TCP);
	portInfo.type = type;
	pDataGlove->ConnectDataGlove(eIMUError, portInfo);

	if (eIMUError != VRTRIX::IMUError_None) {
		if (type == VRTRIX::Hand_Left) std::cout << "LEFT HAND CONNECTION OPEN FAILED! " << std::endl;
		else std::cout << "RIGHT HAND CONNECTION OPEN FAILED! " << std::endl;
	}
}

int main()
{
	VRTRIX::IVRTRIXDataGloveClient* pRightHandDataGlove;
	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << "Right Hand Glove Initialization Start!" << std::endl;
	std::cout << "**************************************" << std::endl;
	InitDataGloveStreaming(VRTRIX::Hand_Right, pRightHandDataGlove);
	std::cout << std::endl;

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(10ms);

	////Start left hand data streaming
	VRTRIX::IVRTRIXDataGloveClient* pLeftHandDataGlove;
	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << "Left Hand Glove Initialization Start!" << std::endl;
	std::cout << "**************************************" << std::endl;
	InitDataGloveStreaming(VRTRIX::Hand_Left, pLeftHandDataGlove);
	std::cout << std::endl;

	while (1) {
		int key = _getch();
		//If Esc is pressed, close the connection and exit the program.
		if (key == 27) {
			std::cout << "Close Connection" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->DisconnectDataGlove(eIMUError);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->DisconnectDataGlove(eIMUError);
			}
			break;
		}
		//If a is pressed, trigger haptics once
		if (key == 97) {
			std::cout << "Trigger Haptics Once" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->Vibrate(eIMUError);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->Vibrate(eIMUError);
			}
		}
		//If b is pressed, trigger haptics 500ms
		if (key == 98) {
			std::cout << "Trigger Haptics 500ms" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->VibratePeriod(eIMUError, 500);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->VibratePeriod(eIMUError, 500);
			}
		}
		//If c is pressed, Toggle haptics
		if (key == 99) {
			std::cout << "Toggle Haptics" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->ToggleVibration(eIMUError);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->ToggleVibration(eIMUError);
			}
		}
		//If d is pressed, save hardware calibration
		if (key == 100) {
			std::cout << "Save Hardware Calibration" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->HardwareCalibrate(eIMUError);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->HardwareCalibrate(eIMUError);
			}
		}
		//If e is pressed, perform t-pose calibration
		if (key == 101) {
			std::cout << "TPose Calibration" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->TPoseCalibration(eIMUError);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->TPoseCalibration(eIMUError);
			}
		}
		//If f is pressed, switch to advanced mode
		if (key == 102) {
			std::cout << "Switch to Advanced Mode" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->SwitchToAdvancedMode(eIMUError, true);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->SwitchToAdvancedMode(eIMUError, true);
			}
		}
		//If g is pressed, switch hardware version to PRO
		if (key == 103) {
			std::cout << "Switch to PRO Hardware" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->SwitchHardwareVersion(eIMUError, VRTRIX::PRO);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->SwitchHardwareVersion(eIMUError, VRTRIX::PRO);
			}
		}
		//If h is pressed, set radio channel limit: 65-99
		if (key == 104) {
			std::cout << "Set Radio Channel Limit" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->SetRadioChannelLimit(eIMUError, 99, 65);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->SetRadioChannelLimit(eIMUError, 99, 65);
			}
		}
		//If i is pressed, perform channel hopping
		if (key == 105) {
			std::cout << "Channel Hopping" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->ChannelHopping(eIMUError, 95);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->ChannelHopping(eIMUError, 93);
			}
		}
		//If j is pressed, perform algorithm tuning
		if (key == 106) {
			std::cout << "Algorithm Tuning" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->AlgorithmTuning(eIMUError, VRTRIX::Index_Intermediate, VRTRIX::AlgorithmConfig_FingerSpacing, 5.f);
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->AlgorithmTuning(eIMUError, VRTRIX::Thumb_Distal, VRTRIX::AlgorithmConfig_ThumbOffset, 0.f, {15.f, 15.f, 0.f});
			}
		}
		//If k is pressed, set wrist reference pose
		if (key == 107) {
			std::cout << "Set Ref Pose" << std::endl;
			if (pLeftHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pLeftHandDataGlove->SetRefPose(eIMUError, { 0.f, 0.f, 0.f, 1.0f });
			}
			if (pRightHandDataGlove != nullptr) {
				VRTRIX::EIMUError eIMUError;
				pRightHandDataGlove->SetRefPose(eIMUError, { 0.f, 0.f, 0.f, 1.0f });
			}
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
