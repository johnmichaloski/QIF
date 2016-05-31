//
// MSVCQIF.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#define WIN32_LEAN_AND_MEAN

#if defined(WIN64) && defined( _DEBUG) 
#pragma message( "DEBUG x64" )
#pragma comment(lib, "C:\\Program Files\\NIST\\src\\boost_1_54_0\\vc10\\stagex64\lib\\libboost_program_options-vc100-mt-sgd-1_54.lib")
#pragma comment(lib, "C:\\Program Files (x86)\\CodeSynthesis XSD 4.0\\lib64\\vc-10.0\\xerces-c_3D.lib")

#elif !defined( _DEBUG) && defined(WIN64)
#pragma message( "RELEASE x64" )
#pragma comment(lib, "C:\\Program Files (x86)\\CodeSynthesis XSD 4.0\\lib64\\vc-10.0\\xerces-c_3.lib")
#pragma comment(lib, "C:\\Program Files\\NIST\\src\\boost_1_54_0\\vc10\\stagex64\\lib\\libboost_program_options-vc100-mt-s-1_54.lib")

#elif defined(_DEBUG) && defined(WIN32)
//#pragma message( "DEBUG x32" )
#pragma comment(lib, "C:\\Program Files (x86)\\CodeSynthesis XSD 4.0\\lib\\vc-10.0\\xerces-c_3D.lib")
#pragma comment(lib, "C:\\Program Files\\NIST\\src\\boost_1_54_0\\vc10\\stagex32\\lib\\libboost_program_options-vc100-mt-sgd-1_54.lib")

#elif !defined( _DEBUG) && defined(WIN32)
#pragma message( "RELEASE x32" )
#pragma comment(lib, "C:\\Program Files (x86)\\CodeSynthesis XSD 4.0\\lib\\vc-10.0\\xerces-c_3.lib")
#pragma comment(lib, "C:\\Program Files\\NIST\\src\\boost_1_54_0\\vc10\\stagex32\\lib\\libboost_program_options-vc100-mt-s-1_54.lib")

#endif 

#include <string>
#include <memory>   // std::auto_ptr
#include <cstring>  // std::memcpy
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <stdio.h>
#include <stdlib.h>


#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/framework/XMLGrammarPoolImpl.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/framework/psvi/XSTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSParticle.hpp>
#include <xercesc/framework/psvi/XSModelGroup.hpp>

#include <xsd/cxx/xml/string.hxx>
#include "QIFDocument.hxx"


#include "FairReports.h"
#include "XercesUtils.h"
#include "XsdParser.h"
#include "ProgramOptions.h"


using std::cerr;
using std::endl;
using std::string;
using namespace xercesc;
using namespace xsd;
using namespace qif2;

#define SAFESTORE(X,Y,Z) \
	try { X=Y; } catch(...) { X=Z;}

#define SAFESTORE(X,Y,Z) \
	try { X=Y; } catch(...) { X=Z;}

std::string XsdDateToString( const xercesc::DOMElement& e)
{
	return  xsd::cxx::xml::transcode<char> (e.getTextContent ());
}
static void SaveReport(std::string filename, const std::string &report)
{
	std::ofstream out(filename.c_str());
	out << report <<std::endl;
	out.close();
}
//extern std::string ExeDirectory();


void GenerateFairReport(CXsdParser &parser, 
	std::string infilename,
	std::string report1filename,
	std::string report3filename)
{
	CFairReports fair;
	//DOMDocument * document;
	//DOMElement * root;

	// Parse QIF XML and generate FAIR reports
	std::auto_ptr<QIFDocumentType> qif (
		QIFDocument (infilename, xml_schema::flags::dont_initialize|xml_schema::flags::dont_validate|xml_schema::flags::keep_dom)
		);
	//document = parser.getDocument();
	//root = document->getDocumentElement();
	DOMElement* e = static_cast<DOMElement*> ((*qif)._node ());

	std::string report1 = fair.GenerateFAIRRepor1(e);
	SaveReport(report1filename, report1);

	//
	// Generate FAIR Form 3
	std::string report3 = fair.GenerateFAIRRepor3(e);
	SaveReport(report3filename, report3);

}

void ParseQifXsd(CXsdParser & parser, std::string filename)
{
	std::string str;

	std::cout << "********** Parsing XML File **********" << "\n";
	parser.XsdParse(filename);
}

#ifdef GCC
#include <signal.h>
static void segfault_sigaction(int signal, siginfo_t *si, void *arg)
{
//    printf("Caught segfault at address %p\n", si->si_addr);
    sigset_t x;
    sigemptyset (&x);
    sigaddset(&x, SIGSEGV);
    sigprocmask(SIG_UNBLOCK, &x, NULL);

    throw std::exception();
}

void SetupSignalHandler() {
	struct sigaction sa;

	memset(&sa, 0, sizeof(sigaction));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = segfault_sigaction;
	sa.sa_flags = SA_SIGINFO;

	sigaction(SIGSEGV, &sa, NULL);

}
#endif

int main(int argc, char* argv[])
{
	try
	{
		std::string cmd(argv[0]);
		Globals::ExeDirectory = cmd.substr( 0, cmd.find_last_of( '\\' ) +1 );

		// Initialize Xerces 
		XMLPlatformUtils::Initialize();

		// Front end class to xerces/code synthesis parsing of QIF
		CXsdParser parser;

		// First Article Inspection report (FAIR) report generation 
		CFairReports fair;

		FairOptions options;
		options.CommandLineOptions(argc, argv);
		GenerateFairReport(parser, 
			options.qiffilename, 
			options.report1filename, 
			options.report3filename);

	}
	catch (const xml_schema::exception& e)
	{
		std::cerr << e << endl;
		return 1;
	}
	catch (...)
	{
		XERCES_STD_QUALIFIER cerr << "An error occurred prarsing/creating the document" << XERCES_STD_QUALIFIER endl;
	}	

	XMLPlatformUtils::Terminate ();
	return 0;
}

