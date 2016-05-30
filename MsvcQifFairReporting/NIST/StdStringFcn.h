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
  return  StdStringFormat("%4d-%02d-%02dT%02d:%02d:%02d.%04d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
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

inline std::string UrlEncode(const std::string& input)
{
    std::ostringstream ssOut;
    ssOut << std::setbase(16);
    for(std::string::const_iterator i = input.begin(); i != input.end(); ++i)
    {
        if(isalnum(*i))
            ssOut << *i;
        else
            ssOut << '%' << std::setw(2) << std::setfill('0') << ((unsigned int)(unsigned char)*i);
    }
    return ssOut.str();
} 
// std::string test = urldecode("http%3A%2F%2F");
inline std::string UrlDecode ( const std::string& str ) 
{
	std::string result;
	std::string::size_type i;
	for (i = 0; i < str.size(); ++i)
	{
		if (str[i] == '+')
		{
			result += ' ';
		}
		else if (str[i] == '%' && str.size() > i+2)
		{
			std::istringstream iss("0x" + str.substr(i+1,2));
			long h=0;
			if(iss>>std::hex>>h)
				result += (unsigned char) h;
			i += 2;
		}
		else
		{
			result += str[i];
		}
	}
	return result;
}
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
inline std::string StdStringFormat(const char* format, ...)
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

	//std::ifstream in(filename, std::ios::in | std::ios::binary);
	//if (in)
	//{
	//	in.seekg(0, std::ios::end);
	//	contents.resize(in.tellg());
	//	in.seekg(0, std::ios::beg);
	//	in.read(&contents[0], contents.size());
	//	in.close();
	//	return true;
	//}
	//return false;
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
inline std::string ConvertToString(T dStratys)
{
	std::stringstream stream;
	try{
		stream << dStratys;
	}catch(...){}

	return stream.str();
}

inline std::string HrMinSecFormat(std::string sec)
{
	int nsec=(int) ConvertString<double>(sec,0.0);
	return StdStringFormat("%02d:%02d:%02d", nsec/3600,  (nsec%3600)/60,  nsec%60);
}

inline std::string HrMinSecFormat(int nsec)
{
	return StdStringFormat("%02d:%02d:%02d", nsec/3600,  (nsec%3600)/60,  nsec%60);
}


// From: http://www.ccp4.ac.uk/ccp4bin/viewcvs/pointless/string_util.cpp?rev=1.1&content-type=text/vnd.viewcvs-markup

// Return string of length <fieldwidth> with text centred on position
// cenpos (numbered from 0)
//#ifndef MAX
//#define MAX(X,Y) ((X>Y) ?  X : Y)
//#endif
//#ifndef MIN
//#define MIN(X,Y) ((X<Y) ?  X : Y)
//#endif

inline double MAX(double x, double y) { if(x>y) return x; return y; }
inline double MIN(double x, double y) { if(x<y) return x; return y; }
inline double POSMAX(double x, double y, double dMin=0) {x=MAX(x,dMin); y=MAX(y,dMin); if(x>y) return x;   return y;}
inline double POSMIN(double x, double y, double dMin=0) {x=MAX(x,dMin); y=MAX(y,dMin);  if(x<y) return x; return y; }

#include <streambuf>
#include <ostream>

template <class cT, class traits = std::char_traits<cT> >
class basic_nullbuf: public std::basic_streambuf<cT, traits> {
	typename traits::int_type overflow(typename traits::int_type c)
	{
		return traits::not_eof(c); // indicate success
	}
};

template <class cT, class traits = std::char_traits<cT> >
class basic_onullstream: public std::basic_ostream<cT, traits> {
public:
	basic_onullstream():
	  std::basic_ios<cT, traits>(&m_sbuf),
		  std::basic_ostream<cT, traits>(&m_sbuf)
	  {
		  init(&m_sbuf);
	  }

private:
	basic_nullbuf<cT, traits> m_sbuf;
};

typedef basic_onullstream<char> onullstream;
typedef basic_onullstream<wchar_t> wonullstream;

// off the internet - who knows if it works...
inline std::vector<std::string> Tokenize(const std::string& str,const std::string& delimiters)
{
	std::vector<std::string> tokens;
	std::string::size_type delimPos = 0, tokenPos = 0, pos = 0;

	if(str.length()<1)  return tokens;
	while(1){
		delimPos = str.find_first_of(delimiters, pos);
		tokenPos = str.find_first_not_of(delimiters, pos);

		if(std::string::npos != delimPos){
			if(std::string::npos != tokenPos){
				if(tokenPos<delimPos){
					tokens.push_back(str.substr(pos,delimPos-pos));
				}else{
					tokens.push_back("");
				}
			}else{
				tokens.push_back("");
			}
			pos = delimPos+1;
		} else {
			if(std::string::npos != tokenPos){
				tokens.push_back(str.substr(pos));
			} else {
				tokens.push_back("");
			}
			break;
		}
	}
	return tokens;
}

inline std::vector<std::string> Tokenizer(const std::string& str,const std::string& delimiters, bool bKeepToken=false)
{
	std::vector<std::string> tokens;
	std::string::size_type delimPos = 0, tokenPos = 0, pos = 0;

	if(str.length()<1)  return tokens;
	std::string strcopy = str;

	while(strcopy.size() > 0)
	{
		delimPos = strcopy.find_first_of(delimiters, pos);
		//tokenPos = str.find_first_not_of(delimiters, pos);

		if(std::string::npos != delimPos)
		{
			size_t n = delimPos;
			if(bKeepToken)
				n++;
			tokens.push_back(strcopy.substr(0,n));
			strcopy=strcopy.substr(delimPos+1);
		}
		else
		{
			if(!strcopy.empty())
				tokens.push_back(strcopy);
			strcopy.clear();
		}
	}
	return tokens;
}
template<typename T>
inline std::vector<T> TokenList(const std::string& str,const std::string& delimiters)
{
 std::vector<T> tokens;
 std::string::size_type delimPos = 0, tokenPos = 0, pos = 0;

 if(str.length()<1)  return tokens;
 while(1){
   delimPos = str.find_first_of(delimiters, pos);
   tokenPos = str.find_first_not_of(delimiters, pos);

   if(std::string::npos != delimPos)
   {
	   if(std::string::npos != tokenPos)
	   {
		   if(tokenPos<delimPos)
		   {
			   std::string s1 = str.substr(pos,delimPos-pos);
			   Trim(s1);
			   tokens.push_back(ConvertString<T>(s1,T()));
		   }
		   else
		   {
			   tokens.push_back(T());
		   }
     }
	 else
	 {
       tokens.push_back(T());
     }
     pos = delimPos+1;
   } 
   else {
     if(std::string::npos != tokenPos){
       tokens.push_back(ConvertString<T>(str.substr(pos), T()));
     } else {
       tokens.push_back(T());
     }
     break;
   }
 }
 return tokens;
}
inline std::vector<std::string>Lines(const std::string str)
{
	std::vector<std::string>lines;
	std::istringstream input;
    input.str(str);
    for (std::string line; std::getline(input, line); ) 
	{
       lines.push_back(line);
    }
	return lines;
}
inline std::vector<std::string>TrimmedTokenize(std::string value, std::string delimiter)
{
	std::vector<std::string>tokens = Tokenize(value, delimiter);
	for(size_t i=0; i< tokens.size(); i++)
	{
		// double check
		if(tokens[i]== delimiter)
		{
			tokens.erase(tokens.begin()+i);
			continue;
		}
		tokens[i] = Trim(tokens[i]);
	}
	return tokens;
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
//#include <dirent.h>
//inline std::string RecursiveFileFind(std::string path, std::string filetitle)
//{
//	struct dirent *fname;
//	DIR *od;   
//	std::string filename;
//	if (path[path.length()-1] != '\\') 
//		path += "\\";
//	od = opendir(path.c_str());
//
//	while((od != NULL) && (fname = readdir(od)) != NULL)
//	{
//		if((strcmp(fname->d_name, ".") != 0) && (strcmp(fname->d_name, "..") != 0))
//		{
//			if(fname->d_type == DT_DIR)
//			{
//				filename = path +  fname->d_name; 
//				std::string fullpath = RecursiveFileFind(filename, filetitle);
//				if(!fullpath.empty())
//					return fullpath;
//			}
//
//			if(fname->d_type == DT_REG)
//			{
//				if(filetitle == fname->d_name) 
//					return  path +  fname->d_name;;
//			}
//		}
//	}
//	return "";
//}

inline std::string ReplaceOnce(
							   std::string result, 
							   const std::string& replaceWhat, 
							   const std::string& replaceWithWhat)
{
	const size_t pos = result.find(replaceWhat);
	if (pos==-1) return result;
	result.replace(pos,replaceWhat.size(),replaceWithWhat);
	return result;
}


// usual one can have infinite loop (e.g., \r\n for \n
inline void ReplaceAll( tstring& tInput, tstring tFind, tstring tReplace ) 
{ 
	size_t uPos = 0; 
	size_t uFindLen = tFind.length(); 
	size_t uReplaceLen = tReplace.length();
	if( uFindLen == 0 )
	{
		return;
	}

	for( ;(uPos = tInput.find( tFind, uPos )) != tstring::npos; )
	{
		tInput.replace( uPos, uFindLen, tReplace );
		uPos += uReplaceLen;
	}

}

inline bool ReplacePattern(std::string & inStr, std::string oldBeginningStr, std::string oldEndingStr, std::string newStr)
{
	size_t i,j,k;

	// Sanity checks
	if(inStr.size()<1) return false;
	if(oldBeginningStr.size()<1) return false;
	if(oldEndingStr.size()<1) return false;

	// Setup up StringEx sizes
	size_t newCnt = newStr.size();  

	i=0; // Let find determine where to start.

	int cnt=0;
	for(;i<inStr.size();){
		if((j=inStr.find(oldBeginningStr,  i))==std::string::npos)break;
		if((k=inStr.find(oldEndingStr, j))==std::string::npos) break;
		inStr = inStr.substr(0,j)+ newStr + inStr.substr(k+1);
		i=j+newCnt;
		cnt++;
	}
	if(cnt>0) return true;
	return false;
}

// poor mans pattern match
inline std::string FindPattern(std::string & inStr, std::string oldBeginningStr, std::string oldEndingStr)
{
	size_t i,j,k;

	// Sanity checks
	if(inStr.size()<1) return false;
	if(oldBeginningStr.size()<1) return false;
	if(oldEndingStr.size()<1) return false;


	i=0; // Let find determine where to start.

//	int cnt=0;
	for(;i<inStr.size();){
		if((j=inStr.find(oldBeginningStr,  i))==std::string::npos)break;
		if((k=inStr.find(oldEndingStr, j))==std::string::npos) break;
		return  inStr.substr(j+1,k);
//		i=j+newCnt;
//		cnt++;
	}
	return "";
}
#endif