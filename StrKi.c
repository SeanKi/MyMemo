#include <windows.h>
#define BUFFER_LEN 4096
// 2005-07-12
// ������ �������� ���ߴ� 2002-11-8�� �����κ��� ã�� �۾� - PARSE_sLeft
// 2003-12-11
// *** MyMemo�� ���� 2002-05-11�� 2003-2-12�� ������ �κ��� �����Ͽ����Ƿ�, 2002-05-11�� ������ ������� ���� �̰��� ����� ��!!
// 2003-2-12
/*
- 2002.11.8
 PARSE_GetNextToken - Second Param char * -> LPCSTR
 * dev/mysource/strki/strki.c �� dev/enpia/drmproject/include/strki.c�� ��������� ��ħ
 ���� ������ �ֿ� �Լ� - PARSE_sLeft, PARSE_GetMid2
// 2002.5.13
// PARSE_GetMid2 ����
//2001.9.21 ����
/*
PARSE_sRight : ���ڿ��� ������ ������ Ư�� ����(1����)�� ��Ÿ�� ���� ���� �����ֱ�
PARSE_GetFileExt : ���� Ȯ���� ������(������ ��� �߿��� ���� ���� ��Ÿ���� \ ������ ���ڿ��� ����) 
*/
//2001.9.15 ����
/*
PARSE_GetNextToken : ������(/, ',' ��)�� ���� ��ū����������, ���̻� ��ū�� ������ FALSE�� ��ȯ
PARSE_IsNullStr : ���ڿ��� ""���� üũ
GetTag2�� GetMid2�� ���� �Լ��� �ٸ� ���� ���ڿ��� ã�� ���� ��ġ�� ������ ���ڼ� ���� ���� pointer�� �ѱ�ٴ� ���̴�. �� ���� �̰��� �θ� �Լ����� �ʿ�� ó���ϸ� �ȴ�.
PARSE_GetTag2 : HTML�̳� XML���� �±� ���̿� ���� ������
PARSE_GetMid2 : �� ���ڿ� ������ ���� ������
PARSE_GetNextTagNelem : �±� ������ ���� element���� ��� �´�.

PARSE_GetPathAddBackSlash : ���丮 ��ο� \ �� ������ �߰�
PARSE_GetPathAddSlash : URL ��ο� /�� ������ �߰� �Ѵ�.
*/
//2000.12.27 ����
/*
PARSE_sLeft : ���ڿ� �� Ư�� ���ڿ� ���� ���ڿ��� ���Ѵ�.
PARSE_sMid : ���ڿ� �� Ư�� ���ڿ� �� ������ ���ڿ��� ���Ѵ�.
PARSE_SkipStr : ���ڿ� �� Ư�� ���ڿ� ã�� �� ���� ���ڿ��� �����͸� ���Ѵ�.
PARSE_MidTag : ���ڿ� �� Ư�� �±��� ���۰� �� ������ ���ڿ��� ���Ѵ�.
PARSE_GetNextTag : ���ڿ� �� ó������ ã�� �±��� �̸��� ���ϰ� �� �±��� ���۰� �� ������ ���ڿ��� ���Ѵ�.
RemoveStr : ���ڿ� �߿��� Ư�� ���ڸ� ��� ����. ���ڿ� ũ��� BUFFER_LENũ�⿡ ����
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

/* ���� �ۼ��� :
// ���� ������ : 2002.11.8
// ���� : szInStr���� szSub1 �ձ����� ���ڿ��� ��ȯ
//szInStr���ڿ� �߿��� ó������ szSub1���ڿ� ���� ���ڸ� �̾Ƴ� szOutStr�� �ѱ�
//szSub1�� ���̸� ���� ���� -3 //? �� ���� �ʿ��ұ�
//szSub1�� ã�� �� ������ ���� ���� -1
//szSub1�� ã���� ���� ���� ã�� ��ġ
// Update : 
- 2002.11.8
  �� �Լ��� strncpy���� �ٲٰ� ���ڿ� �������� '\0'���� ������
// 2002-5-11
// �Ϻ� ũ�⸸ ������ ����� strncpy �� ���� -> 2003-12-11 �����Ǿ� �־� �ٽ� �۾�
*/
int PARSE_sLeft(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1)
{
// ��ȯ�� : 0 �̻� - ���� ���ڿ��� ũ�� ��ȯ, -3 - ���� ���ڿ� szSub1�� "" 
//		-1 - szSub1�� ã�� �� ���� ���  
// szOutStr : szInStr���� szSub1 �ձ����� ���ڿ��� ��ȯ
// szSub1 : 
	char *pdest = NULL;
	if (*szSub1 == '\0') return -3; //���� sub ���ڿ��� NULL�̸� NULL�� ����
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

// ���� �ۼ��� :
// ���� ������ :
// ���� :
//��ȯ���ڿ��� ũ��� BUFFER_LEN�� ����
// Update :
// 2002-5-11
// Fatal error - szSub1�� ã�ƾ� �ϴ� �� szInStr�� ã�� ������ �־� ����->PARSE_sLeft�κа� *pos���� ��Ű�� �κ�
// �ӽù��� szTmp�� �ʱ�ȭ �ȵǾ� �־� �ʱ�ȭ��
BOOL PARSE_GetNextToken(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1, int * pos)
{
	/* ���ڵ��� ��Ʈ�� ������ �߰��Ѵ�.*/
	char szTmp[BUFFER_LEN]={0,};
	char * p;
	int iipos;
	if (!pos) //int pos�� NULL�̸� FALSE�� return
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

//szInStr���� szSub1���ڿ��� ã�� �� ���� ��ġ�� ���ڿ� �����͸� ����
//szSub1�� ��ã���� ���� ����
char * PARSE_SkipStr(LPCTSTR szInStr, LPCTSTR szSub1)
{
	char *pdest = NULL;
	if (!szSub1) return NULL; //���� sub ���ڿ��� NULL�̸� NULL�� ����
	pdest = 	strstr(szInStr, szSub1);
	if (pdest)
	{
		if (strlen(szInStr) > strlen(pdest + strlen(szSub1)))
			return pdest + strlen(szSub1); //���ڿ��� pSub1��ŭ ��ŵ���� �� ���� ���ڿ��� �Ѿ��
	}
	return NULL;
}

//c���ڿ��� ��ã���� ""�� �������� �ʴ´�.
char * PARSE_sRight(LPTSTR szOutStr, LPCTSTR szInStr, char c)
{
	char * p;
	if ((p=strrchr(szInStr, c))==NULL)
		strcpy(szOutStr, "");
	else
		strcpy(szOutStr, p+1);//1�� ã�� ���ڰ� 1������ ��
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

/* ���ڿ� szInStr �߿� szSub1�� �ش��ϴ� ���ڿ��� ã�� bBefore���� ���� ���� �Ǵ� 
�� �ڿ� szSub2���ڿ��� �߰�*/
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

//�Է� ���ڿ��� BUFFER_LEN���� ���� ũ�⸸ �����ϴ�.
int PARSE_RemoveStr(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1, int pos)
{
//	const char * szSub1 = "/";
	/* ���ڵ��� ��Ʈ�� ������ �߰��Ѵ�.*/

	char pTmp[BUFFER_LEN];
	char szTmp[BUFFER_LEN];
	int iipos;
	if (PARSE_IsNullStr(szSub1))
		return -3;
	strcpy(pTmp, szInStr);
	if (pos >= 0) { // offset�� 0�̻��� ����
		char * p;
		strcpy(szOutStr, ""); //����� ���ڿ��� �ʱ�ȭ
		p = pTmp + pos;// + strlen(szSub1);
		while (p[0]) {
			iipos = PARSE_sLeft(szTmp, p, szSub1);
			if (iipos == -1) {
				strcat(szOutStr, p);
				break;
			}
			if (*szTmp != '\0') //������ �������� ��� ����
				strcat(szOutStr, szTmp);
			p = p + iipos + strlen(szSub1);
		}	
	}
	return  1;
}
//���� ���ڿ����� szSub1�� BUFFER_LEN�̳��� ���ݿ� �־�� ��
int PARSE_RemoveStr2(LPTSTR szInStr, LPCTSTR szSub1, int pos)
{
//	const char * szSub1 = "/";
	/* ���ڵ��� ��Ʈ�� ������ �߰��Ѵ�.*/

	char szTmp[BUFFER_LEN];
	LPTSTR szOutStr;
	int iipos;
	if (PARSE_IsNullStr(szSub1))
		return -3;
	szOutStr = (char *)malloc(strlen(szInStr));
	if (pos >= 0) { // offset�� 0�̻��� ����
		char * p;
		strcpy(szOutStr, ""); //����� ���ڿ��� �ʱ�ȭ
		p = szInStr + pos;// + strlen(szSub1);
		while (p[0]) {
			iipos = PARSE_sLeft(szTmp, p, szSub1);
			if (iipos == -1) {
				strcat(szOutStr, p);
				break;
			}
			if (*szTmp != '\0') //������ �������� ��� ����
				strcat(szOutStr, szTmp);
			p = p + iipos + strlen(szSub1);
		}	
	}
	strcpy(szInStr, szOutStr);
	free(szOutStr);
	return  1;
}


//���ڿ� �߿��� szSub1���ڿ��� szSub2���ڿ� ���̿� �ִ� ���ڿ��� �̾� szOutStr�� �ѱ�
//szSub1���ڿ��� ��ã���� -1�� ����, ��ȯ�� ���ڿ��� ũ��� 0�̵� �� �����Ƿ� 0���� ���� ���� ����
//szSub2���ڿ��� ��ã�Ƶ� -2�� ����
//���ڿ��� �̾Ƴ��� �� ���ڿ��� ũ�⸦ ����
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

// ���� �ۼ��� :
// ���� ������ :
// ���� : Mid�� �������� ������ ũ�⸦ �ѱ��.
// Update :
//2003-2-12
//���� ���ڿ��� ã�� �����ڿ� ã���� ���� ���ڿ� ã�� �� ���� ��ġ���� ã�´�. ���� ���ڿ��� �����ڿ��� ������ ������ �ϴ� ���� �ذ�
int PARSE_GetMid2(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szSub1, LPCTSTR szSub2, int * iMovePos)
{
// ��ȯ�� : 0 �̻� - ���� ���ڿ��� ũ�� ��ȯ, -3 - ���� ���ڿ� szSub1�� "" 
//		-1 - ���� ���ڿ��� ã�� �� ���� ���  
//		-2 - �� ���ڿ��� ã�� �� ���� ���
// szOutStr : szInStr���� szSub1�� szSub2���ڿ� ������ ���ڿ��� �Ѱ� ����, �Ҵ�� ������ �ּҰ� �Ѿ�;� ��
//     szSub2�� "" �̸� ���ڿ� ������ ��ȯ
// szSub1 : ���� ���ڿ�
// szSub2 : �� ���ڿ�

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
// 2002.6.17 �ּ� �߰�
//�±��� ���� �������� ������ ũ�⸦ �ѱ��.
//�±�ǥ�ÿ� ������ ������ �ȵȴ�.
int PARSE_GetTag2(LPTSTR szOutStr, LPCTSTR szInStr, LPCTSTR szTagStr, int * iMovePos)
{
// [out] szOutStr : Return String
// [in] szInStr : Original String
// [in] szTagStr : Tag String - valid just until 64 characters
// [in, out] iMovePos : if NULL, then ignore. if valid value, Original's moved position
// [out, retval] : Return String's length or error

	char szTmp[64]; //�±� �̸��� ũ��� 64����Ʈ �̻��� �Ѿ �� ����..
	char szTmp2[64];
	wsprintf(szTmp, "<%s>",szTagStr); //��ҹ��ڸ� �����ϹǷ� ��Ȯ�� �Է��� ���;� �Ѵ�.
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

// ���ڿ� pszPos���� szTagStr�� �̸��� ���� �±׸� ã�� �� �±� ���� ���ڿ��� szOutStr�� �ѱ��.
// ���⿡�� �±״� <�±�> </�±�>�� ���� �� ������ �� ��츸 ��ȿ�ϴ�.
// �����±׳� ���±׸� ��ã���� 0�� ����
// ã���� �� ���ڿ��� ũ�⸦ ����
int PARSE_MidTag(LPTSTR szOutStr, LPCTSTR szTagStr, LPTSTR * pszPos)
{
	char szTmp[64]; //�±� �̸��� ũ��� 64����Ʈ �̻��� �Ѿ �� ����..
	char * p;
	char * pStart;
	wsprintf(szTmp, "<%s>",szTagStr); //��ҹ��ڸ� �����ϹǷ� ��Ȯ�� �Է��� ���;� �Ѵ�.
	p = strstr(*pszPos, szTmp);
	if (!p)
		return 0;
	pStart = p + strlen(szTmp);
	wsprintf(szTmp, "</%s>",szTagStr);
	p = 	strstr(pStart, szTmp);
	if (!p)
		return 0;
	strncpy(szOutStr, pStart, (p - pStart));
	szOutStr[p - pStart] = '\0'; //������ ���� NULL ǥ�ø� �Ѵ�.
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

/////////////4bit ���� ����Ʈ..
	ret = ai * 16 + bi;
	return ret;
}

//szPath�� ������ ������ NULL�� ����, szRval�� szPath�� �޸� �Ҵ��� �� �Ǿ� �־�� ��
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


//�Է��� �±��� ũ��� <>�� ���� 64�ڸ� �Ѿ�� �ȵȴ�.
//�Է¹��� element�� ũ��� 256�ڱ��� �����ϴ�.
int PARSE_GetNextTagNelem(LPTSTR szOutStr, LPTSTR * pszPos, LPCTSTR szTagStr, LPTSTR szOutElem)
{
	char szTmp[64]; //�±� �̸��� ũ��� 64����Ʈ �̻��� �Ѿ �� ����..
	char szTmp2[512]; //element���� ���� �ӽ�
	int iTmp;
	char * p;
	char * pStart = * pszPos;
	int iMovePos;
	wsprintf(szTmp, "<%s",szTagStr); //��ҹ��ڸ� �����ϹǷ� ��Ȯ�� �Է��� ���;� �Ѵ�.
	iTmp = PARSE_GetMid2(szTmp2, *pszPos, szTmp, ">", &iMovePos);
	*pszPos += iMovePos;
	//iTmp 0�̸� �±װ� ����, -1
	while(1) {
		if (iTmp < 0) {
			*pszPos = pStart; //������ �����Ƿ� ��ġ�� ó������ ������.
			return iTmp; //�±׸� ã�� �� ���ų� ����
		}
		if (iTmp == 0) {
			strcpy(szOutElem, "");
			break;
		}
		if (szTmp2[0] == ' ') //��� �±׿� element���̿��� �ϳ��� ������ �־�� �Ѵ�.
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
		return -2; //�ݴ��� ���� ����
	strncpy(szOutStr, pStart, (p - pStart));
	szOutStr[p - pStart] = '\0'; //������ ���� NULL ǥ�ø� �Ѵ�.
	*pszPos = p + strlen(szTmp);
	return strlen(szOutStr);
}

// szTagStr : �߰��� �±�
// szSub1 : �߰��� �±��� value
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

	int iret = PARSE_GetTag2((LPSTR)destbuf, szInStr, szTagStr, NULL); //��ü String�߿��� �ش� �±װ� �ִ��� ���� ã�´�.
	if (iret<0) { //szTagStr�� ������ 
		sTemp = sLTag; 
		sTemp += szSub1;
		sTemp += sRTag;
		sTemp += szInStr; // �߰��� �±׸� ���� �տ� �д�.
		strcpy(szOutStr, sTemp.c_str());
	} else { //Tag�� ������ Update
		PARSE_sLeft((LPSTR)destbuf, szInStr, sLTag.c_str());
		sTemp = (LPCSTR)destbuf;
		string sTemp2 = PARSE_SkipStr(szInStr, sRTag.c_str());
		string sTemp3 = sLTag; //�� ���� DOI ��ȣ�� ���� �ִ´�.
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
