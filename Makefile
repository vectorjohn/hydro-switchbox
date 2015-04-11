
verify:
	arduino --verify --board digistump:avr:digispark-tiny hydro_switchbox.ino

upload:
	arduino --upload --board digistump:avr:digispark-tiny hydro_switchbox.ino
