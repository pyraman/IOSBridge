#include "com_logigear_automation_mobile_model_ScreenshotClientModel.h"
#include "System.h"
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/screenshotr.h>

#include <iostream>
using namespace std;

/*
* Class:     com_logigear_automation_mobile_model_DeviceModel
* Method:    initialize
* Signature: (Ljava/lang/String;)J
*/
JNIEXPORT jlong JNICALL Java_com_logigear_automation_mobile_model_ScreenshotClientModel_connect
(JNIEnv * env, jobject thizz, jlong nDevice)
{
	lockdownd_error_t ldret = LOCKDOWN_E_UNKNOWN_ERROR;
	lockdownd_service_descriptor_t service = 0;
	lockdownd_client_t lckd = 0;
	screenshotr_client_t client = 0;

	idevice_t device = (idevice_t)nDevice;;

	ldret = lockdownd_client_new_with_handshake(device, &lckd, NULL);

	if (LOCKDOWN_E_SUCCESS != ldret)
	{
		return ldret;
	}

	ldret = lockdownd_start_service(lckd, SCREENSHOT_SERVICE, &service);
	lockdownd_client_free(lckd);
	if (ldret != LOCKDOWN_E_SUCCESS)
	{
		return ldret;
	}

	if (service == 0 || service->port <= 0)
	{
		return LOCKDOWN_E_UNKNOWN_ERROR;
	}

	screenshotr_error_t ssRet = screenshotr_client_new(device, service, &client);
	if (ssRet != SCREENSHOTR_E_SUCCESS)
	{
		return ssRet;
	}

	long nClient =  (long)client;
	return nClient;
}

JNIEXPORT jbyteArray JNICALL Java_com_logigear_automation_mobile_model_ScreenshotClientModel_takeScreenshot
(JNIEnv * env, jobject thizz, jlong nClient)
{
	__int64 nBegin = IOSBridge::System::currentMilliseconds();
	screenshotr_client_t client = (screenshotr_client_t)nClient;
	char *imgdata = NULL;
	uint64_t imgsize = 0;

	screenshotr_error_t nRet = screenshotr_take_screenshot(client, &imgdata, &imgsize);
	if (nRet != SCREENSHOTR_E_SUCCESS)
	{
		return NULL;
	}

	jbyteArray screenshot = env->NewByteArray(imgsize);
	env->SetByteArrayRegion(screenshot, 0, imgsize, reinterpret_cast<jbyte*>(imgdata));

	delete imgdata;

	__int64 nEnd = IOSBridge::System::currentMilliseconds();
	long nTotalDuration = nEnd - nBegin;

	cout << "[NATIVE] TOTAL: " << nTotalDuration << endl;
	return screenshot;
}