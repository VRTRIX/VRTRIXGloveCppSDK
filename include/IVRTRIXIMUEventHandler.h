//------------------------------------------------------------------------
/**
\file		IVRTRIXIMUEventHandler.h
\brief		Contains the VRTRIX glove event handler base class.
\Date       2018-8-3
\Version    1.1
*/
//------------------------------------------------------------------------
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#define IMU_NUM 6
namespace VRTRIX
{
	/** enum values of hand type */
	enum HandType
	{
		Hand_None = 0,		   
		Hand_Other = 1,		  
		Hand_Left = 2,	 
		Hand_Right = 3,		   
		Hand_Both = 4,  // Currently not supported
	};

	/** enum values of joint type */
	enum Joint {
		Pinky_Intermediate = 0,
		Ring_Intermediate = 1,
		Middle_Intermediate = 2,
		Index_Intermediate = 3,
		Thumb_Distal = 4,
		Wrist_Joint = 5,
		Pinky_Proximal = 6,
		Ring_Proximal = 7,
		Middle_Proximal = 8,
		Index_Proximal = 9,
		Thumb_Intermediate = 10,
		Pinky_Distal = 11,
		Ring_Distal = 12,
		Middle_Distal = 13,
		Index_Distal = 14,
		Thumb_Proximal = 15,
		Joint_MAX = 16
	};

	/** enum values to pass into InitDataGlove to identify what kind of initialization error is arised. */
	enum EInitError
	{
		InitError_None = 0,
		InitError_Unknown = 1,
		InitError_PortBusy = 2,
		InitError_ConnectionRefused = 3,
		InitError_ConnectionReset = 4,
		InitError_NoSuchDevice = 5
	};

	/** enum values to pass into methods to identify what kind of IMU error is arised. */
	enum EIMUError
	{
		IMUError_None = 0,
		IMUError_Unknown = 1,
		IMUError_ConnectionAorted = 2,
		IMUError_ConnectionInterrupted = 3,
		IMUError_ConnectionBusy = 3,
		IMUError_NotConnected = 5,
		IMUError_TimedOut = 6,
		IMUError_PortNotFound = 7
	};

	struct PortInfo {
		/*! Address of the serial port (this can be passed to the constructor of Serial). */
		std::string port;

		/*! Human readable description of serial device if available. */
		std::string description;

		/*! Hardware ID (e.g. VID:PID of USB serial devices) or "n/a" if not available. */
		std::string hardware_id;

		/*! Instance ID */
		std::string instance_id;

		/*! Baud Rate */
		int baud_rate;

		/*! Hand Type */
		HandType type;
	};


	struct VRTRIXQuaternion_t
	{
		float qx, qy, qz, qw;
		friend std::ostream& operator << (std::ostream &o, const VRTRIXQuaternion_t a) {
			o << "[" << std::setiosflags(std::ios::fixed) << std::setprecision(4)
				<< a.qx << "," << a.qy << "," << a.qz << "," << a.qw  << "]" << std::endl;
			return o;
		}
	};

	struct Pose
	{
		VRTRIXQuaternion_t imuData[Joint_MAX];
		HandType type;
		int calScore[IMU_NUM];
		int radioStrength;
		double battery;
	};


	class IVRTRIXIMUEventHandler
	{
	public:
		/** Uninitialization (Destructor)
		*/
		virtual ~IVRTRIXIMUEventHandler(void) {};


		/** OnReceivedNewPose event call back function implement
		*
		* @param pose: Pose struct returned by the call back function
		* @param pUserParam: user defined parameter
		* @returns void
		*/
		virtual void OnReceivedNewPose(Pose pose, void* pUserParam) = 0;
	};
}