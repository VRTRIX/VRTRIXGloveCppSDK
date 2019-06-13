// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VRTRIXIMU_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VRTRIXIMU_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef VRTRIXIMU_EXPORTS
#define VRTRIXIMU_INTERFACE extern "C" __declspec( dllexport )
#else
#define VRTRIXIMU_INTERFACE extern "C" __declspec( dllimport )
#endif

#include "IVRTRIXIMUEventHandler.h"

namespace VRTRIX
{
	class IVRTRIXIMU
	{
	public:
		virtual ~IVRTRIXIMU(){};

		/** Open the data streaming serial port according to the PortInfo
		*
		* @param eError: return the error during opening the port, if any.
		* @param info: struct contains port information, feed in baud_rate & hand type, 
		*			   returns other parameters of the port.
		* @returns void.
		*/
		virtual void OpenPort(EIMUError &eError, PortInfo &info) = 0;

		/** Start the data streaming serial port according to the PortInfo
		*	Note that this function contains a infinite loop inside, the loop only breaks when 
		*	error occurs during data streaming.
		*
		* @param eError: return the error during data streaming, if any.
		* @param info: struct contains port information.
		* @returns void.
		*/
		virtual void StartDataStreaming(EIMUError &eError, PortInfo info) = 0;

		/** Perform hardware calibrate process.
		*	Note: All data gloves have performed IN-FACTORY hardware calibration, no need to do it again  
	    *   unless the environment magnetic field has been changed dramatically. 
		* @param eError: return the error during calibration, if any.
		* @returns void.
		*/
		virtual void HardwareCalibrate(EIMUError &peError) = 0;

		/** Perform hardware pairing
		*	Note: All data gloves have performed IN-FACTORY pairing, no need to do it again
		*   unless the pairing is lost due to unexpected reason.
		* @param channel: channel to pair.
		* @param info: serial port information.
		* @param eError: return the error during pairing, if any.
		* @returns void.
		*/
		virtual void RequestToPair(int channel, PortInfo info, EIMUError &peError) = 0;		

		/** Reset hardware to rssi noise scan mode
		* @param eError: return the error during pairing, if any.
		* @returns void.
		*/
		virtual void RequestToRSSIScan(EIMUError &peError) = 0;		
		
		/** Perform COM Port setting
		*	Note: This function need to be called when running on a new computer
		* @param eError: return the error during opening the port, if any.
		* @param info: struct contains port information, feed in baud_rate & hand type,
		* @returns void.
		*/
		virtual void ComPortLatencySetting(PortInfo info, EIMUError &peError) = 0;

		/** Get finger angle according to specific joint.
		*	Note: valid joint index for this function is from 0-4, which indicates five fingers gesture,
		*		  any value other than that is NOT allowed and will return error.
		* @param eError: return the error calculate finger angle, if any.
		* @param finger: joint/finger index.
		* @returns the specific finger bend angle.
		*/
		virtual double GetFingerAngle(Joint finger, EIMUError &peError) = 0;

		/** Trigger a haptic pulse to the glove.
		* @param eError: return the error during vibration, if any.
		* @returns void.
		*/
		virtual void Vibrate(EIMUError &peError) = 0;

		/** Trigger a period of haptic pulse to the glove.
		* @param eError: return the error during vibration, if any.
		* @param msDurationMillisec: the duration of pulse in millisecond
		* @returns void.
		*/
		virtual void VibratePeriod(EIMUError & peError, int msDurationMillisec) = 0;

		/** Align the fingers' orientations to hand.
		*
		* @param eError: return the error during software alignment, if any.
		* @returns void
		*/
		virtual void SoftwareAlign(EIMUError &peError) = 0;

		/** Switch to advanced mode during runtime
		*
		* @param eError: return the error during switching to advanced mode during runtime, if any.
		* @param advancedMode: advanced mode flag.
		* @returns void
		*/
		virtual void SwitchToAdvancedMode(EIMUError & peError, bool advancedMode) = 0;

		/** Register data call back event handler class.
		*
		* @param pEventHandler: the call back event handler class.
		* @param pUserParam: the user-defined pointer.
		* @returns void
		*/
		virtual void RegisterIMUDataCallback(IVRTRIXIMUEventHandler* &pEventHandler, void* pUserParam = NULL) = 0;

		/** Stop the data streaming and close the serial port 
		*
		* @param eError: return the error during closing the port, if any.
		* @returns void
		*/
		virtual void ClosePort(EIMUError &eError) = 0;		
		
		/** Config the HTC Tracker with specified handtype
		*
		* @param eError: return the error during configuration, if any.
		* @param type: specify left hand / right hand.
		* @returns void
		*/
		virtual void ConfigHTCTracker(EConfigError &eError, HandType type) = 0;
		
		/** Handle HTC tracker events, should be called in loop;
		*
		* @param eError: return the error during events handling, if any.
		* @returns void
		*/
		virtual void HandleHTCTrackerEvents(EConfigError &eError) = 0;	

		/** Identify HTC tracker for left hand & right hand
		*
		* @param eError: return the error during identify tracker, if any.
		* @param type: specify left hand / right hand.
		* @returns void
		*/
		virtual void IdentifyHTCTracker(EConfigError &eError, HandType type) = 0;

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
	VRTRIXIMU_INTERFACE IVRTRIXIMU* InitDataGlove(EInitError &eError, InitMode mode = InitMode_Normal);

	/** Uninit VRTRIXIMU.dll. Any interface pointers from the interface are
	* invalid after this point.
	*
	* @param m_pIMU: the IMU system pointer to close.
	*/
	VRTRIXIMU_INTERFACE void UnInit(IVRTRIXIMU * &m_pIMU);

}
