## Preface
I'm new at this

# RomRaider Pi
The purpose of this project is to use a raspberry pi to create an integrated data logger that would power on and off with the car, display live data and run software that is configured to read Subaru specific PIDs (like fbkc or iam). 

## The Pi
### Pi OS
For the least headache it is strongly recommended that the operating system used is 32 bit, not 64bit. The recommended OS is the latest version of Raspberry Pi OS (32 bit), with desktop enviornment. It can be found here: https://www.raspberrypi.com/software/

Once done setting up the OS, open the terminal and enter `sudo apt update`, then `sudo apt upgrade`. This ensures that the pi is up to date.
### config.txt changes
After installing the OS, I recommend that the config.txt changes be made first. That way if something goes wrong with the config changes you wont have lost much work after reinstalling the OS.
The config.txt file address is `/boot/firmware/config.txt`. The following should be added to the end of the file:
- `dtoverlay=gpio-shutdown`
- `dtoverlay=disable-bt` Do not add this line if you would like to use the bluetooth feature of the pi, this disables it.
- `disable_splash=1`
- `boot_delay=0`

This would also be a good time to go to this file `/etc/systemd/login.conf`, find `HandlePowerKey=ignore` and change it to `HandlePowerKey=shutdown`. Make sure this line is not also commented out.
### java and drivers
To install the necessary jdk for RomRaider to function, using the terminal enter `sudo apt install default-jdk`. Since you are using a 32bit OS, the jdk will be native 32bit as well.
NOTE: this jdk has worked for me, but it is technically unsupported by romraider. However, the jdks that are, are ancient and annoying to get installed.

If you plan to the tactrix cable to connect to the obd2 port, you will also need to install the drivers to communicate with the tactrix cable. That driver, and installation instructions can be found here: https://github.com/dschultzca/j2534. There is one modification to the instructions; when told to download `libusb-1.0-0-devel`, instead use `libusb-1.0-0-dev`.
### RomRaider
To install romraider, download the linux .zip from https://www.romraider.com/index.php/RomRaider/Download and extract the files. Then, from the terminal, use the `cd <directory path>` command to move to the romraider directory. Use `ls` to verify the .jar files are present in the directory. Run `java -jar RomRaider.jar`. This should launch romraider editor, and create a run.sh file in the romraider directory. RomRaider can now be closed. The run.sh file can (and should) be edited to change which version of romraider (editor, logger, logger fullscreen, logger touchscreen) is launched when the program starts. run the run.sh file to verify that is can launch RR correctly.
#### RR autostart
Now that rr runs, it needs to do the starting bit on its own. To do this we need to go to the `~/.config/autostart/` directory. If its not there, you simply need to create autostart directory in the `~/.config/` directory. 
Once we are in the `~/.config/autostart/` directory, we need to make a <filename>.desktop file. This can be done in the terminal by simply running `sudo vim <filename>.desktop`, openning the vim editor.
The following must be added to the .desktop file:

`[Desktop Entry]`

`Type=Application`

`Name=RomRaider`

`Exec=/home/pi/RomRaider/run.sh`

`Icon=/home/pi/RomRaider/src/main/resources/graphics/romraider-ico-large.gif`

This file is an app launcher, and can be copied to your desktop as well. 

Once this is complete, romraider should automatically start when the pi turns on.
## The arduino/external hardware
### arduino to pi interface

### arduino code

## CAD/3d printed parts
I uploaded the fusion assembly file .f3d to the repository. Fusion is free, so hopefully anyone can
use the file without issue.
I printed the large housing component in 2 halves (the housing is made of 2 bodies in the assembly)
out of asa. asa is supposed to be very uv and heat resistant, as well as strong and less warp prone 
than abs. Even with asa and a prusa mk3 printer, you absolutely should take printing slow and use an 
enclosure. This thing wants to warp. Then glue the 2 halves together with acetone and you're done 
with that part.
## Arduino
The arduino is powered straight from const 12v using the vin and ground pins on the headers, however is also perfectly reasonable to 
run that 12v into the arduino via the dc barrel jack. That is all that is needed to power the arduino.

Overall the arduino setup is pretty simple, doesnt require much other than a straight line between its pins and whatever its connected to.
Only circuit additions are a voltage divider using a 10kohm resistor between the output leg of the ldr and the analog input pin. The ldr is fed by 5v instead of 3.3v
The optocoupler no and nc pins must be connected to 3.3v and ground on the arduino to draw the com pin high and low without the need of pull
resistors. Thats it.
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
