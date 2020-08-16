/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
// Example5Dlg.cpp : implementation file
// The server select dialog
//

#include "stdafx.h"
#include <string>
#include "Example5.h"
#include "Example5Dlg.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExample5Dlg dialog

CExample5Dlg::CExample5Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExample5Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExample5Dlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExample5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExample5Dlg)
	DDX_Control(pDX, IDC_CHECK1, m_AllowSpider);
	DDX_Control(pDX, IDC_CHECK2, m_CanBeMaster);
	DDX_Control(pDX, IDC_EDIT2, mChatHandle);
	DDX_Control(pDX, IDC_EDIT1, mJoinEdit);
	DDX_Control(pDX, IDC_LIST1, mServerListControl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExample5Dlg, CDialog)
	//{{AFX_MSG_MAP(CExample5Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static char *names[8] = {"Fred","John","Clive","Morph","Seb","Wibble","Yippee","Hello"};

BOOL CExample5Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	m_AllowSpider.SetCheck(1);
	m_CanBeMaster.SetCheck(1);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	mXPSession = ((CExample5App *)AfxGetApp())->mXPSession;
	
	SetTimer(1,500,0);
	OnButton3();

	CTime time = CTime::GetCurrentTime();
	int seed = time.GetSecond();
	srand(seed);
	int index = rand() & 7;

	mChatHandle.SetWindowText(names[index]);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExample5Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExample5Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExample5Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CExample5Dlg::OnButton3() 
{
	// TODO: Add your control notification handler code here

	mServerListControl.ResetContent();
	mXPSession->Find();
}

void CExample5Dlg::OnTimer(UINT_PTR nIDEvent) 
{
	std::string url;
	// TODO: Add your message handler code here and/or call default
	url = mXPSession->EnumerateFound();
	if (url != "")
	{
		mServerListControl.AddString(url.c_str());
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CExample5Dlg::OnButton2() 
{
	// Try to get a session allocated on port 2050
	mXPSession->SetGameChannel(2050);
	mXPSession->Create(std::string("Example5"));

	ShowWindow(SW_MINIMIZE);
	CChatDlg dlg2;
	mChatHandle.GetWindowText(dlg2.mName,64);
	int nResponse = dlg2.DoModal();


	EndDialog(IDOK);
}

void CExample5Dlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	char buffer[512];
	mJoinEdit.GetWindowText(buffer,sizeof(buffer));
	if (strlen(buffer) > 0)
	{
		mXPSession->Join(std::string(buffer));

		ShowWindow(SW_MINIMIZE);
		CChatDlg dlg2;
		mChatHandle.GetWindowText(dlg2.mName,64);
		int nResponse = dlg2.DoModal();

		EndDialog(IDOK);
	}
}

void CExample5Dlg::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	int sel = mServerListControl.GetCurSel();
	if (sel != LB_ERR)
	{
		char buffer[512];
		mServerListControl.GetText(sel,buffer);
		mJoinEdit.SetWindowText(buffer);
	}
}

void CExample5Dlg::OnChangeEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}


void CExample5Dlg::OnCheck1() 
{
	if (m_AllowSpider.GetCheck())
	{
		mXPSession->SetCanSpider(true);
	}
	else
	{
		mXPSession->SetCanSpider(false);
	}
}

void CExample5Dlg::OnCheck2() 
{
	if (m_CanBeMaster.GetCheck())
	{
		mXPSession->SetCanBecomeMaster(true);
	}
	else
	{
		mXPSession->SetCanBecomeMaster(false);
	}
}

