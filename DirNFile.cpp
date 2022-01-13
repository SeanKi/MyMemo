// SaveSpace.cpp: implementation of the CDirNFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirNFile.h"
#include "ParseKi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirNFile::CDirNFile()
{
	m_bDirDelToo = FALSE;
	m_bExceptDel = FALSE;
	m_RemoteType = 0;

	//�̸� ������ Except���� ����
	m_sAryExceptDel.Add("exe");
}

CDirNFile::~CDirNFile()
{

}

void CDirNFile::InitExceptDelList(CStringArray &sAry)
{
	int iSize = sAry.GetSize();
	for (int i =0; i<iSize;i++)
	{
		CString s = sAry[i];
		s.MakeLower();
		s = "." + s;
		m_sAryExceptDel.Add(s);
	}
	m_sAryExceptDel.FreeExtra();

}

void CDirNFile::InitDelXList(CStringArray &sAry)
{
	int iSize = sAry.GetSize();
	for (int i =0; i<iSize;i++)
	{
		CString s = sAry[i];
//		s.MakeLower();
		m_sAryDelX.Add(s);
	}
	m_sAryDelX.FreeExtra();
}

BOOL CDirNFile::DelX(LPCTSTR str)
{
	CString l_path(str);
	CFileFind filefind;
	int nCount=0;
	CString l_dir = l_path;
	l_path += "\\*.*";
	if (filefind.FindFile(l_path))
	{
		BOOL bVal;
		do 
		{
			bVal = filefind.FindNextFile();
			TRACE0(filefind.GetFileName()+"\n");
			if (!filefind.IsDirectory()) //ã�� ���� ������ ��츸
			{
				BOOL bDel = TRUE;
				CString sfilepath = filefind.GetFilePath();
				if (m_bExceptDel)
				{
					CString s;
					int i = sfilepath.ReverseFind('.');
					s = sfilepath.Mid(i+1);
					if (i > 0)
						if (IsInExceptDelList(s))
							bDel = FALSE;
				}
				if (bDel)
					nCount = DeleteFile(sfilepath)?(nCount+1):nCount;
			}
		} while (bVal);
	}
	if (m_bDirDelToo)
		RemoveDirectory(l_dir);
	return nCount;
}

BOOL CDirNFile::SearchDelX(LPCTSTR str)
{
	CString l_path(str);
	if (l_path == "") return FALSE;
	CFileFind filefind;
	l_path += "\\*.*";
	CStringArray sAryDirList;
	if (filefind.FindFile(l_path))
	{
		BOOL bVal;
		do 
		{
			bVal = filefind.FindNextFile();
			TRACE0(filefind.GetFileName()+"\n");
			if (filefind.IsDirectory())
			{
				CString filename = filefind.GetFileName();
				if ((filename == "." )||(filename == ".."))
					continue;
				sAryDirList.Add(filefind.GetFilePath());
			}
		} while (bVal);
	}

// ���� ���丮�� ���� ������丮 ����߿� �ִ��� �˻�
	int iSize = sAryDirList.GetSize();
	int i,j;
	int iCount=0; //ã�� ���� ī��Ʈ
	for (i=0; i<iSize;i++)
	{
		CString srVal;
		int jSize = m_sAryDelX.GetSize();
		for (j=0; j<jSize; j++)
		{
			CString l_sAryDelX(m_sAryDelX[j]);
			CString sAryDirList(sAryDirList[i]);
			srVal = sAryDirList.Right(l_sAryDelX.GetLength());
			if (!l_sAryDelX.CompareNoCase(srVal)) //������ �����̸� ������ ��ü �н��̹Ƿ� �ٷ� ���ϸ� �ȵ�
			{
				DelX(sAryDirList);
				iCount++;
			}
		}
	}
	if (iCount == 0) //���� ������丮�� ��ã������ �� ������丮�� ���� ������. 
	{
		int jSize = sAryDirList.GetSize();
		for (j=0; j<jSize; j++)
		{
			SearchDelX(sAryDirList[j]);
		}
		
	}
	return iCount;
}

BOOL CDirNFile::IsInExceptDelList(LPCTSTR str)
{
	int jSize = m_sAryExceptDel.GetSize();
	CString sExcept(str);
	for (int j=0; j<jSize; j++)
	{
//		sExcept.MakeLower();
		if (!m_sAryExceptDel[j].CompareNoCase(sExcept))
		{
			return TRUE;
		}
	}

	return FALSE;

}

int CDirNFile::DelTreeFiles(CString spath, CStringArray &sAry)
{
	if (spath == "") return 0;
	int iCount = 0;
	CFileFind filefind;
	spath += "\\*.*";
	if (filefind.FindFile(spath))
	{
		BOOL bVal;
		do 
		{
			bVal = filefind.FindNextFile();
			if (filefind.IsDirectory())
			{
				CString filename = filefind.GetFileName();
				if ((filename == "." )||(filename == ".."))
					continue;
				iCount += DelTreeFiles(filefind.GetFilePath(), sAry);
			} else { //�̰��� �����̸�
				CString ext = CParseKi::sRight(filefind.GetFileName(), '.');
				for (int i=0; i<sAry.GetSize();i++) {
					if (sAry.GetAt(i) == ext) {
						DeleteFile(filefind.GetFilePath());
						TRACE0(filefind.GetFileName()+"\n");
						iCount++;
						break;
					}
				}
			}
		} while (bVal);
	}
	return iCount;

}

int CDirNFile::DelTreeFiles(CString spath, BOOL bDirDelToo, CInetFile * pInetf)
{
	if (spath == "") return 0;
	int iCount = 0;
	//CFileFind filefind;
	BOOL bContinue;
	HINTERNET hInternet;
	HANDLE hFindFile;
	WIN32_FIND_DATA finddata;
	CString strFindPath = spath;
	CString strFileName;
	char SEP = pInetf?'/':'\\';
	if(strFindPath.Right(1) != SEP) strFindPath += SEP;
	strFindPath += "*.*";
	if (!pInetf) { //local file�̸�
		bContinue = ((hFindFile = FindFirstFile(strFindPath, &finddata)) != INVALID_HANDLE_VALUE); 
	} else if(pInetf->m_dwService == INTERNET_SERVICE_FTP) {
		pInetf->SetCurrentDirectory(pInetf->m_initDir.c_str()); //�߰��� ���� ���丮 ������ �ٸ� ������ �Ǿ� ������ �߻��Ͽ�, �׻� �ʱ� ���丮�� �̵��� �� �۾��ϵ��� ��
		pInetf->SetCurrentDirectory(spath);
		strFindPath = "*.*";
		hInternet = FtpFindFirstFile(pInetf->GetConnectHandle(), strFindPath, &finddata, 0, 0);
		bContinue = (hInternet != NULL);
	}
	vector <WIN32_FIND_DATA> vFinddata;
	while(bContinue) {
		vFinddata.push_back(finddata);
		if (!pInetf) { //local file�̸�
			bContinue = FindNextFile(hFindFile, &finddata);
		} else {
			bContinue = InternetFindNextFile(hInternet, &finddata);
		}
	}
	if (!pInetf) { //local file�̸�
		FindClose(hFindFile);
	} else if(pInetf->m_dwService == INTERNET_SERVICE_FTP) {
		InternetCloseHandle(hInternet);
		pInetf->SetCurrentDirectory(pInetf->m_initDir.c_str());
	}
	CString thisdir = spath;
	// ���丮 Ž��
	BOOL bDirectory;
	string strFileTitle;
	for (int i=0; i < vFinddata.size(); i++) {
		finddata = vFinddata[i];
		bDirectory = finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
		strFileName = finddata.cFileName;
		string strFilePath = thisdir + SEP + strFileName;
			if (bDirectory)
			{
				CString filename = strFileName;
				if ((filename == "." )||(filename == ".."))
					continue;
				iCount += DelTreeFiles(strFilePath.c_str(), bDirDelToo, pInetf);
			} else { //�̰��� �����̸�
				if (!pInetf) {
					DeleteFile(strFilePath.c_str());
				} else {
					pInetf->DeleteFile(strFilePath.c_str());
				}
				TRACE0(strFileName+"\n");
				iCount++;
			}
		
	}
	if (bDirDelToo) {
		if (!pInetf) {
			iCount += RemoveDirectory(thisdir)?1:0;
		} else {
			iCount += pInetf->RemoveDirectory(thisdir)?1:0;
		}
	}
	return iCount;
}

//'2003.1.9 FileOpen��Ŀ��� FindFile������� ���� -> File Open����� FTP���� ���� Rename����� ����� �������� ���ϰ� �ϴ� ������ �߻��߾���
BOOL FileExists(LPCTSTR FilePath, CInetFile * pInetf) {
	WIN32_FIND_DATA finddata;
	BOOL bContinue;
	if (pInetf) {
		HINTERNET hInternet;
		hInternet = FtpFindFirstFile(pInetf->GetConnectHandle(), FilePath, &finddata, 0, 0);
		bContinue = (hInternet != NULL);
		InternetCloseHandle(hInternet);
		return bContinue;
/*		BOOL b = pInetf->OpenFile(FilePath, "");
		if (!b)
			return FALSE;// File not exist or other error '2003.1.9
		pInetf->CloseFile();
		return TRUE;
		*/
	}
	HANDLE hFindFile;
	bContinue = ((hFindFile = FindFirstFile(FilePath, &finddata)) != INVALID_HANDLE_VALUE); 
	FindClose(hFindFile);
	return bContinue;
	/*
	HANDLE hFile;
	hFile = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;	//
	CloseHandle(hFile);
	return TRUE;*/
}
