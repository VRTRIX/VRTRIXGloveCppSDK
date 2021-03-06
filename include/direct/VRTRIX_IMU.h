//============= Copyright (c) VRTRIX INC, All rights reserved. ================
//
// Purpose: Contains VRTRIX Data Glove interface class which includes API functions 
//          for calling. Also, dll entry point are defined in this header file.
//
//=============================================================================
#pragma once
#ifdef VRTRIXIMU_EXPORTS
#define VRTRIXIMU_INTERFACE extern "C" __declspec( dllexport )
#else
#define VRTRIXIMU_INTERFACE extern "C" __declspec( dllimport )
#endif

#include "IVRTRIXIMUEventHandler.h"

namespace VRTRIX
{
    //!  VRTRIX Data Glove interface class. 
    /*!
        Interface class that contains most APIs for calling
    */
	class IVRTRIXIMU
	{
	public:
		virtual ~IVRTRIXIMU(){};

		/** Start the data streaming serial port according to the PortInfo
		*	Note that this function contains a infinite loop inside, the loop only breaks when 
		*	error occurs during data streaming.
		*
		* @param eError: return the error during data streaming, if any.
		* @param info: struct contains port information.
		* @returns void.
		*/
		virtual void StartDataStreaming(EIMUError &eError, const PortInfo &info) = 0;

		/** Perform hardware calibrate process.
		*	Note: All data gloves have performed IN-FACTORY hardware calibration, no need to do it again  
	    *   unless the environment magnetic field has been changed dramatically. 
		* @param eError: return the error during calibration, if any.
		* @returns void.
		*/
		virtual void HardwareCalibrate(EIMUError &eError) = 0;

		/** Perform hardware pairing
		*	Note: All data gloves have performed IN-FACTORY pairing, no need to do it again
		*   unless the pairing is lost due to unexpected reason.
		* @param channel: channel to pair.
		* @param eError: return the error during pairing, if any.
		* @returns void.
		*/
		virtual void RequestToPair(int channel, EIMUError &eError) = 0;		

		/** Reset hardware to rssi noise scan mode
		* @param eError: return the error during pairing, if any.
		* @returns void.
		*/
		virtual void RequestToRSSIScan(EIMUError &eError) = 0;		
		
		/** Trigger a haptic pulse to the glove.
		* @param eError: return the error during vibration, if any.
		* @returns void.
		*/
		virtual void Vibrate(EIMUError &eError) = 0;

		/** Trigger a period of haptic pulse to the glove.
		* @param eError: return the error during vibration, if any.
		* @param msDurationMillisec: the duration of pulse in millisecond
		* @returns void.
		*/
		virtual void VibratePeriod(EIMUError & eError, int msDurationMillisec) = 0;

		/** Toggle vibration status (on -> off, off -> on)
		* @param eError: return the error during toggle vibration, if any.
		* @returns void.
		*/
		virtual void ToggleVibration(EIMUError & eError) = 0;

		/** Peform T-Pose Calibration
		*
		* @param eError: return the error during calibration, if any.
		* @returns void
		*/
		virtual void TPoseCalibration(EIMUError &eError) = 0;

		/** Peform OK-Pose Calibration
		*
		* @param eError: return the error during calibration, if any.
		* @returns void
		*/
		virtual void OKPoseCalibration(EIMUError &eError) = 0;

		/** Switch to advanced mode during runtime
		*
		* @param eError: return the error during switching to advanced mode during runtime, if any.
		* @param advancedMode: advanced mode flag.
		* @returns void
		*/
		virtual void SwitchToAdvancedMode(EIMUError & eError, bool advancedMode) = 0;

		/** Switch data glove hardware version
		*	
		* @param eError: return the error during switch hardware version, if any.
		* @param version: hardware version specified
		* @returns void
		*/
		virtual void SwitchHardwareVersion(EIMUError & eError, GLOVEVERSION version) = 0;

		/** Set radio channel limit
		*
		* @param eError: return the error during set radio channel limit, if any.
		* @param upperBound: top bound of radio channel used for glove (should be less than 100 and greater than 0)
		* @param lowerBound: low bound of radio channel used for glove (should be less than 100 and greater than 0)
		* @returns void
		*/
		virtual void SetRadioChannelLimit(EIMUError & eError, int upperBound, int lowerBound) = 0;

		/** Trigger a radio channel hopping
		*
		* @param eError: return the error during channel hopping, if any.
		* @param channel: radio channel specified
		* @returns void
		*/
		virtual void ChannelHopping(EIMUError & eError, int channel) = 0;

		/** Slerp algorithm tuning
		*
		* @param eError: return the error during slerp algorithm tuning, if any.
		* @param finger: finger joint name (use intermediate joint of each finger to tune algorithm).
		* @param type: algorithm configuration type.
		* @param value: value to tune.
		* @param vec: vector to tune the offset.
		* @returns void
		*/
		virtual void AlgorithmTuning(EIMUError & eError, Joint finger, AlgorithmConfig type, double value = 0, VRTRIXVector_t vec = {0,0,0}) = 0;

		/** Load alignment parameters
		*
		* @param eError: return the error during parameter loading, if any.
		* @param param: alignment parameters specified
		* @returns void
		*/
		virtual void LoadAlignmentParam(EIMUError & eError, AlignmentParameter param) = 0;

		/** Register data call back event handler class.
		*
		* @param pEventHandler: the call back event handler class.
		* @param pUserParam: the user-defined pointer.
		* @returns void
		*/
		virtual void RegisterIMUDataCallback(IVRTRIXIMUEventHandler* &pEventHandler, void* pUserParam = NULL) = 0;

		/** Prepare for firmware update
		*
		* @param type: device type that prepare to update.
		* @param eError: return the error during firmware update, if any.
		* @returns void
		*/
		virtual void PrepareFirmwareUpdate(EIMUError &eError, DeviceType type) = 0;

		/** Start firmware update
		*
		* @param firmwarePath: return the error during firmware update, if any.
		* @param type: device type that prepare to update.
		* @param eError: return the error during firmware update, if any.
		* @returns void
		*/
		virtual void StartUpdateFirmware(EIMUError &eError, std::string firmwarePath, DeviceType type) = 0;

		/** Stop the data streaming and close the serial port 
		*
		* @param eError: return the error during closing the port, if any.
		* @returns void
		*/
		virtual void ClosePort(EIMUError &eError) = 0;		
		
		/** Perform COM Port setting (Lagacy, NOT USED NOW)
		*	Note: This function need to be called when running on a new computer
		* @param eError: return the error during opening the port, if any.
		* @param info: struct contains port information, feed in baud_rate & hand type,
		* @returns void.
		*/
		virtual void ComPortLatencySetting(PortInfo info, EIMUError &eError) = 0;

		/** Get finger bend angle according to specific joint.
		*	Note: valid joint index for this function is the intermediate joint for five fingers
		*		, which indicates five fingers gesture, any value other than that is NOT allowed
		*		and will return error.
		* @param eError: return the error calculate finger angle, if any.
		* @param finger: joint/finger index.
		* @returns the specific finger bend angle.
		*/
		virtual double GetFingerBendAngle(Joint finger, EIMUError &eError) = 0;

		/** Get finger yaw angle according to specific joint.
		* @param eError: return the error calculate finger angle, if any.
		* @param origFinger: first joint/finger index.
		* @param destFinger: second joint/finger index.
		* @returns the specific finger yaw angle between two joints specified.
		*/
		virtual double GetFingerYawAngle(Joint origFinger, Joint destFinger, EIMUError &eError) = 0;

		/** Uninit the data glove
		*
		* @returns void
		*/
		virtual void Uninit() = 0;
	};

	/** Initialization of the data glove.
	*
	* @param eError: Initialization errror return..
	* @param advancedMode: The mode of initialization, default set to false.
	*		 Note that the advanced mode only need to set true when yaw rotation need to perform
	* @returns a interfacce pointer of IMU system.
	*/
	VRTRIXIMU_INTERFACE IVRTRIXIMU* InitDataGlove(EInitError &eError, InitMode mode = InitMode_Normal, GLOVEVERSION version = DK2);

	/** Uninit VRTRIXIMU.dll. Any interface pointers from the interface are
	* invalid after this point.
	*
	* @param m_pIMU: the IMU system pointer to close.
	*/
	VRTRIXIMU_INTERFACE void UnInit(IVRTRIXIMU * &m_pIMU);

	/** Get all connected gloves PortInfo
	*
	* @param eError: return the error during identifying the port, if any.
	* @param info: return all connected gloves port info in container.
	* @returns the number of data glove ports found.
	*/
	VRTRIXIMU_INTERFACE int GetConnectedGlovePortInfo(EIMUError &eError, PortInfo info[MAX_GLOVE_SUPPORTED]);

}
