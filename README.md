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