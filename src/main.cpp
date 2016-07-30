#include <main.h>
#include <ctype.h>

void usage(const char* progName) {
	printf("Usage:\n");
	printf("\t%s [options] <channel> [max-voltage]\n", progName);
	printf("\n");
	printf("<channel>\tAnalog input channel, can be 0 to 3\n");
	printf("[max-voltage]\tOptional: Set maximum expected input voltage\n");
	printf("\tIf used over-rides -g option setting.\n");
	printf("\tEnter as a real number for maximum voltage.\n");
	printf("\t\tmaximum usable is:6.144\n");
	printf("\t\tminimum usable is:0.256\n");
	printf("\n");
	printf("[options] - any number of:\n");
	printf("\t-q	Quiet: no output\n");
	printf("\t-v	Verbose: debug output\n");
	printf("\t-x	Extra Verbose: all debug output\n");
        printf("\t-g<gain>    Gain: gain to use. Affects maximum voltage measurable\n");
        printf("\t    0 - max voltage 6.144V - the default\n");
        printf("\t    1 - max voltage 4.096V\n");
        printf("\t    2 - max voltage 2.048V\n");
        printf("\t    3 - max voltage 1.024V\n");
        printf("\t    4 - max voltage 0.512V\n");
        printf("\t    5 - max voltage 0.256V\n");
        printf("\t-t<type>    Type: the device type\n");
        printf("\t    1015 - 12 bit A/D - the default. Maximum raw data value 2047\n");
        printf("\t    1115 - 16 bit A/D. Maximum raw data value 32767\n");
        printf("\t-o<output>  Output: form of result output\n");
        printf("\t    s - standard output giving channel number and raw data - the default\n");
        printf("\t    r - raw data read\n");
        printf("\t    m - data read in millivolts\n");
        printf("\t    v - data read in volts\n");
	printf("\t-d	Debug: do not carry out any I2C transactions\n");
	printf("\n");
}

/*
int parseArguments(const char* progName, int argc, char* argv[], gpioSetup *setup)
{
	// check for the correct number of arguments
	if ( argc < 2 ) 
	{
		usage(progName);
		return EXIT_FAILURE;
	}

	// get the channel number


	return EXIT_SUCCESS;
}*/

float convertRawToMilli(int ival, adsTypes_t chipType, int gain) {
    if (chipType == ADS1X15_TYPE_ADS1015) {
        ival = ival << 4;
    }

    float fval = 0.0f;
    switch (gain) {
        case 0:
            fval = (1.0f * ival) * 0.1875f;
            break;

        case 1:
            fval = (1.0f * ival) * 0.125f;
            break;

        case 2:
            fval = (1.0f * ival) * 0.0625f;
            break;

        case 3:
            fval = (1.0f * ival) * 0.03125f;
            break;

        case 4:
            fval = (1.0f * ival) * 0.015625f;
            break;

        case 5:
            fval = (1.0f * ival) * 0.0078125f;
            break;
    }
    
    return fval;
}

int main(int argc, char* argv[])
{
	int 		status;
	int 		value;

	const char 	*progname;
	int 		verbose, debug;
	int 		ch;

	int 		channel	= -1;
	int 		gain 	= 0;  // Default of 0 sets to 6.144 volt range
        adsTypes_t      chipType = ADS1X15_TYPE_ADS1015;
        outputType_t    outType = OUTPUT_STANDARD;

	// save the program name
	progname 	= argv[0];	

	// set the defaults
	verbose 	= ADS1X15_MAIN_DEFAULT_VERBOSITY;
	debug 		= ADS1X15_MAIN_DEFAULT_DEBUG;

	//// parse the option arguments
	while ((ch = getopt(argc, argv, "xvqdhg:t:o:")) != -1) {
		switch (ch) {
		case 't':
			// chip type - arg = 1015 or 1115
                        if (strcmp(optarg, "1015") == 0) {
                            chipType = ADS1X15_TYPE_ADS1015;
                        } else if (strcmp(optarg, "1115") == 0) {
                            chipType = ADS1X15_TYPE_ADS1115;
                        } else {
                            usage(progname);
                            printf ("ERROR: invalid chip type:%s\n", optarg);
                            return EXIT_FAILURE;
                        }
			break;
		case 'o':
			// output type
                        if ((strcmp(optarg, "s") ==0) || (strcmp(optarg, "S") == 0)) {
                            outType = OUTPUT_STANDARD;
                        } else if ((strcmp(optarg, "r") ==0) || (strcmp(optarg, "R") == 0)) {
                            outType = OUTPUT_RAW;
                        } else if ((strcmp(optarg, "m") ==0) || (strcmp(optarg, "M") == 0)) {
                            outType = OUTPUT_MILLIVOLT;
                        } else if ((strcmp(optarg, "v") ==0) || (strcmp(optarg, "V") == 0)) {
                            outType = OUTPUT_VOLT;
                        } else {
                            usage(progname);
                            printf ("ERROR: invalid output type:%s\n", optarg);
                            return EXIT_FAILURE;
                        }
			break;
		case 'g':
			// gain setting
                        if ((strlen(optarg) == 1) && isdigit(optarg[0])) {
                            gain = atoi(optarg);
                            if ((gain < 0) || (gain > 5)) {
                                usage(progname);
                                printf ("ERROR: invalid gain:%s\n", optarg);
                                return EXIT_FAILURE;
                            }
                        } else {
                            usage(progname);
                            printf ("ERROR: invalid gain:%s\n", optarg);
                            return EXIT_FAILURE;
                        }
			break;

                    
                case 'x':
			// verbose output, including i2c lib
			verbose = ADS1X15_MAIN_VERBOSITY_EXTRA_VERBOSE;
			break;
		case 'v':
			// verbose output
			verbose = ADS1X15_MAIN_VERBOSITY_VERBOSE;
			break;
		case 'q':
			// quiet output
			verbose = ADS1X15_MAIN_VERBOSITY_QUIET;
			break;
		case 'd':
			// debug mode
			debug 	= 1;
			break;
		default:
			usage(progname);
			return 0;
		}
	}

	// advance past the option arguments
	argc 	-= optind;
	argv	+= optind;

	
	//// parse the arguments
	// channel argument
	if (argc > 0) {
		channel 	= atoi(argv[0]);

		if (channel < 0 || channel > ADS1X15_NUM_CHANNELS) {
			usage(progname);
			printf ("ERROR: invalid number of channels!\n");
			return EXIT_FAILURE;
		}
	}
	else {
		usage(progname);
		return EXIT_FAILURE;
	}

	ads1X15*	adsObj	= new ads1X15(ADS1X15_I2C_DEVICE_ADDR, chipType);
        
	// gain argument
	if (argc > 1) {
		gain 		= adsObj->ReadMaxVoltage( atof(argv[1]) );
		printf ("> Max input voltage %0.3f V corresponds to gain setting of %d\n", atof(argv[1]), gain);

		if (gain < 0 || gain > ADS1X15_NUM_GAIN) {
			usage(progname);
			printf ("ERROR: invalid max input voltage!\n");
			return EXIT_FAILURE;
		}
        }


	//// actual program
	// setup the verbosity
	adsObj->SetVerbosity(verbose);
	adsObj->SetDebugMode(debug);

	// set the gain
        status 	= adsObj->SetGain(gain);
        if (status != EXIT_SUCCESS) {
                printf("ERROR setting gain!\n");
                return EXIT_FAILURE;
        }

	// run the ADC conversion
	status 	= adsObj->ReadAdc(channel, value);
	if (status == EXIT_SUCCESS) {
            switch (outType) {
                case OUTPUT_STANDARD:
		printf("> ADS channel%d, value is %d\n", channel, value);
                break;
                
                case OUTPUT_RAW:
		printf("%d\n", value);
                break;
                    
                case OUTPUT_MILLIVOLT:
                case OUTPUT_VOLT:
                {
                    float volt = convertRawToMilli(value, chipType, gain);
                    if (outType == OUTPUT_VOLT) {
                        volt = volt / 1000.0f;
                    }
                    printf("%f\n", volt);
                }
                break;
            }
	}


	return 0;
}