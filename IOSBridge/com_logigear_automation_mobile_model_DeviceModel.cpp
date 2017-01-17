#include "com_logigear_automation_mobile_model_DeviceModel.h"
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>

/*
* Class:     com_logigear_automation_mobile_model_DeviceModel
* Method:    initialize
* Signature: (Ljava/lang/String;)J
*/
JNIEXPORT jlong JNICALL Java_com_logigear_automation_mobile_model_DeviceModel_initialize
(JNIEnv * env, jobject thizz, jstring jsUdid)
{
	idevice_t device = 0;

	const char* szUdid = (env->GetStringUTFChars(jsUdid, NULL));
	idevice_error_t nRet = idevice_new(&device, szUdid);
	if (nRet != IDEVICE_E_SUCCESS)
	{
		return nRet;
	}

	return (long)device;

}