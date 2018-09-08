// VRTRIXDataGloveTest.cpp : Demo project to show the usage of VRTRIX_IMU.dll
//

#include "stdafx.h"
#include <conio.h>
#include <thread>
#include "VRTRIX_IMU.h"

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
		switch (pose.type)
		{
		case(VRTRIX::Hand_Left): {
			/*std::cout << "Left Hand Data Packet Received:" << std::endl;
			std::cout << "	Battery (Left Hand): " << pose.battery << "%"<< std::endl;
			std::cout << "	Radio Strength (Left Hand): " << pose.radioStrength << std::endl;
			for (int i = 0; i < (sizeof(pose.imuData) / sizeof(*pose.imuData)); i++) {
			std::cout << "	" << GetJointNamebyIndex(i) << ": " << pose.imuData[i];
			}
			std::cout << std::endl;*/
		}
		break;
		case(VRTRIX::Hand_Right): {
			/*std::cout << "Right Hand Data Packet Received:" << std::endl;
			std::cout << "	Battery (Right Hand): " << pose.battery << std::endl;
			std::cout << "	Radio Strength (Right Hand): " << pose.radioStrength << std::endl;
			for (int i = 0; i < (sizeof(pose.imuData) / sizeof(*pose.imuData)); i++) {
			std::cout << GetJointNamebyIndex(i) << ": " << pose.imuData[i];
			}
			std::cout << std::endl;*/
		}
		break;
		default:
		break;
		}
	}
};

/** Thread function for initializing data gloves & performing data streaming
*
* @param type: Hand type enum
* @param pDataGlove: Data glove object pointer, returned after it is initialized.
* @returns void
*/
void DataGloveThread(VRTRIX::HandType type, VRTRIX::IVRTRIXIMU* &pDataGlove) {

	VRTRIX::EInitError eInitError;
	VRTRIX::EIMUError eIMUError;
	//Initialize event handler.
	VRTRIX::IVRTRIXIMUEventHandler* pEventHandler = new CVRTRIXIMUEventHandler();
	//Initialize data glove.
	pDataGlove = InitDataGlove(eInitError);
	//Register event call back and perform events handling/pose updating.
	pDataGlove->RegisterIMUDataCallback(pEventHandler);
	if (eInitError != VRTRIX::InitError_None) {
		std::cout << "Error: " << eInitError << std::endl;
		return;
	}
	//Prepare PortInfo struct and open the data streaming serial port of glove.
	//Note: the default baud_rate is 1000000 and should NOT be changed.
	VRTRIX::PortInfo portInfo;
	portInfo.baud_rate = 1000000;
	portInfo.type = type;
	pDataGlove->OpenPort(eIMUError, portInfo);

	if (eIMUError == VRTRIX::IMUError_None) {
		//Print out full port information
		std::cout << "PORT DESCIPTION: " << portInfo.description << std::endl;
		std::cout << "PORT HARDWARE ID: " << portInfo.hardware_id << std::endl;
		std::cout << "PORT INSTANCE ID: " << portInfo.instance_id << std::endl;
		std::cout << "PORT NAME: " << portInfo.port << std::endl;
		std::cout << "PORT BAUD RATE: " << portInfo.baud_rate << std::endl;
		//Start data streaming.
		pDataGlove->StartDataStreaming(eIMUError, portInfo);
	}
}


int main()
{
	//Start a new thread for right hand data streaming
	VRTRIX::EIMUError eIMUError;
	VRTRIX::IVRTRIXIMU* pRightHandDataGlove;
	std::thread rightHand(DataGloveThread, VRTRIX::Hand_Right, std::ref(pRightHandDataGlove));
	rightHand.detach();

	//Start a new thread for left hand data streaming
	VRTRIX::IVRTRIXIMU* pLeftHandDataGlove;
	std::thread leftHand(DataGloveThread, VRTRIX::Hand_Left, std::ref(pLeftHandDataGlove));
	leftHand.detach();

	while (1) {
		int key = _getch();
		//If Esc is pressed, close the connection and exit the program.
		if (key == 27) {
			pRightHandDataGlove->ClosePort();
			pLeftHandDataGlove->ClosePort();
			break;
		}
		//If v is pressed, trigger a 1s haptic pulse for both hands.
		if (key == 118) {
			pRightHandDataGlove->VibratePeriod(eIMUError, 1000);
			pLeftHandDataGlove->VibratePeriod(eIMUError, 1000);
		}
		//If r is pressed, align both finger orientation to the hand (what we called "software alignment")
		// Note: Only use this when advanced mode is set to true.
		if (key == 114) {
			pRightHandDataGlove->SoftwareAlign(eIMUError);
			pLeftHandDataGlove->SoftwareAlign(eIMUError);
		}
		//If c is pressed, save current background calibration file to the hardware ROM (what we called "software alignment")
		// Note: All data gloves have performed IN-FACTORY hardware calibration, no need to do it again unless the environment 
		//       magnetic field has been changed dramatically. 
		if (key == 99) {
			pRightHandDataGlove->HardwareCalibrate(eIMUError);
			pLeftHandDataGlove->HardwareCalibrate(eIMUError);
		}

	}

	return 0;
}

