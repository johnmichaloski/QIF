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
There are two major setup requirements: 1) additional include directories and 2) link library files.

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