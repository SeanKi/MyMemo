#include <windows.h>
#define BUFFER_LEN 4096
// 2005-07-12
// 이전에 정리하지 못했던 2002-11-8의 수정부분을 찾아 작업 - PARSE_sLeft
// 2003-12-11
// *** MyMemo에 사용됨 2002-05-11과 2003-2-12의 엇갈린 부분을 수정하였으므로, 2002-05-11일 버전을 사용하지 말고 이것을 사용할 것!!
// 2003-2-12
/*
- 2002.11.8
 PARSE_GetNextToken - Second Param char * -> LPCSTR
 * dev/mysource/strki/strki.c 와 dev/enpia/drmproject/include/strki.c의 변경사항을 합침
 변경 수정된 주요 함수 - PARSE_sLeft, PARSE_GetMid2
// 2002.5.13
// PARSE_GetMid2 수정
//2001.9.21 정리
/*
PARSE_sRight : 문자열의 오른쪽 끝에서 특정 문자(1글자)가 나타난 이후 부터 보여주기
PARSE_GetFileExt : 파일 확장자 얻어오기(파일의 경로 중에서 가장 끝에 나타나는 \ 이후의 문자열을 얻어옴) 
*/
//2001.9.15 정리
/*
PARSE_GetNextToken : 구분자(/, ',' 등)에 의한 토큰값가져오기, 더이상 토큰이 없으면 FALSE를 반환
PARSE_IsNullStr : 문자열이 ""인지 체크
GetTag2와 GetMid2가 기존 함수와 다른 점은 문자열을 찾고 다음 위치를 움직인 문자수 만금 정수 pointer로 넘긴다는 점이다. 이 값을 이것을 부른 함수에서 필요시 처리하면 된다.
PARSE_GetTag2 : HTML이나 XML등의 태그 사이에 값을 가져옴
PARSE_GetMid2 : 두 문자열 사이의 값을 가져옴
PARSE_GetNextTagNelem : 태그 사이의 값과 element값을 얻어 온다.

PARSE_GetPathAddBackSlash : 디렉토리 경로에 \ 가 없으면 추가
PARSE_GetPathAddSlash : URL 경로에 /가 없으면 추가 한다.
*/
//2000.12.27 정리
/*
PARSE_sLeft : 문자열 중 특정 문자열 앞의 문자열을 구한다.
PARSE_sMid : 문자열 중 특정 문자열 들 사이의 문자열을 구한다.
PARSE_SkipStr : 문자열 중 특정 문자열 찾아 그 다음 문자열의 포인터를 구한다.
PARSE_MidTag : 문자열 중 특정 태그의 시작과 끝 사이의 문자열을 구한다.
PARSE_GetNextTag : 문자열 중 처음으로 찾은 태그의 이름을 구하고 그 태그의 시작과 끝 사이의 문자열을 구한다.
RemoveStr : 문자열 중에서 특정 문자를 모두 제거. 문자열 크기는 BUFFER_LEN크기에 준함
*/

#ifdef __cplusplus
extern "C"   {
#endif

BOOL PARSE_IsNullStr(LPCTSTR szSub1)
{
	if (*szSub1 == '\0')
		return TRUE;
	return FALSE;
}

/* 최초 작성일 :
// 최종 수정일 : 2002.11.8
// 설명 : szInStr에서 szSub1 앞까지의 문자열을 반환
//szInStr문자열 중에서 처음부터 szSub1문자열 앞의 문자만 뽑아내 szOutStr에 넘김
//szSub1이 널이면 리턴 값은 -3 //? 꼭 굳이 필요할까
//szSub1을 찾을 수 없으면 리턴 값은 -1
//szSub1을 찾으면 리턴 값은 찾은 위치
// Update : 
- 2002.11.8
  주 함수를 strncpy으로 바꾸고 문자열 마지막을 '\0'으로 끊어줌
// 2002-5-11
// 일부 크기만 얻어오는 기능을 strncpy 로 변경 -> 2003-12-11 누락되어 있어 다시 작업
*/
int PARSE_sLeft(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1)
{
// 반환값 : 0 이상 - 얻어온 문자열의 크기 반환, -3 - 시작 문자열 szSub1이 "" 
//		-1 - szSub1을 찾을 수 없을 경우  
// szOutStr : szInStr에서 szSub1 앞까지의 문자열을 반환
// szSub1 : 
	char *pdest = NULL;
	if (*szSub1 == '\0') return -3; //비교할 sub 문자열이 NULL이면 NULL을 리턴
	pdest = 	strstr(szInStr, szSub1);
	if (pdest)
	{
		int len = pdest - szInStr;
		strncpy(szOutStr, szInStr, len);
		szOutStr[len] = '\0';
		return len;
	}
	return -1;
}

// 최초 작성일 :
// 최종 수정일 :
// 설명 :
//반환문자열의 크기는 BUFFER_LEN에 준함
// Update :
// 2002-5-11
// Fatal error - szSub1을 찾아야 하는 데 szInStr를 찾는 오류가 있어 수정->PARSE_sLeft부분과 *pos증가 시키는 부분
// 임시버퍼 szTmp가 초기화 안되어 있어 초기화함
BOOL PARSE_GetNextToken(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1, int * pos)
{
	/* 인자들을 스트링 변수에 추가한다.*/
	char szTmp[BUFFER_LEN]={0,};
	char * p;
	int iipos;
	if (!pos) //int pos가 NULL이면 FALSE를 return
		return 0;
	p = (char *)szInStr + *pos;// + strlen(szInStr);
	if (p[0]) {
		iipos = PARSE_sLeft(szTmp, p, szSub1);
		if (iipos == -1) {
			strcpy(szOutStr, p);
                        *pos += strlen(p);
			return 1;
		}
		*pos += (iipos + strlen(szSub1));
		strcpy(szOutStr, szTmp);
		return 1;
	}
	return 0;
}

//szInStr에서 szSub1문자열을 찾아 그 다음 위치의 문자열 포인터를 리턴
//szSub1을 못찾으면 널을 리턴
char * PARSE_SkipStr(LPCTSTR szInStr, LPCTSTR szSub1)
{
	char *pdest = NULL;
	if (!szSub1) return NULL; //비교할 sub 문자열이 NULL이면 NULL을 리턴
	pdest = 	strstr(szInStr, szSub1);
	if (pdest)
	{
		if (strlen(szInStr) > strlen(pdest + strlen(szSub1)))
			return pdest + strlen(szSub1); //문자열을 pSub1만큼 스킵했을 때 원래 문자열을 넘어서면
	}
	return NULL;
}

//c문자열을 못찾으면 ""을 리턴하지 않는다.
char * PARSE_sRight(LPTSTR szOutStr, LPCTSTR szInStr, char c)
{
	char * p;
	if ((p=strrchr(szInStr, c))==NULL)
		strcpy(szOutStr, "");
	else
		strcpy(szOutStr, p+1);//1은 찾는 문자가 1자임을 의
	return szOutStr;
}

char * PARSE_GetFileExt(LPTSTR szOutStr, LPCTSTR szInStr)
{
	return PARSE_sRight(szOutStr, szInStr, '.');
}

char * PARSE_GetFileTitle(LPTSTR szOutStr, LPCTSTR szInStr)
{
	char * p;
	if (PARSE_IsNullStr(PARSE_sRight(szOutStr, szInStr, '\\'))) {
		strcpy(szOutStr, szInStr);
	}
	p = PARSE_SkipStr(szOutStr, ":");
	if (p)
	{
		char *p2 = (char *)malloc(strlen(szOutStr));
		strcpy(p2, p);
		strcpy(szOutStr, p2);
		free(p2);
	}
	return szOutStr;
}

/* 문자열 szInStr 중에 szSub1에 해당하는 문자열을 찾아 bBefore값에 따라 이전 또는 
그 뒤에 szSub2문자열을 추가*/
BOOL PARSE_InsertStr(LPTSTR szOutStr, LPTSTR szInStr, LPCTSTR szSub1, LPCTSTR szSub2, BOOL bBefore)
{
	char *pdest = NULL;
	//char *pVal = NULL;
	int iTmp;
	int ret = FALSE;
	int fullsize = strlen(szInStr) + strlen(szSub1);

	if (fullsize < 1) return FALSE;
	//pVal = (char *)malloc(fullsize);
	if (bBefore) {
		iTmp = PARSE_sLeft(szOutStr, szInStr, szSub1);
		if (iTmp >= 0){
			strcat(szOutStr, szSub2);
			strcat(szOutStr, strstr(szInStr, szSub1));
			ret = TRUE;
		}
	} else {
		pdest = PARSE_SkipStr(szInStr, szSub1);
		if (pdest) {
			strncpy(szOutStr, szInStr, pdest - szInStr);
			strcat(szOutStr, szSub2);
			strcat(szOutStr, pdest);
		}
	}
	return ret;
}

//입력 문자열은 BUFFER_LEN범위 내의 크기만 가능하다.
int PARSE_RemoveStr(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1, int pos)
{
//	const char * szSub1 = "/";
	/* 인자들을 스트링 변수에 추가한다.*/

	char pTmp[BUFFER_LEN];
	char szTmp[BUFFER_LEN];
	int iipos;
	if (PARSE_IsNullStr(szSub1))
		return -3;
	strcpy(pTmp, szInStr);
	if (pos >= 0) { // offset이 0이상일 때만
		char * p;
		strcpy(szOutStr, ""); //결과값 문자열을 초기화
		p = pTmp + pos;// + strlen(szSub1);
		while (p[0]) {
			iipos = PARSE_sLeft(szTmp, p, szSub1);
			if (iipos == -1) {
				strcat(szOutStr, p);
				break;
			}
			if (*szTmp != '\0') //공백이 여러개인 경우 무시
				strcat(szOutStr, szTmp);
			p = p + iipos + strlen(szSub1);
		}	
	}
	return  1;
}
//들어온 문자열에서 szSub1은 BUFFER_LEN이내의 간격에 있어야 함
int PARSE_RemoveStr2(LPTSTR szInStr, LPCTSTR szSub1, int pos)
{
//	const char * szSub1 = "/";
	/* 인자들을 스트링 변수에 추가한다.*/

	char szTmp[BUFFER_LEN];
	LPTSTR szOutStr;
	int iipos;
	if (PARSE_IsNullStr(szSub1))
		return -3;
	szOutStr = (char *)malloc(strlen(szInStr));
	if (pos >= 0) { // offset이 0이상일 때만
		char * p;
		strcpy(szOutStr, ""); //결과값 문자열을 초기화
		p = szInStr + pos;// + strlen(szSub1);
		while (p[0]) {
			iipos = PARSE_sLeft(szTmp, p, szSub1);
			if (iipos == -1) {
				strcat(szOutStr, p);
				break;
			}
			if (*szTmp != '\0') //공백이 여러개인 경우 무시
				strcat(szOutStr, szTmp);
			p = p + iipos + strlen(szSub1);
		}	
	}
	strcpy(szInStr, szOutStr);
	free(szOutStr);
	return  1;
}


//문자열 중에서 szSub1문자열과 szSub2문자열 사이에 있는 문자열을 뽑아 szOutStr에 넘김
//szSub1문자열을 못찾으면 -1을 리턴, 반환된 문자열의 크기는 0이될 수 있으므로 0보다 작은 값이 에러
//szSub2문자열을 못찾아도 -2을 리턴
//문자열을 뽑아내고 그 문자열의 크기를 리턴
int PARSE_sMid(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1, LPCTSTR szSub2)
{
	char * pszc, * pszc2;
	char cTmp;
	pszc = strstr(szInStr,szSub1);
	if (pszc == NULL) return -1;
	pszc2 = strstr(pszc,szSub2);
	if (pszc2 == NULL) return -2;
	cTmp = *pszc2;
	*pszc2 = '\0';
	strcpy(szOutStr, pszc + strlen(szSub1));
	*pszc2 = cTmp;
	return strlen(szOutStr);
}

// 최초 작성일 :
// 최종 수정일 :
// 설명 : Mid를 가져오고 움직인 크기를 넘긴다.
// Update :
//2003-2-12
//시작 문자열을 찾고 끝문자열 찾을때 시작 문자열 찾은 그 다음 위치부터 찾는다. 시작 문자열과 끝문자열이 같을때 오동작 하는 문제 해결
int PARSE_GetMid2(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1, LPCTSTR szSub2, int * iMovePos)
{
// 반환값 : 0 이상 - 얻어온 문자열의 크기 반환, -3 - 시작 문자열 szSub1이 "" 
//		-1 - 시작 문자열을 찾을 수 없을 경우  
//		-2 - 끝 문자열을 찾을 수 없을 경우
// szOutStr : szInStr에서 szSub1과 szSub2문자열 사이의 문자열을 넘겨 받음, 할당된 버퍼의 주소가 넘어와야 함
//     szSub2가 "" 이면 문자열 끝까지 반환
// szSub1 : 시작 문자열
// szSub2 : 끝 문자열

	char * pszc, *pszc1, * pszc2;
	if (*szSub1 == '\0')
		return -3;
	pszc = strstr(szInStr,szSub1);
	if (pszc == NULL) return -1;
	pszc2 = strstr(pszc+strlen(szSub1),szSub2);
	if (pszc2 == NULL) return -2;
	strcpy(szOutStr, "");
	//cTmp = *pszc2;
	pszc1 = pszc + strlen(szSub1);
	//*pszc2 = '\0';
	strncat(szOutStr, pszc1, pszc2 - pszc1);
//	strcpy(szOutStr, pszc + strlen(szSub1));
//	*pszc2 = cTmp;
	if (iMovePos)
		*iMovePos = (pszc - szInStr) + strlen(szSub1) + strlen(szOutStr) + strlen(szSub2);
	return strlen(szOutStr);
}

// Update 
// 2002.6.17 주석 추가
//태그의 값을 가져오고 움직인 크기를 넘긴다.
//태그표시에 공백이 있으면 안된다.
int PARSE_GetTag2(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szTagStr, int * iMovePos)
{
// [out] szOutStr : Return String
// [in] szInStr : Original String
// [in] szTagStr : Tag String - valid just until 64 characters
// [in, out] iMovePos : if NULL, then ignore. if valid value, Original's moved position
// [out, retval] : Return String's length or error

	char szTmp[64]; //태그 이름의 크기는 64바이트 이상을 넘어갈 수 없다..
	char szTmp2[64];
	wsprintf(szTmp, "<%s>",szTagStr); //대소문자를 구분하므로 정확한 입력이 들어와야 한다.
	wsprintf(szTmp2, "</%s>",szTagStr);
	return PARSE_GetMid2(szOutStr, szInStr, szTmp, szTmp2, iMovePos);
}

int PARSE_GetNextTag(LPTSTR szOutStr, LPTSTR * pszPos, LPTSTR szTagStr)
{
	int pos;
	int iTmp = PARSE_GetTag2(szOutStr, *pszPos, szTagStr, &pos);
	*pszPos += pos;
	return iTmp;
}

// 문자열 pszPos에서 szTagStr의 이름을 가진 태그를 찾아 그 태그 안의 문자열을 szOutStr에 넘긴다.
// 여기에서 태그는 <태그> </태그>와 같이 한 쌍으로 된 경우만 유효하다.
// 시작태그나 끝태그를 못찾으면 0을 리턴
// 찾으면 그 문자열의 크기를 리턴
int PARSE_MidTag(LPTSTR szOutStr, LPCTSTR szTagStr, LPTSTR * pszPos)
{
	char szTmp[64]; //태그 이름의 크기는 64바이트 이상을 넘어갈 수 없다..
	char * p;
	char * pStart;
	wsprintf(szTmp, "<%s>",szTagStr); //대소문자를 구분하므로 정확한 입력이 들어와야 한다.
	p = strstr(*pszPos, szTmp);
	if (!p)
		return 0;
	pStart = p + strlen(szTmp);
	wsprintf(szTmp, "</%s>",szTagStr);
	p = 	strstr(pStart, szTmp);
	if (!p)
		return 0;
	strncpy(szOutStr, pStart, (p - pStart));
	szOutStr[p - pStart] = '\0'; //문장의 끝에 NULL 표시를 한다.
	*pszPos = p + strlen(szTmp);
	return strlen(szOutStr);
}

int HexstrToInt(char a, char b)
{
	int ret ;
	int ai,bi;

	if( a>='0' && a<='9')
			ai = a-'0';
	else if(a>='a'  && a <= 'f')
			ai = (a-'a')  + 10;
	else if(a>='A'  && a <= 'F')
			ai = (a-'A')  + 10;

	if( b>='0'  && b<='9')
			bi = b-'0';
	else if(b>='a'  && b <= 'f')
			bi = b-'a' + 10;
	else if(b>='A'  && b <= 'F')
			bi = b-'A' + 10;

/////////////4bit 왼쪽 쉬프트..
	ret = ai * 16 + bi;
	return ret;
}

//szPath가 내용이 없으면 NULL을 리턴, szRval과 szPath가 메모리 할당이 꼭 되어 있어야 함
char * PARSE_GetPathAddBackSlash(LPTSTR szRval, LPCTSTR szPath) //for LocalPath
{
	if (szPath[0] == '\0')
		return NULL;
	strcpy(szRval, szPath);
	if (szPath[strlen(szPath)-1] != '\\')
		strcat(szRval, "\\");
	return szRval;
}

char * PARSE_GetPathAddSlash(LPTSTR szRval, LPCTSTR szPath) //for URL
{
	if (szPath[0] == '\0')
		return NULL;
	strcpy(szRval, szPath);
	if (szPath[strlen(szPath)-1] != '/')
		strcat(szRval, "/");
	return szRval;
}


//입력할 태그의 크기는 <>를 씨워 64자를 넘어가면 안된다.
//입력받을 element의 크기는 256자까지 가능하다.
int PARSE_GetNextTagNelem(LPTSTR szOutStr, LPTSTR * pszPos, LPCTSTR szTagStr, LPTSTR szOutElem)
{
	char szTmp[64]; //태그 이름의 크기는 64바이트 이상을 넘어갈 수 없다..
	char szTmp2[512]; //element등을 담을 임시
	int iTmp;
	char * p;
	char * pStart = * pszPos;
	int iMovePos;
	wsprintf(szTmp, "<%s",szTagStr); //대소문자를 구분하므로 정확한 입력이 들어와야 한다.
	iTmp = PARSE_GetMid2(szTmp2, *pszPos, szTmp, ">", &iMovePos);
	*pszPos += iMovePos;
	//iTmp 0이면 태그가 없음, -1
	while(1) {
		if (iTmp < 0) {
			*pszPos = pStart; //에러가 있으므로 위치를 처음으로 돌린다.
			return iTmp; //태그를 찾을 수 없거나 에러
		}
		if (iTmp == 0) {
			strcpy(szOutElem, "");
			break;
		}
		if (szTmp2[0] == ' ') //적어도 태그와 element사이에는 하나의 공백이 있어야 한다.
		{
			strcpy(szOutElem, szTmp2 + 1);
			break;
		}
		iTmp = PARSE_GetMid2(szTmp2, *pszPos, szTmp, ">", &iMovePos);

		*pszPos += iMovePos;
	};
	pStart = *pszPos;
	wsprintf(szTmp, "</%s>",szTagStr);
	p = 	strstr(pStart, szTmp);
	if (!p)
		return -2; //반대쪽 끝이 없음
	strncpy(szOutStr, pStart, (p - pStart));
	szOutStr[p - pStart] = '\0'; //문장의 끝에 NULL 표시를 한다.
	*pszPos = p + strlen(szTmp);
	return strlen(szOutStr);
}

// szTagStr : 추가할 태그
// szSub1 : 추가할 태그의 value
/*
int PARSE_AddTag(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szTagStr, LPCTSTR szSub1, int * iMovePos)
{
	int size = strlen(szInStr);
	int addsize = strlen(szSub1) + strlen(szTagStr)*2 + 8;
	PBYTE destbuf = (PBYTE)malloc(size+addsize);
	memset(destbuf, 0, size+addsize);
	char szTmp[128]={0,};
	string sTemp;
	wsprintf(szTmp, "<%s>",szTagStr);
	string sLTag = szTmp;
	wsprintf(szTmp, "</%s>",szTagStr);
	string sRTag = szTmp;

	int iret = PARSE_GetTag2((LPSTR)destbuf, szInStr, szTagStr, NULL); //전체 String중에서 해당 태그가 있는지 먼저 찾는다.
	if (iret<0) { //szTagStr이 없으면 
		sTemp = sLTag; 
		sTemp += szSub1;
		sTemp += sRTag;
		sTemp += szInStr; // 추가한 태그를 가장 앞에 둔다.
		strcpy(szOutStr, sTemp.c_str());
	} else { //Tag가 있으면 Update
		PARSE_sLeft((LPSTR)destbuf, szInStr, sLTag.c_str());
		sTemp = (LPCSTR)destbuf;
		string sTemp2 = PARSE_SkipStr(szInStr, sRTag.c_str());
		string sTemp3 = sLTag; //젤 먼저 DOI 번호를 집어 넣는다.
		sTemp3 += szSub1;
		sTemp3 += sRTag";
		strcpy(szOutStr, sTemp.c_str());
		strcat(szOutStr, sTemp3.c_str());
		strcat(szOutStr, sTemp2.c_str());
	}
	free(destbuf);
	return strlen(szOutStr);
} */
#ifdef __cplusplus
}  /*extern "C" */
#endif
