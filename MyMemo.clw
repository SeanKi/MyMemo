; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyMemoView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mymemo.h"
LastPage=0

ClassCount=10
Class1=CDlgVirtualFolder
Class2=CLabel
Class3=CLeftView
Class4=CMainFrame
Class5=CMyMemoApp
Class6=CAboutDlg
Class7=CMyMemoDoc
Class8=CMyMemoView
Class9=CTreeViewExt

ResourceCount=9
Resource1=IDD_VIRTUALFOLDER
Resource2=IDR_TOOLBAR1
Resource3=IDD_FIND_REPLACE (English (U.S.))
Resource4=IDR_MAINFRAME
Resource5=IDD_FIND (English (U.S.))
Resource6=IDR_MAINFRAME (English (U.S.))
Class10=CDlgVirtualFolder2
Resource7=IDD_ABOUTBOX
Resource8=IDD_VIRTUALFOLDER2
Resource9=IDD_VIRTUALFOLDER (English (U.S.))

[CLS:CDlgVirtualFolder]
Type=0
BaseClass=CDialog
HeaderFile=DlgVirtualFolder.h
ImplementationFile=DlgVirtualFolder.cpp

[CLS:CLabel]
Type=0
BaseClass=CStatic
HeaderFile=Label.h
ImplementationFile=Label.cpp

[CLS:CLeftView]
Type=0
BaseClass=CTreeViewExt
HeaderFile=LeftView.h
ImplementationFile=LeftView.cpp
Filter=C
VirtualFilter=VWC
LastObject=ID_VIEW_TAPSTOPS_1

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=ID_VIEW_ONTOP

[CLS:CMyMemoApp]
Type=0
BaseClass=CWinApp
HeaderFile=MyMemo.h
ImplementationFile=MyMemo.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MyMemo.cpp
ImplementationFile=MyMemo.cpp
LastObject=CAboutDlg

[CLS:CMyMemoDoc]
Type=0
BaseClass=CDocument
HeaderFile=MyMemoDoc.h
ImplementationFile=MyMemoDoc.cpp

[CLS:CMyMemoView]
Type=0
BaseClass=CEditView
HeaderFile=MyMemoView.h
ImplementationFile=MyMemoView.cpp
Filter=C
VirtualFilter=VWC
LastObject=ID_EDIT_FIND

[CLS:CTreeViewExt]
Type=0
BaseClass=CTreeView
HeaderFile=TreeViewExt.h
ImplementationFile=TreeViewExt.cpp

[DLG:IDD_VIRTUALFOLDER]
Type=1
Class=CDlgVirtualFolder
ControlCount=8
Control1=IDC_STATIC,static,1342308481
Control2=IDC_STATIC,static,1342308353
Control3=IDOK,button,1342373889
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_BUTTON1,button,1342242816
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=8
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC1,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_MYHOME,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT1,edit,1344342020

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_SAVE
Command2=ID_FILE_PRINT
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_MEMO1
Command5=ID_APP_EXIT
Command6=ID_MEMO_NEW_VIRTUAL
Command7=ID_MEMO_NEW_FOLDER
Command8=ID_MEMO_NEW_MEMO
Command9=ID_RENAME
Command10=ID_DELETE
Command11=ID_REFRESH
Command12=ID_PROPERTY
Command13=ID_EDIT_UNDO
Command14=ID_EDIT_CUT
Command15=ID_EDIT_COPY
Command16=ID_EDIT_PASTE
Command17=ID_EDIT_FIND
Command18=ID_EDIT_REPEAT
Command19=ID_EDIT_REPLACE
Command20=ID_ADDTODAY
Command21=ID_SETTORDONLY
Command22=ID_VIEW_TOOLBAR
Command23=ID_VIEW_STATUS_BAR
Command24=ID_VIEW_WORDWRAP
Command25=ID_CHOOSEFONT
Command26=ID_VIEW_ONTOP
Command27=ID_VIEW_TAPSTOPS_1
Command28=ID_VIEW_TAPSTOPS_2
Command29=ID_APP_ABOUT
CommandCount=29

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_SAVE
Command2=ID_FILE_PRINT
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_MEMO1
Command5=ID_APP_EXIT
Command6=ID_MEMO_NEW_VIRTUAL
Command7=ID_MEMO_NEW_FOLDER
Command8=ID_MEMO_NEW_MEMO
Command9=ID_RENAME
Command10=ID_DELETE
Command11=ID_REFRESH
Command12=ID_PROPERTY
Command13=ID_EDIT_UNDO
Command14=ID_EDIT_CUT
Command15=ID_EDIT_COPY
Command16=ID_EDIT_PASTE
Command17=ID_EDIT_FIND
Command18=ID_EDIT_REPEAT
Command19=ID_EDIT_REPLACE
Command20=ID_ADDTODAY
Command21=ID_SETTORDONLY
Command22=ID_VIEW_TOOLBAR
Command23=ID_VIEW_STATUS_BAR
Command24=ID_VIEW_WORDWRAP
Command25=ID_CHOOSEFONT
Command26=ID_VIEW_ONTOP
Command27=ID_VIEW_TEST
Command28=ID_VIEW_TAPSTOPS_1
Command29=ID_VIEW_TAPSTOPS_2
Command30=ID_APP_ABOUT
CommandCount=30

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_MEMO_NEW_VIRTUAL
Command2=ID_MEMO_NEW_FOLDER
Command3=ID_MEMO_NEW_MEMO
Command4=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_FILE_PRINT
Command9=ID_VIEW_WORDWRAP
Command10=ID_VIEW_READONLY
Command11=ID_APP_ABOUT
CommandCount=11

[TB:IDR_TOOLBAR1]
Type=1
Class=?
Command1=ID_BUTTON32791
Command2=ID_BUTTON32792
CommandCount=2

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_EDIT_FIND
Command3=ID_EDIT_REPLACE
Command4=ID_FILE_NEW
Command5=ID_FILE_OPEN
Command6=ID_FILE_PRINT
Command7=ID_FILE_SAVE
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_RENAME
Command12=ID_EDIT_REPEAT
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
Command15=ID_EDIT_CUT
Command16=ID_EDIT_UNDO
CommandCount=16

[DLG:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_VIRTUALFOLDER (English (U.S.))]
Type=1
Class=CDlgVirtualFolder
ControlCount=7
Control1=65535,static,1342308480
Control2=65535,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_BUTTON1,button,1342242816
Control7=IDCANCEL,button,1342242816

[DLG:IDD_VIRTUALFOLDER2]
Type=1
Class=CDlgVirtualFolder2
ControlCount=18
Control1=IDC_STATIC,static,1342308481
Control2=IDC_STATIC,static,1342308353
Control3=IDOK,button,1342373889
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_BUTTON1,button,1342242816
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_RADIO1,button,1342177289
Control10=IDC_RADIO2,button,1342177289
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT3,edit,1350631552
Control16=IDC_EDIT4,edit,1350631552
Control17=IDC_EDIT5,edit,1350631584
Control18=IDC_EDIT6,edit,1350631552

[CLS:CDlgVirtualFolder2]
Type=0
HeaderFile=DlgVirtualFolder2.h
ImplementationFile=DlgVirtualFolder2.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_MEMO_NEW_VIRTUAL

[DLG:IDD_FIND (English (U.S.))]
Type=1
Class=?
ControlCount=12
Control1=65535,static,1342308352
Control2=IDC_EDIT_FIND,edit,1350762624
Control3=IDC_CHECK_WHOLE_WORD,button,1342373891
Control4=IDC_CHECK_MATCH_CASE,button,1342242819
Control5=IDC_CHECK_REGEX,button,1342242819
Control6=1072,button,1342308359
Control7=IDC_RADIO_UP,button,1342308361
Control8=IDC_RADIO_DOWN,button,1342177289
Control9=IDC_CHECK_CLOSE_ON_MATCH,button,1342242819
Control10=IDOK,button,1342373889
Control11=IDCANCEL,button,1342242816
Control12=IDC_BUTTON_HELP,button,1342242816

[DLG:IDD_FIND_REPLACE (English (U.S.))]
Type=1
Class=?
ControlCount=13
Control1=65535,static,1342308352
Control2=IDC_EDIT_FIND,edit,1350762624
Control3=65535,static,1342308352
Control4=IDC_EDIT_REPLACE,edit,1350762624
Control5=IDC_CHECK_WHOLE_WORD,button,1342373891
Control6=IDC_CHECK_MATCH_CASE,button,1342242819
Control7=IDC_CHECK_REGEX,button,1342242819
Control8=IDC_CHECK_LIKE_NOTEPAD,button,1342242819
Control9=IDOK,button,1342373889
Control10=IDC_BUTTON_REPLACE,button,1342242816
Control11=IDC_BUTTON_REPLACE_ALL,button,1342242816
Control12=IDCANCEL,button,1342242816
Control13=IDC_BUTTON_HELP,button,1342242816

