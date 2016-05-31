
//
// ProgramOptions.h
//

/*
 * DISCLAIMER:
 * This software was produced by the National Institute of Standards
 * and Technology (NIST), an agency of the U.S. government, and by statute is
 * not subject to copyright in the United States.  Recipients of this software
 * assume all responsibility associated with its operation, modification,
 * maintenance, and subsequent redistribution.
 *
 * See NIST Administration Manual 4.09.07 b and Appendix I.
 */

#pragma once

#ifndef BOOST_ALL_NO_LIB
#define BOOST_ALL_NO_LIB
#endif

#include <string>
#include <vector>
#include <boost/program_options.hpp>
//using namespace  boost::program_options;
namespace po = boost::program_options;

namespace Globals
{
 std::string ExeDirectory;
};

class FairOptions
{
public:

	std::string qiffilename;
	std::string report1filename ;
	std::string report3filename ;
	double version;
	po::options_description desc; // ("Allowed MsvcQifFairReporting options");

	FairOptions() 
	{
		//desc=new po::options_description("Allowed MsvcQifFairReporting options");
		qiffilename=  Globals::ExeDirectory + "QIF_Results_Sample.xml";
		report1filename = Globals::ExeDirectory + "FairReport1.html";
		report3filename = Globals::ExeDirectory + "FairReport3.html";
	}

	bool CommandLineOptions(int argc, char* argv[])
	{
		//http://www.radmangames.com/programming/how-to-use-boost-program_options
		//options_description desc("Allowed MsvcQifFairReporting options");
		desc.add_options()
			("-q", po::value<std::string>(&qiffilename), "QIF XML file")
			("-r1", po::value<std::string>(&report1filename), "FAIR Report 1 file")
			("-r3", po::value<std::string>(&report3filename), "FAIR Report 3 file")
			("-v", po::value<double>(), "QIF Version")
			("-h", "Help")
			("/?", "Help")
			;

		if(argc > 1)
		{
			try {
				//AtlTrace(StrFormat("ShdrPlayback %s\n", lpstrCmdLine).c_str());
				po::variables_map vm;
				po::store(parse_command_line(argc, argv, desc),  vm);


				if (vm.count("-h") || vm.count("/?")) {
					cout << desc << "\n";
					throw std::runtime_error("Help request");;
				}

				//std::vector<std::string> args = split_winmain(lpstrCmdLine);
				//store(command_line_parser(args).options(desc).run(), vm);
				if (vm.count("-v")) { version= vm["-v"].as<double>();}
				if (vm.count("-q")) { qiffilename= vm["-q"].as<std::string>();}
				if (vm.count("-r1")) { report1filename= vm["-r1"].as<std::string>();}
				if (vm.count("-r3")) { report3filename= vm["-r3"].as<std::string>();}
			}
			catch(std::exception e)
			{
				//AtlTrace(e.what());
			}
		}
		return argc > 1;
	}
};