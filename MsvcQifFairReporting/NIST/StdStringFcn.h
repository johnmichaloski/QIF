//
// StdStringFcn.h
//

// This software was developed by U.S. Government employees as part of
// their official duties and is not subject to copyright. No warranty implied 
// or intended.

#ifndef StdStringFcn
#define StdStringFcn
#include <stdarg.h>   

#include <algorithm> 
#include <functional> 
#include <locale>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iosfwd>

#include <string>
#include <iomanip>
#include <vector>
#include <map>

typedef unsigned int UINT;
typedef unsigned long DWORD;
#ifdef UNICODE
#define TEXT(X) L#X
#define TCHAR wchar
#else
#define TEXT(X) X
#define TCHAR char
#endif

#ifdef WINDOWS
inline std::string nowtimestamp()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	return  StrFormat("%4d-%02d-%02dT%02d:%02d:%02d.%04d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}
//inline std::string ExeDirectory()
//{
//	TCHAR buf[1000];
//	GetModuleFileName(NULL, buf, 1000);
//	std::string path(buf);
//	path=path.substr( 0, path.find_last_of( '\\' ) +1 );
//	return path;
//}
#endif
//#ifdef UNICODE
//typedef std::wstring  tstring;
//typedef	std::wifstream tifstream;
//typedef std::wstringstream  tstringstream;
//typedef std::wstreambuf tstreambuf;
//typedef std::wfstream tfstream;
//typedef	std::wofstream tofstream;
//#define t_str w_str
//#else
//typedef std::string  tstring;
//typedef	std::ifstream tifstream;
//typedef std::stringstream  tstringstream;
//typedef std::streambuf tstreambuf;
//typedef std::fstream tfstream;
//#define t_str c_str
//typedef	 std::ofstream tofstream;
//#define A2T(X) X
//
//#endif
#define tstring std::string 

// maximum mumber of lines the output console should have
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

inline std::string & RemoveSpaces(std::string& str)
{
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	return str;
}
inline std::string & RemoveChar(std::string& str, char c)
{
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), c); 
	str.erase(end_pos, str.end()); 
	return str;
}
inline std::string StrFormat(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	int m;
	size_t n= strlen(format) + 1028;
	std::string tmp(n,'0');	


	// Kind of a bogus way to insure that we don't
	// exceed the limit of our buffer
	while((m=_vsnprintf(&tmp[0], n-1, format, ap))<0)
	{
		n=n+1028;
		tmp.resize(n,'0');
	}
	va_end(ap);
	return tmp.substr(0,m);

}
//////////////////////////////////////////////////
// std::string Exensions
//////////////////////////////////////////////////
// trim from start
inline std::string &LeftTrim(std::string &str) 
{

	size_t startpos = str.find_first_not_of(" \t\r\n"); 
	if( std::string::npos != startpos )
		str = str.substr( startpos ); 
	return str;
}
// trim from end
inline std::string &RightTrim(std::string &str, std::string trim=" \t\r\n") 
{
	size_t endpos = str.find_last_not_of(trim); 
	if(std::string::npos != endpos ) 
		str = str.substr( 0, endpos+1 ); 		 
	return str;
}
// trim from both ends
inline std::string &Trim(std::string &s) 
{
	return LeftTrim(RightTrim(s));
}
inline bool EndsWith (std::string const &fullString, std::string const &ending) 
{ 
	if (fullString.length() > ending.length()) { 
		return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending)); 
	} else { 
		return false; 
	} 
} 
inline std::string ExtractDirectory( const std::string& path )
{
	return path.substr( 0, path.find_last_of( '\\' ) +1 );
}
inline std::string ExtractFilename( const std::string& path )
{
	return path.substr( path.find_last_of( '\\' ) +1 );
}
inline std::string ExtractFiletitle( const std::string& path )
{
	std::string filename= path.substr( path.find_last_of( '\\' ) +1 );
	return filename.substr( 0, filename.find_last_of( '.' ) );
}
inline std::string ChangeExtension( const std::string& path, const std::string& ext )
{
	std::string filename = ExtractFilename( path );
	return ExtractDirectory( path ) +filename.substr( 0, filename.find_last_of( '.' ) ) +ext;
}

inline std::string  LocalTimetamp()
{
	time_t rawtime;
	time ( &rawtime );
	return ctime (&rawtime) ;
}
inline std::string fileToText( const std::string &filename )
{
	std::ifstream fin(filename.c_str());
	return std::string((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
}

inline bool ReadFile(std::string filename, std::string & contents)
{
	std::ifstream in(filename.c_str()); 
	std::stringstream buffer; 
	buffer << in.rdbuf(); 
	contents= buffer.str();
	return true;
} 
inline void WriteFile(std::string filename, std::string & contents)
{
	std::ofstream outFile(filename.c_str());
	//Write the string and its null terminator !!
	outFile << contents.c_str();
}
inline std::vector<std::string> &Split(const std::string &s, char delim, std::vector<std::string> &elems, bool bKeepEmpty=false) 
{ 
	std::stringstream ss(s); 
	std::string item; 
	while(std::getline(ss, item, delim)) { 
		if(item.empty() && !bKeepEmpty)
			continue;
		elems.push_back(item); 
	} 
	return elems; 
} 
inline bool CompareInsensitive(std::string &s1, std::string &s2)
{
	return _stricmp(s1.c_str(), s2.c_str()) == 0;
}
inline std::vector<std::string> Split(const std::string &s, char delim) { 
	std::vector<std::string> elems; 
	return Split(s, delim, elems); 
} 
typedef std::map<std::string, std::string>::iterator StrMapIterator;
inline std::string GetIniSection(std::string filename, std::string section)
{
	size_t n,m;
	std::string contents;
	ReadFile(filename, contents);
	if((n=contents.find("["+section +"]"))==std::string::npos)
		return "";
	contents=contents.substr(n+section.size()+2);
	if((m=contents.find("["))==std::string::npos)
		m=contents.size();
	return Trim(contents.substr(0, m-1));
}

inline std::map<std::string, std::string> ParseIniString(std::string s)
{	
	std::map<std::string, std::string> symbols;
	std::vector<std::string> lines = Split(s, '\n');
	for(UINT i=0; i< lines.size(); i++)
	{
		std::vector<std::string> tokens=Split(lines[i], '=');
		if(tokens.size()<2)
			continue;
		symbols[Trim(tokens[0])]=Trim(tokens[1]);

	}
	return symbols;

}
inline std::map<std::string, std::string> ParseIniSection(std::string filename, std::string section)
{
	std::string s=GetIniSection(filename, section);
	return ParseIniString(s);
}

inline std::string GetIniProfileString(std::string filename, std::string section, std::string key)
{
	std::map<std::string, std::string> symbols = ParseIniSection(filename, section);
	return symbols[key];
}
template<typename T>
inline T ConvertString(std::string dStratys, T defaultval)
{
	T result;
	std::istringstream stream(dStratys);

	if(stream >> result)
		return result;
	return defaultval;
}
template<typename T>
inline std::string ConvertToString(T value)
{
	std::stringstream stream;
	try{
		stream << value;
	}catch(...){}

	return stream.str();
}

// Convert the string to lowercase
inline std::string MakeUpper(std::string str)
{
	std::transform(str.begin(),str.end(),str.begin(),toupper);
	return str;
}

inline std::string  MakeLower(std::string str)
{
	std::transform(str.begin(),str.end(),str.begin(),tolower);
	return str;
}
#include <sys/stat.h> 
inline bool FileExists(std::string filename)
{
	struct stat info;
	int ret = -1;

	//get the file attributes
	ret = stat(filename.c_str(), &info);
	if(ret == 0) 
	{
		return true;
	}
	return false;
}

#endif