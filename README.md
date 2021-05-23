# Not So Simple Watch
## Project Description
Main goal of this project is a merge of the TTGO Twatch Library SimpleWatch and Dan Geiger's TWatch project (see link below). Later one has cool features like the Jupiter moon calender, easy UI to set the time and a Star Trek LCARS main screen. On the other side, there is no power management which means either completely powering off with a 6 sec press or having the battery sucked empty within an hour. The SimpleWatch has power management and with it, the watch can sustain up to two days before recharging. On the down side it is only a demo app without much functionality and just a bunch of non-implemented menu entries.

## Setup
The Arduino IDE really stinks and is only a tool for beginners, therefore I chose Visual Studio Code and PlatformIO plugin to create builds and uploading them into the watch. Steps:
1. Install Visual Studio Code
2. In Code search in Extensions for PlatformIO and install it
3. Clone this Github project
4. The PlatformIO extension allows building and uploading
