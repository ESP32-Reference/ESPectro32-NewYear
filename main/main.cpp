
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"
#include "Arduino.h"

const static char *TAG ="APP";
const static char *DEFAULT_WIFI_SSID = "[YOUR_SSID_NAME]";
const static char *DEFAULT_WIFI_PASSWORD = "[YOUR_SSID_PASS]";
const static char *FILENAME = "/WAV/DOWNLOAD.WAV";
const static char *SONG_URL = "http://andriyadi.me/wp-content/uploads/2017/12/We_Wish_You_A_Merry_Christmas_The_Chipmunks_.wav"; //"http://music.albertarose.org/christmas/lyrics/music/AlvinAndTheChipmunks_WeWishYouAMerryChristmas.wav"

#define HTTP_DOWNLOAD_DEBUG_PRINT(...)  ESP_LOGI("HTTP", __VA_ARGS__);

//Song playing icon to display on LED Matrix
static const uint8_t PROGMEM LED_PLAYING_SONG[] =
{ B00011110,
  B00110010,
  B00100010,
  B00100110,
  B01101110,
  B11101100,
  B11000000
};

#include "ESPectro32_Board.h"
#include "WiFiManager.h"
#include "ESPectro32_LedMatrix_Animation.h"
#include "AudioPlayer.h"

extern "C" {
	#include "esp_request.h"
	void app_main(void);
}

WiFiManager wifiMgr;

File downloadedFile;
int download_callback(request_t *req, char *data, int len) {

	req_list_t *found = req->response->header;
	static int binary_len = -1, remain_len = -1;

	static fs::FS fs = SD;

	// HTTP_DOWNLOAD_DEBUG_PRINT("Response code: %d", req->response->status_code);

	// while(found->next != NULL) {
	// 	found = found->next;
	// 	HTTP_DOWNLOAD_DEBUG_PRINT("Response header %s:%s", (char*)found->key, (char*)found->value);
	// }

	HTTP_DOWNLOAD_DEBUG_PRINT("Downloading...%d/%d bytes, remain=%d bytes", len, binary_len, remain_len);
	if(req->response->status_code == 200) {
		//First time
		if(binary_len == -1) {
			found = req_list_get_key(req->response->header, "Content-Length");
			if(found) {
				HTTP_DOWNLOAD_DEBUG_PRINT("Binary len=%s", (char*)found->value);
				binary_len = atoi((const char*)found->value);
				remain_len = binary_len;
			} else {
				HTTP_DOWNLOAD_DEBUG_PRINT("Error get content length");
				return -1;
			}

			//Begin download
			downloadedFile = fs.open(FILENAME, FILE_WRITE);
			if (!downloadedFile) {
				HTTP_DOWNLOAD_DEBUG_PRINT("Failed to open file for writing");
				return -1;
			}
		}

		//Downloading...
		size_t written = downloadedFile.write((uint8_t *)data, len);
		if (written == 0) {
			HTTP_DOWNLOAD_DEBUG_PRINT("No data written to File");
			downloadedFile.close();
			return -1;
		}

		remain_len -= len;

		//Download is finished
		if(remain_len == 0) {
			HTTP_DOWNLOAD_DEBUG_PRINT("Download callback is finished");
			downloadedFile.close(); //Close the file
		}

		//Return success
		return 0;
	}

	//Return failed
	return -1;
}

void app_main(void)
{
	initArduino();
	Serial.begin(115200);

	ESP_LOGD(TAG, "It begins!");
	ESPectro32.begin();

	//Explicitely begin SDCard operation
	ESPectro32.SDCardBegin();
	ESPectro32.SDCardPrintInfo();
	ESPectro32.SDCardListDirectory();

	//Preparing WiFiManager
	ESPectro32_LedMatrix_Animation ledMatrixAnim = ESPectro32_LedMatrix_Animation::startWiFiAnimation();
	wifiMgr.begin(WIFI_MODE_STA, false);
	wifiMgr.connectToAP(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);

	//Actually start WiFiManager
	wifiMgr.start();

	//This will block until connected or timeout
	if (wifiMgr.waitForConnection()) {
		ledMatrixAnim.stop();
		ESPectro32.LedMatrix().displayFrame(2);
	}
	else {
		//If not succeed or timeout, just return
		return;
	}

	ESPectro32_LedMatrix_Animation downloadAnim = ESPectro32_LedMatrix_Animation::startDownloadAnimation();

	//Preparing HTTP client
	request_t *req;
	int status;
	req = req_new(SONG_URL);
	
	req_setopt(req, REQ_SET_HEADER, "User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.84 Safari/537.36");
	req_setopt(req, REQ_FUNC_DOWNLOAD_CB, download_callback);
	status = req_perform(req);
	req_clean(req);

	//If file downloading is success
	if (status) {

		downloadAnim.stop();
		downloadAnim.clearFrames();

		ESPectro32.LedMatrix().drawBitmapFull(LED_PLAYING_SONG);

		HTTP_DOWNLOAD_DEBUG_PRINT("File is downloaded");

		ESPectro32_LedMatrix_ScrollTextAnimation *ledMatrixTextAnim = new ESPectro32_LedMatrix_ScrollTextAnimation();
		ledMatrixTextAnim->setLedMatrix(ESPectro32.LedMatrix());

		//Preparing AudioPlayer a.k.a WAVE Player

		//Adjust the I2S pins assigment
		i2s_pin_config_t pin_config;
		pin_config.bck_io_num = GPIO_NUM_26;
		pin_config.ws_io_num = GPIO_NUM_25;
		pin_config.data_out_num = GPIO_NUM_32;
		pin_config.data_in_num = I2S_PIN_NO_CHANGE;
		
		AudioPlayer *player = new AudioPlayer();
		player->onStopped([ledMatrixTextAnim]() {
			ESP_LOGI(TAG, "Audio Player stopped");
			ledMatrixTextAnim->scrollText("Happy New Year 2018!", 8000);
		});
		player->begin(SD, &pin_config);

		player->playAsync(FILENAME);
	}
}


