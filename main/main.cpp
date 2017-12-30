/*
 * 1. Open up the project properties
 * 2. Visit C/C++ General > Preprocessor Include Paths, Macros, etc
 * 3. Select the Providers tab
 * 4. Check the box for "CDT GCC Built-in Compiler Settings"
 * 5. Set the compiler spec command to "xtensa-esp32-elf-gcc ${FLAGS} -E -P -v -dD "${INPUTS}""
 * 6. Rebuild the index
*/

#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"
#include <Arduino.h>

const static char *TAG ="APP";
const static char *DEFAULT_WIFI_SSID = "DyWare-AP3";
const static char *DEFAULT_WIFI_PASSWORD = "p@ssw0rd";

#include <ESPectro32_Board.h>

#include <WiFiManager.h>
WiFiManager wifiMgr;

#include <ESPectro32_LedMatrix_Animation.h>

extern "C" {
	#include <esp_request.h>
}

void app_main(void)
{
	initArduino();
	Serial.begin(115200);

	ESP_LOGD(TAG, "It begins!");

	ESPectro32_LedMatrix_Animation ledMatrixAnim = ESPectro32_LedMatrix_Animation::startWiFiAnimation();
	wifiMgr.begin(WIFI_MODE_STA, false);
	wifiMgr.connectToAP(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);

	//Actually start
	wifiMgr.start();

	if (wifiMgr.waitForConnection()) {
		ledMatrixAnim.stop();
		ESPectro32.LedMatrix().displayFrame(2);
	}
	
}

extern "C" {
	void app_main(void);
}
