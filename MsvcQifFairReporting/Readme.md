MsvcQifFairReporting 
===================

Below are some "brief" background setup information.  There is MS Word documentation that more completely describing installation on Windows. 

Note, these directions are for Windows systems. It was easier to compile/link on a Linux (Ubuntu 12.4) system.  The code has been ported to 
Linux but this code is not exactly that - there are stdafx.h include files, although precompiled headers is turned off in the project.


Prerequisites
------------------
We assume that Boost, CodeSynthesis and Xerces are installed. 
CodeSynthesis is used to parse the XML into C++ structures. 
CodeSynthesis  uses Xerces and some of the FAIR report generation navigates the parsed Xerces XML tree for branch matches.

The code generation for MsvcQifFairReporting project is multi-threaded, statically linked.

Project Management
------------------
There are three major setup requirements: 1) additional include directories and 2) link library files,
3) 

1) In the MsvcQifFairReporting project, the Microsoft Visual C/C++ Command  Line option has 

	@IncludeDir.txt /bigobj 

added customize the location of header files. 
The file .\MsvcQifFairReporting\IncludeDir.txt contains a list of directories that are used for searching for 
C++ include header files. A search/replace of the project folder should manage local folder header searches.
Boost is part of later MSVC versions so locating the header and building the link files may not be necessary.
CodeSynthesis is fairly well-contained, and once installed locating the include directory is all that is necessary.

Include folders are:

	-I"C:\Program Files\NIST\src\boost_1_54_0"
	-I.
	-I"C:\Users\michalos\Documents\GitHub\QIF\src\MsvcQifFairReporting"
	-I"C:\Users\michalos\Documents\GitHub\QIF\src\MsvcQifFairReporting\QIFApplications"
	-I"C:\Users\michalos\Documents\GitHub\QIF\src\MsvcQifFairReporting\QIFLibrary"
	-I"C:\Users\michalos\Documents\GitHub\QIF\src\MsvcQifFairReporting\MsvcQifFairReporting"
	-I"C:\Users\michalos\Documents\GitHub\QIF\src\MsvcQifFairReporting\NIST"
	-I"C:\Program Files (x86)\CodeSynthesis XSD 4.0\include"

I realize CMake could make this "easier", but easier is in the eye of the beholder.

2) Static linking is used whenever and wherever possible. For boost this means including the #define:

	#define BOOST_ALL_NO_LIB

which generates minimizes DLL's. Boost understands that the code generation for MsvcQifFairReporting project is 

All the MsvcQifFairReporting  exe program needs to link against is the xerces-c_3 static library. 
Depending on the build (Debug vs Release) and/or (64bit versus 32bit) you will require different static
libraries. In the file MsvcQifFairReporting.cpp there is some linking directives
 
	#if defined(WIN64) && defined( _DEBUG) 
	#pragma message( "DEBUG x64" )
	#pragma comment(lib, "C:\\Program Files (x86)\\CodeSynthesis XSD 4.0\\lib64\\vc-10.0\\xerces-c_3D.lib")
	#elif !defined( _DEBUG) && defined(WIN64)
	#pragma message( "RELEASE x64" )
	#elif defined(_DEBUG) && defined(WIN32)
	#pragma comment(lib, "C:\\Program Files (x86)\\CodeSynthesis XSD 4.0\\lib\\vc-10.0\\xerces-c_3D.lib")
	#elif !defined( _DEBUG) && defined(WIN32)
	#pragma message( "RELEASE x32" )
	#pragma comment(lib, "C:\\Program Files (x86)\\CodeSynthesis XSD 4.0\\lib\\vc-10.0\\xerces-c_3.lib")
	#endif 


3) You will need to add the CodeSynthesis DLL to the path or place in the execution folder. I put
xerces-c_3_1D_vc100 in the Win32/Debug folder, since I am using MSVC 2010.


Running  MsvcQifFairReporting 
----------------------------
MsvcQifFairReporting  is a Windows Shell application and you can get "help" by typing 

	.\MsvcQifFairReporting.exe -h

in a command shell in the execution folder. It will return:

	  -q arg              QIF XML file
	  -r1 arg             FAIR Report 1 file
	  -r3 arg             FAIR Report 3 file
	  -v arg              QIF Version
	  -h                  Help
	  /?                  Help

There is also a visual basic script (Vbs) file name FairReport.vbs that pop up a dialog to ask for 
the qif XML file, and then run MsvcQifFairReporting.exe program. FAIR reports 1 & 3 will be put into the
same folder as the QIF  XML file.

Another way to generate reports with the program is to run in a DOS command window. If you bring up a DOS command window (Run->Start->Cmd) and you change to the Exe folder you can run the command with:

	.\MsvcQifFairReporting.exe -v 1.2 -q "C:\Users\michalos\Documents\GitHub\QIF\src\MsvcQifFairReporting\MsvcQifFairReporting\x64\Debug\QIF_Results_Sample.xml"

In this case, Report1.html and Report3.html will be generated in the same folder as the QIF XML exe file. For example:

	"C:\Users\michalos\Documents\GitHub\QIF\src\MsvcQifFairReporting\MsvcQifFairReporting\x64\Debug\Report1.html"


	
