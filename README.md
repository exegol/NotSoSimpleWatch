# Not So Simple Watch
Project for Lilygo T-Watch 2020
## Project Description
Main goal of this project is a merge of the TTGO Twatch Library SimpleWatch and Dan Geiger's TWatch project (see link below). Later one has cool features like the Jupiter moon calender, easy UI to set the time and a Star Trek LCARS main screen. On the other side, there is no power management which means either completely powering off with a 6 sec press or having the battery sucked empty within an hour. The SimpleWatch has power management and with it, the watch can sustain up to two days before recharging. On the down side it is only a demo app without much functionality and just a bunch of non-implemented menu entries.

![screenshot](https://github.com/exegol/NotSoSimpleWatch/blob/main/images/main.jpg)
![screenshot](https://github.com/exegol/NotSoSimpleWatch/blob/main/images/settime.jpg)
![screenshot](https://github.com/exegol/NotSoSimpleWatch/blob/main/images/stephistory.jpg)

## Setup
The Arduino IDE really stinks and is only a tool for beginners, therefore I chose Visual Studio Code and PlatformIO plugin to create builds and uploading them into the watch. Steps:
1. Install Visual Studio Code
2. In Code search in Extensions for PlatformIO and install it
3. Clone this Github project
4. The PlatformIO extension allows building and uploading

## Links
* T-Watch 2020 hardware docs: https://t-watch-document-en.readthedocs.io/en/latest/introduction/product/2020.html
* Basis library(also containing the SimpleWatch example: https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library
* Dan Geigers T-Watch project: https://www.instructables.com/Lilygo-T-Watch-2020-Arduino-Framework/
* LVGL graphics library (used in SimpleWatch): https://lvgl.io/
* LVGL examples: https://github.com/lvgl/lv_examples
* LVGL image converter: https://lvgl.io/tools/imageconverter
* Espressif documentation on non-volatile storage(NVS): https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html
* NVS examples: https://github.com/espressif/esp-idf/tree/73db142403c6e5b763a0e1c07312200e9b622673/examples/storage/nvs_rw_value
