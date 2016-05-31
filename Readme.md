Quality Information Framework (QIF)
=================================

The Quality Information Framework (QIF) is an ANSI standard sponsored by the Dimensional Metrology Standards Consortium (DMSC) 
that defines an integrated set of Extensible Markup Language (XML) information models to enable the effective exchange of metrology 
data throughout the entire manufacturing quality measurement process – from product definition to inspection planning to execution
 to analysis and reporting. QIF handles feature-based dimensional metrology, quality measurement planning, first article 
inspection, and discrete quality measurement. QIF is gaining attention as an important quality technology.

The Extensible Markup Language (XML) is a metalanguage, that is, a language used to create other XML languages. 
XML is typically used for defining a language to exchange data across the internet. With XML, the contextual meaning 
of the data can be represented to create a more application-specific language. The XML structure is a tree of elements 
containing branches of XML elements. An XML element is delimited by a start tag (ex: <ElementName>) and an 
end tag (</ElementName >) with CDATA being the non-markup character data (not XML tag related) between the tags. 
Elements can also have attributes, which are name-value pairs inside start tag (e.g.,: < PartDefinition id="partDefinition1">). 
The structure of XML is fundamentally tree oriented (a graph without any cycles). In QIF, the cross linking of information is 
done using unique identifiers (as shown by the “id” attribute). 

QIF is based on XML, and uses terminology and semantics from the inspection world to represent the various elements in 
the QIF specification. The QIF information models are contained in files written in the XML Schema Definitions (XSD). 
The QIF XSD Version 2.0 models consists of six application schema files QIFRules, QIFResults, QIFPlans,  QIFProduct, QIFStatistics, 
and QIFMeasurementResources bundled into a QIF Document. QIF also includes a library of XSD schema files containing 
information items used by all QIF applications (Auxiliary,  Characteristics,  Expressions, Features,  GenericExpressions,  
Geometry,  IntermediatesPMI,  Primitives,  PrimitivesPD,  PrimitivesPMI,  Statistics, Topology,  Traceability,  Units, and Visualization ). 

Figure 1 shows a high level perspective of the relationship among the different IF information models. 
At the core of the QIF architecture is the reusable QIF library which contains definitions and components 
that are referenced by the application areas. Around the QIF library core, Figure 13 shows the six QIF
 application area information models, Model-Based Design (MBD) which is equivalent to QIFProduct, 
Plans, Resources, Rules, Results, and Statistics. The “QIF Execution” model is, in the current version of QIF, 
a placeholder for future standardization and is now handled by the Dimensional Measuring Interface Standard (DMIS) standard. 
The order of generation of QIF data generally proceeds clockwise around the diagram, beginning with QIF MBD and ending with 
QIF Statistics. Of note, users of the QIF information model are not required to implement the entire model.

![QIFDonut](images/QIFDonut.png?raw=true)


Included in this github repository is code that generates a First Article Inspection report (FAIR) reports. If you look in the Doc folder
you will find more deatiled documentation on CodeSynthesis, Xerces and FAIR. CodeSynthesis (and as a by product Xerces) was used 
since the code generation is "free" as opposed to other XML commerical product code generation tools - or at least it was indeterminate, which
makes it unreliable tool. CodeSynthesis was better, although Xerces is now primarily a Java tool and C++ related documentation was scarce.
Having done the bloated Java thing already, C++ Xerces-c .  There is a companion project that was to take QIF results and post them to a relational 
data base. The limitation of xerces-c documentation and the overengineering of XML were more pronounced here, but the size of QIF was more
of a hinderance.