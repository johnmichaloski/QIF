
Boost command line program options
======================================

1) Include file and namespace abbreviation
#include <boost/program_options.hpp>
using namespace  boost::program_options;


2) Declare option variables with defaults:
	std::string shdrfilename=::ExeDirectory() + "M25751-12-03-09.txt";
	std::string devicefilename=::ExeDirectory() + "Devices.xml";


3) Setup the options to accept:e:
	options_description desc("Allowed options");
	desc.add_options()
		("-p", value<int>(), "IP Port")
		("-d", value<std::string>(&devicefilename), "Devices xml file")
		("-s", value<std::string>(&shdrfilename), "Shdr Script file")
		;


	if(__argc > 1)
	{
		try {
			AtlTrace(StrFormat("ShdrPlayback %s\n", lpstrCmdLine).c_str());
			variables_map vm;
			std::vector<std::string> args = split_winmain(lpstrCmdLine);
			store(command_line_parser(args).options(desc).run(), vm);
			if (vm.count("-p")) { ipport= vm["-p"].as<int>();}
			if (vm.count("-d")) { devicefilename= vm["-d"].as<std::string>();}
			if (vm.count("-s")) { shdrfilename= vm["-s"].as<std::string>();}

		}
		catch(std::exception e)
		{
			AtlTrace(e.what());
		}
		AtlTrace(StrFormat("ShdrPlayback Add Echo %s %s %d %d \n", 
			devicefilename.c_str(), shdrfilename.c_str(), ipport, dOverride).c_str());
		CShdrEchoInstance * shdr = shdrs.AddShdrEcho(devicefilename, shdrfilename, ipport, dOverride);
		shdr->SetFault(mtbfstat,mttrstat);
	}


static std::string GetStrDateTimestamp(xsd::cxx::tree::optional<xsd::qif2::NotedEventType::TimeOccurred_type> & ts)
{
	std::stringstream s;
	ts.get();
	s << ts.get().year() << "/" << ts.get().month() << "/" << ts.get().day() << "T" << ts.get().hours()  << ":" << ts.get().minutes() << ":" << ts.get().seconds()<< "Z";
	return s.str();
}

template <typename T>
inline T& 
	CreateNode (DOMDocument* doc, const std::string& name,  T& x)
{
	xsd::cxx::xml::dom::auto_ptr<xercesc::DOMElement> e (
		doc->createElement(X(name.c_str())));
	*e << x;
	return x;
}

	

void ExampleSerializeQIF()
{
	//v->ThisInstanceQPId() =xml_schema::string("1");
	DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(X("Core"));

	DOMDocument* doc = impl->createDocument(
		0,                    // root element namespace URI.
		X("QIFDocument"),         // root element name
		0);                   // document type object (DTD).


	DOMElement* rootElem = doc->getDocumentElement();
	QIFDocumentType qif(*rootElem);

	qif.Version(::xsd::qif2::VersionType()) ; 
	qif.Version()->ThisInstanceQPId(QPIdType("1.0")); 

	qif.Header(QIFDocumentHeaderType());
	qif.Header()->Scope("Results");
	qif.Header()->Description()="Hello World";

	qif.Header()->Author(AuthorType());
	qif.Header()->Author()->Name("John Michaloski"); // CreateNode(doc, "Name", AuthorType::Name_type("John Michaloski")));
	qif.Header()->Author()->Organization("NIST");

	//qif.Header()->Author()->Name()=::xml_schema::string("John Michaloski");; // CreateNode(doc, "Name", AuthorType::Name_type("John Michaloski")));
	//qif.Header()->Author()->Organization()=::xml_schema::string("NIST");; // CreateNode(doc, "Name", AuthorType::Name_type("John Michaloski")));

	//qif.Header()->Author()->Name(*(doc->createElement(X("Name")))); // CreateNode(doc, "Name", AuthorType::Name_type("John Michaloski")));
	//AuthorType::Name_type & name = qif.Header ()->Author()->Name().get();
	//name=::xml_schema::string("John L Michaloski");

	//qif.MeasurementsResults(::xsd::qif2::MeasurementsResultsType(*(doc->createElement(X("MeasurementsResults")))));
	qif.MeasurementsResults(::xsd::qif2::MeasurementsResultsType());
	::xsd::qif2::MeasurementsResultsType & results = qif.MeasurementsResults().get();
	results.Version( ::xsd::qif2::VersionType(*(doc->createElement(X("Version")))));
	results.Version()->ThisInstanceQPId(CreateNode(doc, "ThisInstanceQPId", QPIdType("234")));

	MeasurementResultsType &m = MeasurementResultsType(
		(const MeasurementResultsType::InspectionStatus_type&)MeasurementResultsType::InspectionStatus_type(),
		(const MeasurementResultsType::id_type&)58);
	m.ThisResultsInstanceQPId()="8521ff0f-4c05-4f13-a2be-1386190f75a6";

	ExternalFileReferenceType &file=ExternalFileReferenceType("C:\\R19_parts\\5802804.res",
		FileSpecType(),
		"Brown/Sharpe CMM Results");
	file.FileSpec().OtherFileSpec("res");
	m.ExternalFileReference ().push_back(file);
	// Code synthesis copies the node, so you had better make all additions before pushing

	m.MeasuredFeatures(MeasuredFeaturesType(FeatureActualsType()));

	//  <EdgePointFeatureActual id="10">
	//    <FeatureItemId>9</FeatureItemId>
	//    <Location>2460.72 770.62 944.98</Location>
	//    <Normal>-0.735465884156759 -0.307902932144912 0.603560864882807</Normal>
	//  </EdgePointFeatureActual>
	EdgePointFeatureActualType &edge = EdgePointFeatureActualType(10,9);

	std::vector<double> pts; pts.push_back(2460.72); pts.push_back(770.62); pts.push_back(944.98);
	edge.Location() = ActualPointType(ListDoubleType(pts.begin(), pts.end()));

	std::vector<double> norm;	norm.push_back(-0.735465884156759);norm.push_back(-0.307902932144912); norm.push_back(0.603560864882807);
	edge.Normal() = ActualUnitVectorType(ListDoubleType(norm.begin(), norm.end()));

	m.MeasuredFeatures()->FeatureActuals().FeatureActual().push_back((EdgePointFeatureActualType&) edge);

	//  <PointFeatureActual id="20">
	//    <FeatureItemId>19</FeatureItemId>
	//    <Location>2466.9 774.31 944.84</Location>
	//  </PointFeatureActual>
	PointFeatureActualType &point = PointFeatureActualType(20,19);
	pts.clear(); pts.push_back(2466.9); pts.push_back(774.31); pts.push_back(944.84);
	point.Location() = ActualPointType(ListDoubleType(pts.begin(), pts.end()));
	m.MeasuredFeatures()->FeatureActuals().FeatureActual().push_back((PointFeatureActualType&) point);


	//  <PointFeatureActual id="36">
	//    <FeatureItemId>35</FeatureItemId>
	//    <Location>2537.17 783.38 920.02</Location>
	//  </PointFeatureActual>
	PointFeatureActualType &point2 = PointFeatureActualType(36,35);
	pts.clear(); pts.push_back(2537.17); pts.push_back(783.38); pts.push_back( 920.02);
	point2.Location() = ActualPointType(ListDoubleType(pts.begin(), pts.end()));
	m.MeasuredFeatures()->FeatureActuals().FeatureActual().push_back((PointFeatureActualType&) point2);


	//  <CircleFeatureActual id="44">
	//    <FeatureItemId>43</FeatureItemId>
	//    <Location>2434.01 801.52505599193 889.98</Location>
	//    <Normal>0.0558150216639657 -0.907624351305543 -0.416056150385791</Normal>
	//    <Diameter>9.499476</Diameter>
	//  </CircleFeatureActual>
	CircleFeatureActualType &circle = CircleFeatureActualType(44,43);
	pts.clear(); pts.push_back(2434.01); pts.push_back(801.52505599193); pts.push_back( 889.98);
	circle.Location() = ActualPointType(ListDoubleType(pts.begin(), pts.end()));
	norm.clear();	norm.push_back(0.0558150216639657); norm.push_back(-0.907624351305543); norm.push_back(-0.416056150385791);
	circle.Normal() = ActualUnitVectorType(ListDoubleType(norm.begin(), norm.end()));
	m.MeasuredFeatures()->FeatureActuals().FeatureActual().push_back((CircleFeatureActualType&) circle);


	results.MeasurementResults().push_back(m);

	//qif.Header ()->Author()->Name()=xml_schema::string("John Michaloski");
	//qif.Header ()->Author()->Organization()="NIST";
	//qif.Header ()->Description() ="Sample QMR file";
	//qif.Header ()->Scope() ="Results";

	//qif.MeasurementsResults()->InspectionTraceability()->ReportPreparer()->Name()="John Michaloski";
	//qif.MeasurementsResults()->InspectionTraceability()->ReportPreparationDate()=xml_schema::date_time (1809, 3, 31,12,24,34);
	/**
	<QIFDocument xmlns="http://qifstandards.org/xsd/qif2" idMax="86" versionQIF="2.0.0" 
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xsi:schemaLocation="http://qifstandards.org/xsd/qif2 ../QIFApplications/QIFDocument.xsd">
	*/
	xml_schema::namespace_infomap map;

	map[""].name = "http://qifstandards.org/xsd/qif2";
	map[""].schema = "http://qifstandards.org/xsd/qif2 ../QIFApplications/QIFDocument.xsd";

	QIFDocument (std::cout,
		qif,
		map,
		"UTF-8",
		xml_schema::flags::dont_initialize);
}