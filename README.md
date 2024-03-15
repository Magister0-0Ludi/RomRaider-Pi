# TO - DO
Finalize some method of controlling power on off, both automatically for a safe shutdown of the
pi and for autostarting correctly when appropriate. This is currently being handled via arduino uno
that monitors an optocoupler for ignition on/off. The arduino sends a wake signal to the pi, getting
startup. Currently always on... might need to change that. Otherwise the arduino code at the very 
least needs to be finished. The screen brightness autocontrol works, as does manual pi on off signal.
currently there are issues with the optocoupler, it looks to possibly be a code issue - need to check that.

otherwise I really need to post the files for the enclosure to be printed/improved, as well as the arduino
code/interface design. Also need to post more detailed stuff for everything lol
# RomRaider-Pi
Place for me to store stuff for the romraider pi project

This project is a lot of firsts for me in terms of, well, most of it. I'm learning and honestly pretty
bad at this. BUT I think this is a really cool idea and I want people to be able to make more of it and
help me turn it into something that people might actually use, or at something that is really cool.
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
## Pi config
### Autostart
starting romraider on powerup is handled with a .desktop file in an autostart folder.
Can't remember specifics, need to go back and figure that stuff out.

To autostart RomRaider, we start at getting to the autostart folder. I created one at 

`~/.config/autostart/`

but recommendations seem to be to use/make the autostart folder at 

`~/.config/lxsession/LXDE-<username, or pi if left default>/autostart/`

use whichever works for you. In the autostart folder, we need to make a desktop entry.
create a file called <filename>.desktop. In that file, write 

`[DESKTOP ENTRY]
Type=Application
Name=RomRaider
Exec=<path to romraider run.sh script>`

to make sure that RomRaider launches with access to all its libraries and such, it needs 
to be launched for its home dir. So we must go edit the run.sh script. Once opened, add

`cd <path to RomRaider directory>`

at the beginning of the script.

Then you're down, RomRaier should autostart after the desktop enviorment loads on the pi.
### config.txt editing
editing the config.txt file. its located at 

`/boot/firmware/config.txt`

you'll likely do a lot of editing in this file it you don't already/raspberry pi is new to you.
for you expirenced guys, you already know.
Right now, the only thing I can think of is adding the following to the end of the file (after "[all]"):
- `dtoverlay=gpio-shutdown`
- `dtoverlay=disable-bt`
- `disable_splash=1`

To set up the pi to have discrete on and off pins, use:

`dtoverlay=gpio-shutdown,gpio_pin=4,active_low=1,gpio_pull=up,debounce=50`

this should make pin 3 only wake the pi with its hardware pullup resistor, and pin 4 (the pin directly below pin 3) the shutdown pin only.
### fast boot
fast boot specific options
### other changes
fixing shutdown key, verifying com ports, os setup?
to make the pi shut down with a single "button press" you have to change the shutdown key behavior. To do this, go to 

`/etc/systemd/login.conf`

and find 

`HandlePowerKey=ignore`

and change it to 

`HandlePowerKey=shutdown`.

Make sure that the line is not commented out as well. This will make wake and halt a single input operation.
### hats used/recommended? 
The hats I'm using now is a usb hub hat an an rs232 converter hat.
Hope is with the rs232 hat I'll be able to log my wideband, but until the jserialcomm issue is dealt
with, that isn't happening.
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
