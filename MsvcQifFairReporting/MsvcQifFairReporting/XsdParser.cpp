//
//  XsdParser.cpp
//

#include "XsdParser.h"
#include "XercesUtils.h"
#include <iostream>
#include <fstream>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/framework/psvi/XSValue.hpp>
#include <xercesc/framework/psvi/XSAttributeDeclaration.hpp>
#include <xercesc/framework/psvi/XSAttributeUse.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSFacet.hpp>
#include <xercesc/framework/psvi/XSModelGroupDefinition.hpp>

#define CLEANFETCH(Y,X,Z) try{ Y=X; } catch(...) { Y=Z; }
static std::string GetText(const XMLCh* xstr)
{
	if (xstr==NULL) return "";
	return	StrX(xstr).localForm();
}
static std::string ToStr( const XMLCh* toTranscode ) 
{  
	return XMLString::transcode(toTranscode); 
}
static XMLCh *ToXMLCh ( std::string str)
{
	return  XMLString::transcode(str.c_str());
}

void CXsdParser::XsdParse(std::string inxsdfilename )
{
	xsdFile             = inxsdfilename.c_str();
	try
	{
		grammarPool = new XMLGrammarPoolImpl(XMLPlatformUtils::fgMemoryManager);

		parser = XMLReaderFactory::createXMLReader(XMLPlatformUtils::fgMemoryManager, grammarPool);
		parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
		parser->setFeature(XMLUni::fgXercesSchema, true);
		parser->setFeature(XMLUni::fgXercesHandleMultipleImports, true);
		parser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);
		parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, false);
		parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
		parser->setFeature(XMLUni::fgXercesDynamic, true);
		parser->setProperty(XMLUni::fgXercesScannerName, (void *)XMLUni::fgSGXMLScanner);

		XPrintHandler handler;
		parser->setErrorHandler(&handler);
		//xsdFile="D:\\michalos\\My Work\\QIF\\xsd\\MSVC QIF_2.0_XMLSchemaFiles\\QIFApplications\\QIFDocument.xsd";
		parser->loadGrammar(xsdFile, Grammar::SchemaGrammarType, true);

		if (handler.getSawErrors())
		{
			handler.resetErrors();
		}
		else
		{
			parsedOneSchemaOkay = true;
		}
		if (parsedOneSchemaOkay)
		{
			bool updatedXSModel;
			xsModel = grammarPool->getXSModel(updatedXSModel);
		}
	}
	catch(...)
	{

	}
}

bool CXsdParser::SaveParseTree(std::string inxsdfilename, std::string outfilename )
{
	xsdFile             = inxsdfilename.c_str();
	try
	{
		grammarPool = new XMLGrammarPoolImpl(XMLPlatformUtils::fgMemoryManager);

		parser = XMLReaderFactory::createXMLReader(XMLPlatformUtils::fgMemoryManager, grammarPool);
		parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
		parser->setFeature(XMLUni::fgXercesSchema, true);
		parser->setFeature(XMLUni::fgXercesHandleMultipleImports, true);
		parser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);
		parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, false);
		parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
		parser->setFeature(XMLUni::fgXercesDynamic, true);
		parser->setProperty(XMLUni::fgXercesScannerName, (void *)XMLUni::fgSGXMLScanner);

		XPrintHandler handler;
		parser->setErrorHandler(&handler);
		//xsdFile="D:\\michalos\\My Work\\QIF\\xsd\\MSVC QIF_2.0_XMLSchemaFiles\\QIFApplications\\QIFDocument.xsd";
		parser->loadGrammar(xsdFile, Grammar::SchemaGrammarType, true);

		if (handler.getSawErrors())
		{
			handler.resetErrors();
		}
		else
		{
			parsedOneSchemaOkay = true;
		}
	}
	catch(...)
	{

	}
	return parsedOneSchemaOkay;
}
