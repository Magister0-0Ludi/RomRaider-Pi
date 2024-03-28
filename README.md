# TO - DO
If you're here for instructions and more readme type material, scroll down to RomRaider Pi

## The GitHub
I need to make this a more user friendly repository. Improving the instructions here, actually uploading my scripts
getting some directories made so I can make this less of a personal dump for stuff and more of an opensource builder friendly 
repository.
## The Pi
Theres a lot of potential to dos here so I'll start simple
### Improving auto on off function
The pi uses the gpio-shutdown to control its on off. By default, it uses the same pin for on and off (or wake and halt), GPIO3
or pin 5. To improve it, the gpio-shutdown should be given a user assigned pin (like GPIO4, the pin directly beneath GPIO3), pin pull up/down
and debounce. This will allow GPIO4 to be used for shutdown only, and since GPIO3 wake up function is hardware based, the wake up will remain
GPIO3, allowing for seperate on and off signals to be sent. This will allow the pi to use i2c which is good for expandability in the future, and potentially simplifiying the logic and reducing the chance of edge cases causing accidental shutdowns up or wake up events. 
### RTC
I would like for the pi to have functional rtc, both because it replaced my clock and at some point I would like to have a clock overlay thing working so I can have an accurate clock again. Also, if I ever do figure out a good way to log my own list of external sensors (like brake pressure, suspension 
travel, gps), making sure I have accurate times on all my logs will be important for analysing both my performance and the cars performance on track.
All in an opensource platform.

Man, I have big dreams.
### Desktop/GUI
The goal for the actual user interface of this system is to build something that is centered around a touch interface and easy navigation.
With the research I've done, it seems like the most effective way to do this would be skip the traditional desktop entirely. Instead from 
the command line one can start and end desktop instances that could be as limited as being an instance of a graphical app (like romraider).
In conjunction with this a simple gui menu could likely be made from scratch with some practice. This could be the first desktop instance that is 
launched, and through a touch friendly interface it could be used to launch a new desktop instance of the app wanted, and close it as appropriate. In
theory, such a ui could also have buttons to go to a cmdline instance or exit the simplified ui and launch a full featured standard instance of the desktop.

It is feasible that hardware buttons could be added to the ui via gpio to act as hotkeys for closing an app, a home button, change desktop, etc. This 
is definitely beyond scope right now as I havent the foggiest on executing the gui changes alone. This will probably come later in this projects life.
### RomRaider
Romraider works pretty well, but does have *some* issues working on the pi platform. There are two issues that I need to address when it comes to romraider. This first is that it requires 32bit java. Problem is, easily getting that setup on a 64bit rpi os has proven to be quite a chore and difficult
to repeat (for me). So you are limited to 32bit os. Not a terrible thing, but it does bottleneck the pi to a certain degree. The other side of this issue
is getting the desired jdk set up for rr. You can get a 32bit jdk on a 32bit os, but the specific version that rr wants is still a challenge to get 
working consistently. The other problem integrating rr has faced is the nonfunctional serial port. I know that this is an issue with the way the jserialcomm library works on the pi4 (as this issue was not present in earlier versions or this that used a pi3). Beyond that I dont know what the problem 
is. But having a usable serial port for rr could be significant in the future, and right now it means I cant log my wideband o2 sensor and I really want to
log that.
## The arduino/external hardware
### arduino to pi interface
- add the seperate power off connection
- add some kind of monitoring. Best way seems to be watching the pi's 3.3v supply line, as it will drop to 0v when the pi has halted
- potentially add serial/i2c communication for data exchange or dynamic instructions to be sent.
if power monitoring can be effectively established, it would be possible for the arduino to control a relay that would cut power to the pi once fully halted. Also very much in theory, the arduino could either carry an rtc and pass that to the pi or power the pi's rtc as it has always on power. Not
really a need as power consumption from the arduino and pi is incredibly low even with both active.
### arduino code
it needs to be improved to actually have an instant on, hold for x seconds for off to the pi. possibly monitor the pi's power state. 
need to improve the auto dimmer to maybe have a full brightness setting for when the sun is really shining, and a buffer so headlights and streetlights
wont cause strobing behavior. Not a massive problem, but still annoying when your making your way through a parking lot at night.

# RomRaider-Pi
Place for me to store stuff for the romraider pi project

This project is a lot of firsts for me in terms of, well, most of it. I'm learning and honestly pretty
bad at this. BUT I think this is a really cool idea and I want people to be able to make more of it and
help me turn it into something that people might actually use, or at something that is really cool.
## RomRaider
(The project is currently based on a raspberry pi running the current raspberry pi 32bit os.
For romraider to be able to run, a 32bit jdk must be installed.
- `sudo apt install default-jdk`

Once this is done, the linux os zip file for RomRaider should be downloaded from romraider.com
and extracted to `/home/<home dir>/`. Then via the termial, move to the new RomRaider directory with 
`cd /RomRaider/`. Now RomRaider should be run with `java -jar RomRaider.jar`.

RomRaider should now open! If it does not GO BACK AND DEBUG!! or install a new os and try again. Part of the
beauty of linux/raspberry pi's is that is easy.

Now close RomRaider.

Go back to the RomRaider directory, either via file manager or cmdline with `ls`, and check for a file named `run.sh`.
That is the launcher script for RomRaider, generated by the first run. Open that file with a text editor and a the line
`cd /home/pi/RomRaider/` to the beginning of the file. This is just to make sure that the working directory for launching
RomRaider will be RomRaiders home directory.

If desired, this would also be an ideal time to download the ecu and logger definitions from the romraider website and
get them loaded into romraider editor and romraider logger.
## Tactrix
To enable romraider to use a tactrix cable allowing it to be used for this project, one must 
install an open source linux driver. These can be found here:
https://github.com/dschultzca/j2534. 

A special note: when told to download `libusb-1.0-0-devel`, instead use `libusb-1.0-0-dev`.

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

`[Desktop Entry]`

`Type=Application`

`Name=RomRaider`

`Exec=/home/pi/RomRaider/run.sh`

`Icon=/home/pi/RomRaider/src/main/resources/graphics/romraider-ico-large.gif`

This is the format for creating a gui/desktop app launcher, and can also be copied to the desktop to launch
RomRaider easily.
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
- `dtoverlay=disable-bt`
- `disable_splash=1`
- `boot_delay=0`
### other changes
fixing shutdown key, verifying com ports, os setup?
to make the pi shut down with a single "button press" you have to change the shutdown key behavior. To do this, go to 

`/etc/systemd/login.conf`

and find 

`HandlePowerKey=ignore`

and change it to 

`HandlePowerKey=shutdown`.

Make sure that the line is not commented out as well. This will mwke wake and halt a single input operation.
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
