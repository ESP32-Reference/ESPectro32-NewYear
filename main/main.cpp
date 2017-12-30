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

extern "C" {
	void app_main(void);
}

void app_main(void)
{
	initArduino();
	Serial.begin(115200);

	ESP_LOGD(TAG, "It begins!");
}

