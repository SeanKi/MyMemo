// FileVersion.h: interface for the CFileVersion class.
// by Manuel Laflamme
//////////////////////////////////////////////////////////////////////

#ifndef __FILEVERSION_H_
#define __FILEVERSION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <TCHAR.H>
#pragma warning(disable:4786)
#include <vector>
#include <string>

using namespace std ;

class CFileVersion
{ 
// Construction
public: 
    CFileVersion();

// Operations	
public: 
    BOOL    Open(LPCTSTR lpszModuleName);
    void    Close();

    string QueryValue(LPCTSTR lpszValueName, DWORD dwLangCharset = 0);
    string GetFileDescription()  {return QueryValue(_T("FileDescription")); };
    string GetFileVersion()      {return QueryValue(_T("FileVersion"));     };
    string GetInternalName()     {return QueryValue(_T("InternalName"));    };
    string GetCompanyName()      {return QueryValue(_T("CompanyName"));     }; 
    string GetLegalCopyright()   {return QueryValue(_T("LegalCopyright"));  };
    string GetOriginalFilename() {return QueryValue(_T("OriginalFilename"));};
    string GetProductName()      {return QueryValue(_T("ProductName"));     };
    string GetProductVersion()   {return QueryValue(_T("ProductVersion"));  };

    BOOL    GetFixedInfo(VS_FIXEDFILEINFO& vsffi);
    string GetFixedFileVersion();
    string GetFixedProductVersion();

// Attributes
protected:
    LPBYTE  m_lpVersionData; 
    DWORD   m_dwLangCharset; 

// Implementation
public:
    ~CFileVersion(); 
}; 

#endif  // __FILEVERSION_H_

