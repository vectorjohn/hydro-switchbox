Arduino Switchbox
=================

This is a small thing I'm working on for an Arduino controlled switchbox. By switchbox I mean it controls a relay connected to main power with power outlets, but I guess the code doesn't know that.

Mostly it just has schedule functionality now, and lacks things like a **real time clock**. It thinks the time is whatever time it was when you programmed it.

TODO
----
* Use EEPROM to store the current time to improve power loss until I have an RTC. Assume when power comes back the time is whatever was in EEPROM.
* when starting up, currently it looks for whatever event last fired and starts it. What it actually needs to do is set the state of all the pins.
* I haven't tested the timer overflow handling. The number of milliseconds since startup will overflow if it runs for a few months but it shouldn't be too hard to detect.
