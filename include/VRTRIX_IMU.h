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

		/** Register data call back event handler class.
		*
		* @param pEventHandler: the call back event handler class.
		* @param pUserParam: the user-defined pointer.
		* @returns void
		*/
		virtual void RegisterIMUDataCallback(IVRTRIXIMUEventHandler* pEventHandler, void* pUserParam = NULL) = 0;

		/** Stop the data streaming and close the serial port 
		*
		* @returns void
		*/
		virtual void ClosePort() = 0;
	};

	/** Initialization of the data glove.
	*
	* @param eError: Initialization errror return..
	* @param advancedMode: The mode of initialization, default set to false.
	*		 Note that the advanced mode only need to set true when yaw rotation need to perform
	* @returns a interfacce pointer of IMU system.
	*/
	VRTRIXIMU_INTERFACE IVRTRIXIMU* InitDataGlove(EInitError &eError, bool advancedMode = false);

	/** Uninit VRTRIXIMU.dll. Any interface pointers from the interface are
	* invalid after this point.
	*
	* @param m_pIMU: the IMU system pointer to close.
	*/
	VRTRIXIMU_INTERFACE void UnInit(IVRTRIXIMU * m_pIMU);

}
