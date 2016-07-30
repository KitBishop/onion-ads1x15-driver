# onion-ads1x15-driver
Driver for ADS1x15 ADC chips that run on an I2C interface.<br>
<br>
```
Usage:
	./ads1x15-chip [options] <channel> [max-voltage]

<channel>	Analog input channel, can be 0 to 3
[max-voltage]	Optional: Set maximum expected input voltage
	If used over-rides -g option setting.
	Enter as a real number for maximum voltage.
		maximum usable is:6.144
		minimum usable is:0.256

[options] - any number of:
	-q	Quiet: no output
	-v	Verbose: debug output
	-x	Extra Verbose: all debug output
	-g<gain>    Gain: gain to use. Affects maximum voltage measurable
	    0 - max voltage 6.144V - the default
	    1 - max voltage 4.096V
	    2 - max voltage 2.048V
	    3 - max voltage 1.024V
	    4 - max voltage 0.512V
	    5 - max voltage 0.256V
	-t<type>    Type: the device type
	    1015 - 12 bit A/D - the default. Maximum raw data value 2047
	    1115 - 16 bit A/D. Maximum raw data value 32767<br>
	-o<output>  Output: form of result output<br>
	    s - standard output giving channel number and raw data - the default<br>
	    r - raw data read<br>
	    m - data read in millivolts<br>
	    v - data read in volts<br>
	-d	Debug: do not carry out any I2C transactions<br>
```
