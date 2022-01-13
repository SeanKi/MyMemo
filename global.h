#ifndef _GLOBAL
#define _GLOBAL

#pragma warning(disable:4786)
#include <string>
#include <vector>
#include <map>

#include "StrKi.h"
#include "InetFile.h"

using namespace std ;

//Icon ID ���� 
#define ID_ROOT		0
#define ID_VIRTUAL_L	1 //���⼭ ���ʹ� ����
#define ID_VIRTUAL_R	2
#define ID_CLOSEDFOLDER 3
#define ID_OPENFOLDER	4
#define ID_TEXT_FILE	5 //���⼭ ���ʹ� �޸�
#define ID_WORD_FILE	6
#define ID_HTML_FILE	7

#define CANEDITITEM(iImage) (iImage > ID_VIRTUAL_R)

#define UM_ITEMDROPED  WM_USER + 100
#define UM_ITEMRESELECT WM_USER + 101
#define UM_FORCESAVE WM_USER + 102
#define UM_NEEDSAVE WM_USER + 103

#define DirRemarkFILEList	"_FolderComment.txt|___��������.txt|ReadMe.txt"
//#define DirRemarkFILE	"_DirRemark.txt" //"___��������.txt"
#define GETUCID(x) x+(WM_USER+1000) //means GetUserCtrlID

enum {VIEW_TEXT = 0, VIEW_HTML, VIEW_CSV};
enum {CTL_HTML = 0, CTL_TEXT, CTL_CSV};
enum {KIND_ROOT = 0x1, KIND_VIRTUAL = 0x2, KIND_FOLDER = 0x4, KIND_FILE = 0x8, 
		KIND_REMOTE = 0x10}; //nKind
enum {REMOTE_LOCAL=0, REMOTE_NETWORK, REMOTE_FTP, REMOTE_HTTP, REMOTE_WEBFOLDER};
enum {ITEM_ROOT = 0, ITEM_VIRTUAL_LOCAL, ITEM_VIRTUAL_NETWORK, //NETWORK�� MS Network
	ITEM_VIRTUAL_FTP, ITEM_FOLDER, ITEM_FILE_TEXT, ITEM_FILE_HTML, 
	ITEM_FILE_CSV, ITEM_FILE_DOC, ITEM_FILE_HWP, ITEM_FILE_MMO};
enum {ICON_ROOT = 0, ICON_VIRTUAL_LOCAL, ICON_VIRTUAL_NETWORK, 
	ICON_VIRTUAL_FTP, ICON_FOLDER_C, ICON_FOLDER_O, ICON_FILE_HTML, ICON_FILE_TEXT, // ~_C = ~_CLOSE, ~_O = ~_OPEN
	ICON_FILE_CSV, ICON_FILE_DOC, ICON_FILE_HWP, ICON_FILE_MMO,
	ICON_SROOT , ICON_SVIRTUAL_LOCAL, ICON_SVIRTUAL_NETWORK, 
	ICON_SVIRTUAL_FTP, ICON_SFOLDER, ICON_SFILE_HTML, ICON_SFILE_TEXT, 
	ICON_SFILE_CSV, ICON_SFILE_DOC, ICON_SFILE_HWP, ICON_SFILE_MMO};


struct MemoList { //VirtualFolder����
	CString name;
	CString path;
	HTREEITEM hItem;
};
struct ViewFileList { //�������ϴ� ������ Ȯ���ڿ� ������ �̹��� ��ȣ�� ����
	char szExt[10];
	BYTE imgNum;
};
struct MemoItem {
	HTREEITEM hItem; //�ش� �������� HTREEITEM(Pointer)
	int iClsItemIndex; //�������� ������ ���� ����
};
class VirtualFolderList { //Virtual Folder ���� Ȯ��
public:
	string title;
	string path; //url -> path //modified 2002.10.13 - if remote -. it's remote dir
	string hostaddr; //host address(IP or DNS) and port <-> folder::hostname
	HTREEITEM hItem;
	int nItem; //�ش��ϴ� ClsItem�迭 ��ȣ -> Virtual Folder�� Local, Network, FTP ���� �뵵
	int nRemoteType; //����Ʈ ���� - ftp, ms network ���
	string userid; // <-> folder::username
	string password; // <-> folder::password
	BOOL NeedUpdatedNow;//Update�� �ʿ䰡 ������ TRUE�� ����, Update�� ������ �ٽ� FALSE�� �����
	CInetFile * pInetf;
	VirtualFolderList() {
		hItem = NULL;
		nItem = 0;
		nRemoteType = 0;
		NeedUpdatedNow = FALSE;
		pInetf = NULL;
	}
	char GetDirSeparator() {
		if (nRemoteType == REMOTE_FTP || nRemoteType == REMOTE_HTTP) {
			return '/';
		}
		return '\\';
	}
};


//////////////////////////////////////////////////////////////////////
//		Class : ClsItem
//////////////////////////////////////////////////////////////////////
//2003.4.11 ���� iImage, iSelectedImage, nKind�� ���
class ClsItem {
public:
	vector <string> exts; //Ȯ���� ���
	int nctrl; //������Ʈ��
	int iImage; //�⺻ ������ ��ȣ
	int iSelectedImage; //���ý� ������ ������ ��ȣ
	int nKind; //�������� ���� <- �������� ������ ������ DataValue�� �־���
	int nRemoteType; //����Ʈ ���� - ftp, ms network ��� //����� VirtualFolderList�� �ִ� ���� ���� �� ����.
	int nVFolderIndex; //�������� index (VirtualFolderList�� �迭��ȣ)
	BOOL bExternalProgram;
	string ExternalProgramPath;
	void SetItem(int nctrl, int iImage, int iSelectedImage, int nKind, LPCTSTR szExts) {
		if (nKind == KIND_FILE) {
			char szVal[255]={0,};
			int pos = 0;
			while(PARSE_GetNextToken(szVal, (LPSTR)szExts, ";", &pos)) {
				string sVal(szVal);
				exts.push_back(sVal);// �����ݷ����� ���е� ���ڿ��� �迭�� ��ȯ���־�� ��
			}
		}
		this->nctrl = nctrl; 
		this->iImage = iImage;
		this->iSelectedImage = iSelectedImage;
		this->nKind = nKind;
	}
};
	
CString MyLoadString(UINT nID);
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
BOOL GetFolder(CString* strSelectedFolder,
				   const char* lpszTitle,
				   const HWND hwndOwner, 
				   const char* strRootFolder, 
				   const char* strStartFolder);

//MFC sample superpad���� ������
static TCHAR BASED_CODE szSettings[] = _T("Settings");
static TCHAR BASED_CODE szTabStops[] = _T("TabStops");
static TCHAR BASED_CODE szFont[] = _T("Font");
static TCHAR BASED_CODE szPrintFont[] = _T("PrintFont");
static TCHAR BASED_CODE szHeight[] = _T("Height");
static TCHAR BASED_CODE szWeight[] = _T("Weight");
static TCHAR BASED_CODE szWidth[] = _T("Width");
static TCHAR BASED_CODE szOutPrecision[] = _T("OutPrecision");
static TCHAR BASED_CODE szQuality[] = _T("Quality");
static TCHAR BASED_CODE szItalic[] = _T("Italic");
static TCHAR BASED_CODE szUnderline[] = _T("Underline");
static TCHAR BASED_CODE szPitchAndFamily[] = _T("PitchAndFamily");
static TCHAR BASED_CODE szCharSet[] = _T("CharSet");
static TCHAR BASED_CODE szFaceName[] = _T("FaceName");
static TCHAR BASED_CODE szWordWrap[] = _T("WordWrap");

static TCHAR BASED_CODE szSystem[] = _T("System");

// ��ü �⺻ ����
static   LONG    DefHeight = -13;
static   LONG    DefWidth=0;
static   LONG    DefEscapement=0;
static   LONG    DefOrientation=0;
static   LONG    DefWeight=400;
static   BYTE    DefItalic=0;
static   BYTE    DefUnderline=0;
static   BYTE    DefStrikeOut=0;
static   BYTE    DefCharSet=DEFAULT_CHARSET;
static   BYTE    DefOutPrecision=3;
static   BYTE    DefClipPrecision=0;
static   BYTE    DefQuality=1;
static   BYTE    DefPitchAndFamily=49;
static   TCHAR   DefFaceName[LF_FACESIZE]="����ü";


static void WriteProfileFont(LPCTSTR szSec, const LOGFONT* plf, LOGFONT* plfOld);

static void GetProfileFont(LPCTSTR szSec, LOGFONT* plf);

DWORD ShowErrorMsg(LPCTSTR title=NULL);
VirtualFolderList * GetVFL(vector <VirtualFolderList> * pvVFL, HTREEITEM hItem);

#endif