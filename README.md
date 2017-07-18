## Problems with the first circuit

The Mark I programmer had many flaws, but one of them was debilitating
enough to deserve a respin: I had to remove the programmer from the
Arduino every edit-compile-test cycle. In between flashing the
ArduinoISP software each time and taking the ATTiny in and out of the
socket, the whole process was too painful and made me resistant to
playing around.

The fundamental reason for this was that, because the pin headers that
fit into the Arduino's female headers had to point down, the socket
for the ATTiny was also mounted upside-down. This made it inaccessible
once the shield was attached.

## The Mark II programmer

What I needed was some kind of double-sided perfboard, or some way to
lengthen the pins and turn them back around. I did neither and instead
made my first big soldering mistake. I decided to try and just solder
the pins on the wrong side of the perfboard.

This ... didn't work that well. The joints between the pins and wires
were fine, but the joints to the board were loose. Even worse, in this
configuration I was often relying on solder to mechanically hold
everything together. I managed to make it to the end and even added a
convenient switch to turn the capacitor on and off (explained below),
but it was a dog's breakfast indeed. I immediately started planning my
next version.

Soldering to the wrong side of the board was so messy that I realized
I was doing something dumb, something wrong. This wasn't a matter of
lacking dexterity. I needed a smarter way.

## A $2 USB-to-UART board

Using my Arduino Uno for programming ATTinys was really painful,
because I was working on a project where two microcontrollers were
communicating with each other. So I was constantly flashing the
Arduino with ArduinoISP, programming the ATTiny with that, and then
flashing it with my code so I could test the communication between the
two chips. Yikes.

What I needed was a permanent solution: a male USB connector that I
could solder onto my perfboard, connect some IC to it and then connect
the microcontroller (an ATMega328, same as what's on an Arduino) to
that. Searching for this led me to the amazing CP2102, a tiny
USB-to-serial board that is currently sold for $1.99 on
taydaelectronics.com:
http://www.taydaelectronics.com/bte13-007-cp2102-serial-converter-usb-2-0-to-ttl-uart-ftdi.html

Now, $2 is the kind of price I'm looking for. That's not a product,
that's a component. I don't feel like I'm just paying for someone
else's solution, I'm buying a building-block component. But can it be
used to program ATTiny's? That's the wrong question. The right
question is, Can a noob like me make it program ATtiny's?

For $2 I was willing to just take a shot. Actually I found an even
cheaper option, a tiny board with FT232RL chips from FTDI going for
$1.79 at taydaelectronics
http://www.taydaelectronics.com/usb-ttl-ft232rl-html.html. I found
this so amazing that I bought 8 of them. That's about how much 1 of
this board https://www.sparkfun.com/products/12731 from Sparkfun
costs, ostensibly with the same chip. Cheap clones are cheap! Having a
bunch of these USB chips will allow me to fearlessly solder in USB
connectivity to my projects.

So can I do it? Can I use this cheap board to program ATTinys? From
reading 
http://arduino.stackexchange.com/questions/3361/i-bought-usb-ft232rl-can-i-use-it-to-program-a-bootloader-onto-an-arduino
this stackexchange page, it appears that it cannot be used alone. The
reason is that USB is an inherently serial protocol, and so is UART.
This is a USB to UART converter, a serial to serial converter. But the
ISP protocol used to program AVR chips is a parallel protocol. A
microcontroller is certainly capable of talking a serial protocol on
one end (certain pins) and a parallel protocol on another (other
pins).

Thinking about this makes me understand the bootloader a bit more. You
can program an AVR chip with a serial connection if the chip has a
bootloader on it. I guess the bootloader talks a serial protocol and
then directly writes the program data it receives onto flash memory.
This bypasses Atmel's ISP protocol for programming the chip.

(I'm guessing the bootloader is also responsible for Arduino's easy
`Serial.print` communication.)`

It seems like we will end up with a stripped-down Arduino board. One
thing that I don't understand about the actual Arduino is whether
there is a tiny microcontroller on the board in addition to the big
one in the socket. I do see a tiny chip on there that I don't know the
purpose of. I should learn more about exactly what's on an Arduino
board.

Well of course I am traveling a well-trodden road, and I will cheat a
little and watch this video
https://www.youtube.com/watch?v=sNIMCdVOHOM for tips. One tip I will
have to follow up with later is this `alice`, a seller of electronics
on Ebay who I've heard of before on some forum or another. There is
also a very helpful official guide here
https://www.arduino.cc/en/Main/Standalone. And here's a pretty nice
long forum discussion about exactly the kind of boards I'd purchased:
https://forum.arduino.cc/index.php?topic=138611.0 .

## First attempt

I hooked up an ATmega328 on the breadboard with the FTDI USB breakout
and tried loading `Blink` on it. This didn't work. Many things could
be wrong: the microcontroller could be dead, it might not have the
bootloader on it (even though it was sold as having the bootloader),
the bootloader might be corrupted, the FTDI breakout board could be
bad, my USB cable could be bad, the Arduino IDE could be calling
`avrdude` with bad parameters for this FTDI board. And so on.

The fact that my Arduino Uno has a removable atmega328 will make
debugging this a lot easier. I flashed `Blink` on that one and then
transferred it to the breadboard. I removed everything but `V_cc` and
`GND` coming from the USB board, and connected the cable. It blinked!

This eliminates the following possibilities: crystal is bad, no power
coming from USB board, power from USB is too noisy, crystal capacitors
are bad, blinking LED is bad. That's not much, in fact I knew all of
those except the crystal and crystal caps just from multimeter testing
earlier.

## Reset pulse

I can get the upload to work by taking the wire that goes to RESET on
the atmega and holding it to ground, then letting it go at the right
moment. I can also get upload to work by unplugging and replugging the
USB cable in time with pressing the upload command. So something is
wrong with the capacitor I have inline with RESET and DTR.

But something could be wrong with the USB breakout board, or with my
circuit. I'm glad I had the foresight to buy two different kinds of
USB breakout board, even though I got eight of the cheap(er) ones and
only one of the more expensive ($1.99 vs. $1.79) boards. I hooked up
the other board and tried. Same problem! Same timing finickiness with
the RESET and DTR lines.

Something is probably wrong with my circuit, then. Makes sense! I'm
the weakest link in the chain, an electronics noob who's too impatient
to read documentation until a problem occurs. 

Let's read the docs and think about this for a second. RESET is
supposed to be an active low pin, which means it needs to be held up
usually. 
		

Okay, I have figured it out. On the FTDI board, the DTR pin is being
held LOW all the time. Instead, it needs to be held HIGH and then made
LOW during communication. Combined with our resistor and capacitor
assembly, this will pulse the RESET pin of the chip and run the
bootloader.

This is what the $2 board is doing. Hmm, now is there maybe some other
pin on the FTDI board that is held HIGH and then made low during
transmission?

Another possibility is that DTR is being held LOW and then made HIGH
during transmission.

Aha, it seems like both RTS and DTR start out HIGH, then go LOW for
the first upload, but then stay LOW! So any subsequent tranmissions
fail.

I tried using avrdude instead of the Arduino IDE. I tried playing with
avrdude settings to see if I could get it to force DTR back to HIGH
after the transmission was over. But no luck.

This really seems like a driver issue. Ultimately the driver controls
whether DTR is HIGH or LOW during transmission and when idle. Since
I'm a spoilt Mac user, I never restart my computer even after
installing drivers or other OS-level software. But it's possible this
niche driver is not as friendly as other OS X software. A restart
could help...

... and it did! Now I have a fully functional breadboarded Arduino
that I can solder up with an 8-pin socket to create my ATtiny
programmer!

And the first damn thing I'm going to have to do after flashing
ArduinoISP is permanently disabling RESET on the main chip. Hmm ... I
guess I could have foregone that whole debugging exercise. But fully
understanding the bug feels pretty good, and the whole thing
solidified my knowledge of something as basic as a capacitor. A
capacitor turns a transition into a pulse! That's something I kinda
knew before, but now it feels real.

To elaborate on that: prviously, if you had asked me "Does a series
capacitor turn a transition into a pulse?" I would have thought about
it for a bit and then probably (hopefully) said Yes. But if I was in
the situation of needing a pulse, I would not have thought "Hmm, I
know I can get a pulse from a transition by just adding a series
capacitor, so is the reduced problem of needing a transition any
easier?" That's the difference, and it's important because it's such a
general thing that it's sure to come up again.

So debugging this pointless issue was actually well worth my time.

## Bootloader burning

https://wolfpaulus.com/tag/arduino/page/2/

It seems crazy that the bootloader is 32670 bytes! Of course avrdude
reports an error at byte 0x7e00.

Had to set fuses. Learned about that from here:
https://wolfpaulus.com/tag/arduino/page/2/

Had to edit boards.txt to turn 0x05 to 0xFD. This is because only the
lowest 3 bits of efuse are writable. The rest stay at one.

Had to use menu option "Upload using programmer."

After burning bootloader, do NOT "upload using programmer" another
sketch. Directly take the newly bootloader-burned atmega and stick it
in the Arduino for testing. This finally worked. Now I can repeatably
burn the bootloader on fresh ATmega328p chips.

But I don't want to permanently solder in my 16MHz crystal for my
ATtiny programmer. For that, I will need to set the correct fuses  ...
Looks like I shouldn't do this because I need accurate timing for
serial USB communication. Ok.

## The programmer on a breadboard

This was a matter of connecting up http://highlowtech.org/?p=1706 and
the Arduino pin mapping.

One final problem, which was that I had to tell the Arduino 1Mhz
internal clock rather than 8Mhz. I suppose it's "burn bootloader"'s
job to set fuses that tell the ATtiny which clock to use. Let's try that.

## Detailed steps to getting blink working on an ATTiny85

This is using the Arduino as ISP with my ATtiny shield v2.

1. Plug shield in. On one side pins go into Reset thru GND. On the
other side pins go from 9 thru GND.

2. Switch should be in position AWAY from capacitor.

3. Use Arduino IDE to upload the ArduinoISP sketch, selecting Board =
Arduino/Genuino Uno and Programmer = AVR ISP.

4. Move switch to other position (TOWARDS capacitor).

5. Run
	 avrdude -p attiny85 -c arduino -P /dev/cu.usbmodem1411 -b 19200 -U flash:w:blink-attiny85.hex

6. Pull ATtiny85 out and put it on the breadboard. On the shield, GND
and 5v are exposed. The pin closer to the capacitor is GND.

7. Led +ve side (long side, with resistor) goes in Pin 2, Led -ve side
(short side) goes on Pin 4 which is GND. 5v goes to Pin 8.