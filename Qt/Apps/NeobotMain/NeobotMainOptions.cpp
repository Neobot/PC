#include "NeobotMainOptions.h"
#include <iostream>
#include <cstdlib> //for atoi

using namespace std;

const int NB_REQUIRED_ARGUMENTS = 0;
const int NB_MAX_MANDATORY_ARGUMENTS = 0;

NeobotMainOptions::NeobotMainOptions()
{
    portname = "/dev/ttyUSB0";
    ax12Portname = "/dev/ttyUSB1";

    simulation = false;
	dstar = false;

    logFile = false;
    debugLevel = 17;

    record = false;

    autoQuit = false;

    reversed = false;

    strategy = 0;
}

void NeobotMainOptions::usage() const
{
	cout << "The main program of the robot Neobot.\n\n"
		 << "Usage:\n"
		 << "Neobot [-h] [--portname portname] [--ax12Portname portname] [--dstar] [--simulation] [--debug-level x] [--file filename] [--auto-quit] [--gui] [--reversed]\n"
		 << "\nOptions:\n"
		 << "\"-h\" or \"--help\": Displays this help.\n"
		 << "\"-p\" or \"--portname\": Use the specified portname for the RS232 connection. Default value is \"/dev/ttyUSB0\""
		<< "\"-ap\" or \"--ax12-portname\": Use the specified portname for the AX-12 RS232 connection. Default value is \"/dev/ttyUSB1\""
		 << "\"-s\" or \"--simulation\": Simulate the communicaion with the microC.\n."
		 << "\"--dstar\": Use the dstar algorithm instead of micropather (slower).\n"
		 << "\"-d\" or \"--debug-level\": Specify the level of debugging. Add the values to display multiple things. Default value is 17.\n"
		 << "     * 0: no message are displayed."
		 << "     * 1: intermediate points debugging. Display a message when the robot is arrived at an intermediate point.\n"
		 << "     * 2: position debugging. Display all positions of the robot and the opponent received from the microC.\n"
		 << "     * 4: trajectories debugging. Display the trajectories calculated by the strategy.\n"
		 << "     * 8: captors debugging. Display the values of the captors (sharps, contactors, beacons, ...).\n"
		 << "     * 16: protocol errors. Display the errors of communications. \n"
		 << "     * 32: protocol messages. Display the received and sent messages.\n"
		 << "     * 64: prococol frames. Displays all received bytes.\n"
		 << "     * 127: debug all.\n"
		 //<< "     * 128: robot debugging. Decoding errors.\n"
		 << "\"-f\" or \"--file\": Write the log in stdout and in the specified \"filename\". Require a normal or auto quit (not ctrl+c).\n"
		 << "\"-a\" or \"--auto-quit\": Quit the program at the end of the strategy.\n"
		 << "\"-m\" or \"--mirror\": Reverse the strategy.\n"
                 << "\"-r\" or \"--record\": Record match data in a file.\n"
		 << "\"-t\" or \"--strategy\":Select a strategy. Default value is 0.\n"
		 << "\n\n";

	cout << "Error codes:\n"
		<< "1: Command line error.\n"
		<< "2: Cannot open the device.\n"
		<< "3: Invalid strategy.\n"
		<< "\n\n";

        cout << "Strategies:\n"
	     << "0: Treasure Island STrategy\n"
	     << "1: Test strategy\n";
}


bool NeobotMainOptions::parse(int argc, char* argv[])
{
	if (argc < NB_REQUIRED_ARGUMENTS + 1)
	{
		cerr << "ERROR: NeobotMain: incorrect number ("
			 << argc - 1 << ") of required (" << NB_REQUIRED_ARGUMENTS << ") arguments.\n\n\n";
		usage();
		return false;
	}

	int nbMandatoryParamRead = 0;
	for(int i = 1; i < argc; ++i)
	{
		string arg = argv[i];
		if (arg[0] == '-')
		{
			// Options

			if (arg.compare("-h") == 0 || arg.compare("--help") == 0)
			{
				usage();
				return false;
			}

			else if (arg.compare("--dstar") == 0)
				dstar = true;

			else if (arg.compare("-a") == 0 || arg.compare("--auto-quit") == 0)
			    autoQuit = true;

			else if (arg.compare("-m") == 0 || arg.compare("--mirror") == 0)
			    reversed = true;

			else if (arg.compare("-s") == 0 || arg.compare("--simulation") == 0)
			    simulation = true;

                        else if (arg.compare("-r") == 0 || arg.compare("--record") == 0)
                            record = true;

			else if (i < argc - 1)
			{
				// There is at least one other argument
				// Parameters options...

				if (arg.compare("-d") == 0 || arg.compare("--debug-level") == 0)
					debugLevel = atoi(argv[++i]);
				else if (arg.compare("-f") == 0 || arg.compare("--file") == 0)
				{
				    logFile = true;
				    filename = argv[++i];
				}
				else if (arg.compare("-p") == 0 || arg.compare("--portname") == 0)
				    portname = argv[++i];
				else if (arg.compare("-ap") == 0 || arg.compare("--ax12-portname") == 0)
				    ax12Portname = argv[++i];
                                else if (arg.compare("-t") == 0 || arg.compare("--strategy") == 0)
                                    strategy = atoi(argv[++i]);
				else
				{
				    cerr << "ERROR: NeobotMain: unknow option \"" << arg << "\"";
				    usage();
				    return false;
				}
			}
			else
			{
			    cerr << "ERROR: NeobotMain: unknow option \"" << arg << "\"";
			    usage();
			    return false;
			}
		}
		else
		{
		    //not an option -> probably a mandatory parameter
		    if (nbMandatoryParamRead >= NB_MAX_MANDATORY_ARGUMENTS)
		    {
			cerr << "ERROR: NeobotMain: unknow parameter \"" << arg << "\"";
			usage();
			return false;
		    }

		    ++nbMandatoryParamRead;

		    //Read the mandatory parameters here
		    //...
		}
	}

	return true;
}
