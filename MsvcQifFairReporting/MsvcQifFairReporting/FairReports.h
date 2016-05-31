//
// FairReports.h
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
#include <stdarg.h>  
#include <map>
#include <xercesc/dom/DOMElement.hpp>
#include <xsd/cxx/xml/string.hxx>
#include "QIFDocument.hxx"

/**
* \brief Handles the  First Article Inspection Report (FAIR) generation.
* Reports 1 and 3 are generated.
* Generates HTML output files correctly formatted.
* The class typically declares an array of the relevant FAIR information.
* Then, a QIF Results document uses a homegrown XPath equivalent to find and
* store the values required of the FAIR report.
*/
class CFairReports
{
public:
	CFairReports(void);
	~CFairReports(void);
	/**
	* \brief Handles the first AS9102 report in First Article Inspection Report (FAIR) generation.
	* \param e is the root element in the Xerces parsed QIF XML.
	*/
	std::string GenerateFAIRRepor1(xercesc::DOMElement* e );

	/**
	* \brief Handles the third AS9102 report in First Article Inspection Report (FAIR) generation.
	* \param e is the root element in the Xerces parsed QIF XML.
	*/
	std::string GenerateFAIRRepor3(xercesc::DOMElement* e );

	/**
	* \brief Result from the first AS9102 report generation.
	* \return HTML formatted string with report.
	*/
	std::string Form1();
	/**
	* \brief Result from the third AS9102 report generation.
	* \return HTML formatted string with report.
	*/
	std::string Form3();

	void CheckEntries();
	void CheckEntries3();
	
	
	// Utilities here - so as not to collide with windows and xerces

	/**
	* \brief Does a test in windows to see if Chrome exists.
	* Only google chrome accurately prints the HTML into pdf.
	* \return true if google chrome exists.
	*/
	bool ChromeExists();
	/**
	* \brief Same as printf only for strings.
	* \param format is the format string in which to format following stack items.
	* \return a formatted string according to format and arguments.
	*/
	static std::string StrFormat(const char* format, ...);
	/**
	* \brief Saves the given report in the file system .
	* \param filename is a string containing filename including path.
	* \param a string (report string) to save to OS.
	*/
	void SaveReport(std::string filename, std::string &report);
	/**
	* \brief Return the folder location of the exe application.
	* \return a  string containing full folder path of exe.
	*/
	//static std::string ExeDirectory();

	////////////////////////////////////////////////////////////////////////
	// Part 1 fields
	std::vector<std::string> part_names;
	std::vector<std::string> part_numbers;
	std::vector<std::string> serial_numbers;
	std::vector<std::string> FAI_report_numbers;
	std::vector<std::string> part_revisions;
	std::vector<std::string> drawing_numbers;
	std::vector<std::string> drawing_revisions;
	std::vector<std::string> additional_changes;
	std::vector<std::string> organization_names;
	std::vector<std::string> supplier_codes;
	std::vector<std::string> po_numbers;
	std::vector<std::string> detail_fai;
	std::vector<std::string>  full_fai;
	
	// Form 3 items - some overlap with Form1
	std::vector<std::string> prepared_by;
	std::vector<std::string> date;

	std::vector<std::string> requirement;
	std::vector<std::string> results;
	std::vector<std::string> non_conformance_number;

	struct CharacteristicInfo
	{
		CharacteristicInfo() {}

		std::string Name;
		int id;
		std::string characteristic_designator;
		std::string requirement;
		std::string results;
		//int designed_toolingId;
		//std::string designed_tooling;

		std::string print_drawing_name;
		std::string sheet_number;
		std::string drawing_zone;
		int  designed_toolingId;

		std::vector<int>  measurement_ids;
		std::vector<std::string>  measurement_names;

		std::string optional;

	};
	std::vector<CharacteristicInfo> char_info;

	struct CharacteristicDefinitionInfo
	{
		CharacteristicDefinitionInfo() {}
		int id;
		int DeviceDatumRefId;
		std::string requirements;
		void * pCharacteristicDefinitionBaseType ;
	};
	std::map<int, CharacteristicDefinitionInfo> chardef_info;

	struct CharacteristicNominalInfo
	{
		CharacteristicNominalInfo() {}
		int id;
		int CharacteristicDefinitionId;
		std::string requirements;
		//xsd::qif2::CharacteristicDefinitionBaseType* pBaseCharacteristicDefinitionType;
		void* pBaseCharacteristicDefinitionType;
	};
	std::vector<CharacteristicNominalInfo> charnom_info;

	struct DatumReferenceFrameInfo
	{
		DatumReferenceFrameInfo() {}
		int id;
		std::string requirements;
	};
	std::map<int, DatumReferenceFrameInfo>  datum_info;

protected:
	std::vector<int> LinesCount();
	std::string Form3TopOfPage(int page, int total_pages);
	std::string Form3Header();
	std::string Form3Trailer();
};

