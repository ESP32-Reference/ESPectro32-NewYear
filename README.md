ESPectro32-NewYear
==================

A super simple project targeted for ESPectro32 IoT Dev Board to download and play Merry Xmas and New Year (or basically any) song, plus LED Matrix animations. The project is based on [Espressif IoT Development Framework](https://github.com/espressif/esp-idf) (ESP-IDF). 

Although it's a simple project, but you'll learn quite a lot:

* Connecting to WiFi
* Download file over HTTP and save it to microSD card
* Reading file content from microSD card
* Play WAV audio file using I2S codec
* Display icon, animation, and text on ESPectro32's on-board LED matrix
* Explore ESPectro32 dev board in general

## ESPectro32

ESPectro32 is an IoT development board based-on ESP32 MCU, made by DycodeX. You can get one from [Makestro Shop](https://shop.makestro.com/product/espectro32/) or [Cytron](https://www.cytron.io/p-espectro32)

In order to play audio, you need an I2S codec hardware and a speaker. I use special version of ESPectro32 - [ESPectro32 Pro](https://shop.makestro.com/product/espectro32-pro/) - that has an I2S codec soldered. But you can also use ESPectro32's compatible audio hat or buy this [bundle](https://shop.makestro.com/product/bundle-holiday-gift-box/). If you're OK with a bit of hassle, you can use any I2S codec and adjust the I2S pins assignment in the code. 

## Getting Started

* Clone this repo
* Update submodules which this project depends on: `git submodule update --init --recursive`
* There's a symbolic link arduino-esp32. It's a Arduino-compatible framework for ESP32. You can clone it from [here](https://github.com/espressif/arduino-esp32). Change (recreate) the symbolic link to point to your local arduino-esp32 folder.
* In the [main.cpp](https://github.com/dycodex/ESPectro32-NewYear/blob/master/main/main.cpp) file, change some constants to fit your need. 
* Via Terminal/Console, on current directory, try to type `make`. Fingers crossed :)
