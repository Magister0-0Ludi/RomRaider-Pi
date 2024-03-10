# RomRaider-Pi
Place for me to store stuff for the romraider pi project
## RomRaider
The project is currently based on a raspberry pi running the current raspberry pi 32bit os.
For romraider to be able to run, a 32bit jdk must be installed. 
- `sudo apt install default-jdk`

Once this is done, romraider zip for linux should be downloaded and extracted. Once extracted

- `java -jar RomRaider.jar`
should be run. This should open romraider. Once this is done, the run script can be editied to
run romraider logger fullscreen. This should allow romraider to open directly to the logger.
## Tactrix
To enable romraider to use a tactrix cable allowing it to be used for this project, one must 
install an open source linux driver. These can be found here:
https://github.com/dschultzca/j2534. Special note about this, the current
instructions of what libusb to install is incorrect. Version that should be installed is 
the most recent version of this lib at the time of writing. Once this is done, make install
should be used for best results (thus far). 

This should allow romraider to open and read ecu data via a tactrix cable.
## Autostart
starting romraider on powerup is handled with a .desktop file in an autostart file.
Can't remember specifics, need to go back and figure that stuff out.
## Other
From here I just have the 3d printed housing, and qled 7in touchscreen from waveshare.
The screen is held in place (currently) with a modified version of the waveshare screenframe
cad assembly. Follow instructions for the screen on the pi. 

From here its the interface layer im working on with an arduino to control power on and off the pi,
control screen brightness and give me a manual power button (the arduino was free, so I'm using that 
for now)

future goals are figuring out a custom data handler in romraider so that I can set up custom logic
based on the ecu data (using that in conjunction with the raspberry pi enables boarderline piggyback
features. Possible options include controlling intercooler sprayers or meth injection with data on
map, iat, driving mode, etc. I would also like to give the pi the ability to read data from something
like a meth tank level sensor, and control if it can switch to an aggressive driving map if the level is 
low. Possibilities
