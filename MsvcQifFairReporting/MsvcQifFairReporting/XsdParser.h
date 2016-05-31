//
// XsdParser.h
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
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
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
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

#include "XercesUtils.h"
#include "Globals.h"

using namespace xercesc;
using namespace std;
XERCES_CPP_NAMESPACE_USE

//#define StrX(x) xercesc::XMLString::transcode(x)

inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}


/**
* \brief Prints error output from Xerces XML Parser. 
* Copied from CodeSynthesis example.
*/
class XPrintHandler : public DefaultHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XPrintHandler():
    fSawErrors(false)
	{
	}
	~XPrintHandler(){}

    bool getSawErrors() const
    {
        return fSawErrors;
    }

	void warning(const SAXParseException& exc);
    void error(const SAXParseException& exc);
    void fatalError(const SAXParseException& exc);
    void resetErrors();
private:
    bool            fSawErrors;
};

// ---------------------------------------------------------------------------
//  SCMPrintHandler: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
inline void XPrintHandler::error(const SAXParseException& e)
{
    fSawErrors = true;
    XERCES_STD_QUALIFIER cerr << "\nError at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

inline void XPrintHandler::fatalError(const SAXParseException& e)
{
    fSawErrors = true;
    XERCES_STD_QUALIFIER cerr << "\nFatal Error at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

inline void XPrintHandler::warning(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nWarning at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

inline void XPrintHandler::resetErrors()
{
    fSawErrors = false;
}

/**
* \brief Front end to Xerces XML Parser. 
* Takes std string converts to Xerces string and parses into DOM document.
*/

class CXsdParser
{
public:
	CXsdParser()
	{
		Init();
	}

	void XsdParse(std::string inxsdfilename );

	void Init();
	bool SaveParseTree(std::string inxsdfilename, std::string outfilename );

	CXercesUtils utils;
	std::stringstream io;
	bool							doList	;
	bool							schemaFullChecking	;
	const char*                     xsdFile   ;
	bool							parsedOneSchemaOkay;
	XMLGrammarPool *				grammarPool ;
	SAX2XMLReader*					parser;
	XSModel *						xsModel;
};
