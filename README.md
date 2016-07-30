# onion-ads1x15-driver
Driver for ADS1x15 ADC chips that run on an I2C interface.<br>
<br>
```
Usage:<br>
	./ads1x15-chip [options] <channel> [max-voltage]<br>
<br>
<channel>	Analog input channel, can be 0 to 3<br>
[max-voltage]	Optional: Set maximum expected input voltage<br>
	If used over-rides -g option setting.<br>
	Enter as a real number for maximum voltage.<br>
		maximum usable is:6.144<br>
		minimum usable is:0.256<br>
<br>
[options] - any number of:<br>
	-q	Quiet: no output<br>
	-v	Verbose: debug output<br>
	-x	Extra Verbose: all debug output<br>
	-g<gain>    Gain: gain to use. Affects maximum voltage measurable<br>
	    0 - max voltage 6.144V - the default<br>
	    1 - max voltage 4.096V<br>
	    2 - max voltage 2.048V<br>
	    3 - max voltage 1.024V<br>
	    4 - max voltage 0.512V<br>
	    5 - max voltage 0.256V<br>
	-t<type>    Type: the device type<br>
	    1015 - 12 bit A/D - the default. Maximum raw data value 2047<br>
	    1115 - 16 bit A/D. Maximum raw data value 32767<br>
	-o<output>  Output: form of result output<br>
	    s - standard output giving channel number and raw data - the default<br>
	    r - raw data read<br>
	    m - data read in millivolts<br>
	    v - data read in volts<br>
	-d	Debug: do not carry out any I2C transactions<br>
```
