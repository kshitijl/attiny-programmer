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


