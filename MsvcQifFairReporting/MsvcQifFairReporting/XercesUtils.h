//
// XercesUtils.h
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

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>

/**
* \brief Contains some XML utilities to simplify Xerces use.
* There are XPath equivalents but require  Xerces version 2.
* So, a few simple XPath like traversal utilities were packaged into a class.
*/
class CXercesUtils
{

public:
	CXercesUtils(void);
	~CXercesUtils(void);

	/**
	* \brief Will return a list of string values that match the query string.
	* \param root is the Xerces root DOM element.
	* \param querystr is a XPath like traversal string. Only no wildcards or regex stuff.
	*/
	std::vector<std::string> GetXpathResults(xercesc::DOMElement* root, std::string querystr);
	/**
	* \brief Given a Xerces DOM element will return Name of element.
	* \param e is the Xerces  DOM element.
	* \return string containing name of element (name between <>).
	*/
	std::string GetElementName(xercesc::DOMElement* e );
	/**
	* \brief Given a Xerces DOM element will return Value of element.
	* \param e is the Xerces  DOM element.
	* \return string containing value of element (value between tags, i.e., <e>value</e>).
	*/
	std::string GetElementValue(xercesc::DOMElement* e );
	std::string GetSubElementValues(xercesc::DOMElement* e , std::vector<std::string> &excludes) ;
	void NagivateParseTree(xercesc::DOMElement* p, std::string tabs="");
	xercesc::DOMElement*  FindChildElementName(xercesc::DOMElement* e , std::string name) ;

		/**
	* \brief Interface to XPath 1 in Xerces.
	* \param root is the Xerces root DOM element. (Has to be Element, not necessarily root).
	* \param querystr is a XPath query string
	* \return vector containing template type T resuls - typicall string, but could be DOM elements.
	*/
	template<typename T>
	std::vector<T *> GetXpathAsStructs(xercesc::DOMElement* root, std::string querystr)
	{
		std::vector<T *> nodes;

		xercesc::DOMDocument* doc (root->getOwnerDocument ());

		// Obtain namespace resolver.
		xsd::cxx::xml::dom::auto_ptr<xercesc::DOMXPathNSResolver> resolver (
			doc->createNSResolver (root));

		// Create XPath expression.
		xsd::cxx::xml::dom::auto_ptr<xercesc::DOMXPathExpression> expr (
			doc->createExpression (
			xsd::cxx::xml::string (querystr.c_str()).c_str (),
			resolver.get ()));

		// Execute the query.
		xsd::cxx::xml::dom::auto_ptr<xercesc::DOMXPathResult> r (
			expr->evaluate (
			root, xercesc::DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE  /*DOMXPathResult::ITERATOR_RESULT_TYPE*/, 0));

		// If no query matches, then return empty vector
		if (!r.get() ) 
			return nodes;
		// Iterate over the result.
		for (unsigned int i=0; i < r->getSnapshotLength(); i++) 
		{
			r->snapshotItem(i);
			xercesc::DOMNode* n (r->getNodeValue ());
			T * p (static_cast<T *> ( n->getUserData (xml_schema::dom::tree_node_key)));
			nodes.push_back(p);
		}
		return nodes;
	}
};

/**
* \brief Interface to Xerces string. Helpful in converting std::string to unicode string.
*/
class XStr
{
public :
    XStr(const char* const toTranscode)
    {
        // Call the private transcoding method
        fUnicodeForm = xercesc::XMLString::transcode(toTranscode);
    }

    ~XStr()
    {
        xercesc::XMLString::release(&fUnicodeForm);
    }

    const XMLCh* unicodeForm() const
    {
        return fUnicodeForm;
    }

private :
    XMLCh*   fUnicodeForm;
};

#define X(str) XStr(str).unicodeForm()
#if 1
/**
* \brief Interface to Xerces string. 
* Helpful in converting Xerces string to std string.
*/
class StrX
{
public :

	StrX(const XMLCh* const toTranscode)
	{
		// Call the private transcoding method
		fLocalForm = xercesc::XMLString::transcode(toTranscode);
	}

	~StrX()
	{
		xercesc::XMLString::release(&fLocalForm);
	}
	const char* localForm() const
	{
		return fLocalForm;
	}

private :
	char*   fLocalForm;
};
#endif