// IOSBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "IOSBridge.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/screenshotr.h>

IOS_BRIDGE_API int takeScreenshot()
{
	idevice_t device = NULL;
	lockdownd_client_t lckd = NULL;
	lockdownd_error_t ldret = LOCKDOWN_E_UNKNOWN_ERROR;
	screenshotr_client_t shotr = NULL;
	lockdownd_service_descriptor_t service = NULL;
	int result = -1;
	int i;
	const char *udid = NULL;
	char *filename = NULL;

	if (IDEVICE_E_SUCCESS != idevice_new(&device, udid)) {
		if (udid) {
			printf("No device found with udid %s, is it plugged in?\n", udid);
		}
		else {
			printf("No device found, is it plugged in?\n");
		}
		return -1;
	}

	if (LOCKDOWN_E_SUCCESS != (ldret = lockdownd_client_new_with_handshake(device, &lckd, NULL))) {
		idevice_free(device);
		printf("ERROR: Could not connect to lockdownd, error code %d\n", ldret);
		return -1;
	}

	lockdownd_start_service(lckd, "com.apple.mobile.screenshotr", &service);
	lockdownd_client_free(lckd);
	if (service && service->port > 0) {
		if (screenshotr_client_new(device, service, &shotr) != SCREENSHOTR_E_SUCCESS) {
			printf("Could not connect to screenshotr!\n");
		}
		else {
			char *imgdata = NULL;
			uint64_t imgsize = 0;
			if (!filename) {
				time_t now = time(NULL);
				filename = (char*)malloc(36);
				strftime(filename, 36, "screenshot-%Y-%m-%d-%H-%M-%S.tiff", gmtime(&now));
			}
			if (screenshotr_take_screenshot(shotr, &imgdata, &imgsize) == SCREENSHOTR_E_SUCCESS) {
				FILE *f = fopen(filename, "wb");
				if (f) {
					if (fwrite(imgdata, 1, (size_t)imgsize, f) == (size_t)imgsize) {
						printf("Screenshot saved to %s\n", filename);
						result = 0;
					}
					else {
						printf("Could not save screenshot to file %s!\n", filename);
					}
					fclose(f);
				}
				else {
					printf("Could not open %s for writing: %s\n", filename, strerror(errno));
				}
			}
			else {
				printf("Could not get screenshot!\n");
			}
			screenshotr_client_free(shotr);
		}
	}
	else {
		printf("Could not start screenshotr service! Remember that you have to mount the Developer disk image on your device if you want to use the screenshotr service.\n");
	}

	if (service)
		lockdownd_service_descriptor_free(service);

	idevice_free(device);
	free(filename);
}


