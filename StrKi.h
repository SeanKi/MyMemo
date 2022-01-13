#ifndef _STRKI_H
#define _STRKI_H

#ifdef __cplusplus
extern "C" {
#endif

char * PARSE_GetFileTitle(LPTSTR szOutStr, LPCTSTR szInStr);
char * PARSE_sRight(LPTSTR szOutStr, LPCTSTR szInStr, char c);
char * PARSE_GetFileExt(LPTSTR szOutStr, LPCTSTR szInStr);
BOOL PARSE_InsertStr(LPTSTR szOutStr, LPTSTR szInStr, LPCTSTR szSub1, LPCTSTR szSub2, BOOL bBefore);
BOOL PARSE_GetNextToken(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1, int * pos);
BOOL PARSE_IsNullStr(LPCTSTR szSub1);
int HexstrToInt(char a, char b);
int PARSE_RemoveStr2(LPTSTR szInStr, LPCTSTR szSub1, int pos);
int PARSE_RemoveStr(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1, int pos);
char * PARSE_GetPathAddBackSlash(LPTSTR szRval, LPCTSTR szPath); //for LocalPath
char * PARSE_GetPathAddSlash(LPTSTR szRval, LPCTSTR szPath); //for URL
int PARSE_GetNextTag(LPTSTR szOutStr, LPTSTR * pszPos, LPTSTR szTagStr);
int PARSE_GetNextTagNelem(LPTSTR szOutStr, LPTSTR * pszPos, LPCTSTR szTagStr, LPTSTR szOutElem);
int PARSE_GetMid2(LPTSTR szOutStr, LPCTSTR szStr, LPCTSTR szSub1, LPCTSTR szSub2, int * iMovePos);
int PARSE_GetTag2(LPTSTR szOutStr, LPCTSTR szStr, LPCTSTR szTagStr, int * iMovePos);
int PARSE_sLeft(LPTSTR szOutStr, LPCTSTR szStr, LPCTSTR szSub1);
char * PARSE_SkipStr(LPCTSTR szStr, LPCTSTR szSub1);
int PARSE_sMid(LPTSTR szOutStr, LPCTSTR szStr, LPCTSTR szSub1, LPCTSTR szSub2);
int PARSE_MidTag(LPTSTR szOutStr, LPCTSTR szTagStr, LPTSTR * pszPos);

#ifdef __cplusplus
}

#endif

#endif