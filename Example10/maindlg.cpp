// maindlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "maindlg.h"

static char pathForProduct[MAX_PATH];

CMainDlg::CMainDlg() : mWindowActive(false) , mExitStatus(RNReplicaNet::RNLobby::ProductPatcher::kErrorEncountered)
{
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);


	mProductProgress = GetDlgItem(IDC_PROGRESS1);
	mFileProgress = GetDlgItem(IDC_PROGRESS2);
	mName = GetDlgItem(IDC_STATIC1);
	mName.SetWindowTextA("");

	char path[MAX_PATH];
	DWORD ret = GetCurrentDirectoryA(sizeof(path),path);
	strcat(path,"\\Product");
	strcpy(pathForProduct,path);
	// Start patching the product using a separate thread
	ProductPatcher::Start(path,"http://www.replicanet.com/Products/Demo1/index","http://www.replicanet.com/Products/Demo1",true);

	mWindowActive = true;
	return TRUE;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
//	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

static void DoError(const int error)
{
	char temp[1024];
	sprintf(temp,"Bootstrap Error $%x\n",error);
	MessageBox(0,temp,0,MB_OK);
	exit(-1);
}

void CMainDlg::CloseDialog(int nVal)
{
	mDialogControls.Lock();
	mWindowActive = false;
	mDialogControls.UnLock();
	ProductPatcher::Stop();
	DestroyWindow();
	::PostQuitMessage(nVal);

	// If the patch succeeded mExitStatus is set
	if ( mExitStatus == RNReplicaNet::RNLobby::ProductPatcher::kSuccess )
	{
		// Try to execute the product executable.
		char temp[MAX_PATH];
		strcpy(temp,pathForProduct);
		strcat(temp,"\\Demo1.exe");
		STARTUPINFO			si;
		PROCESS_INFORMATION pi;

		memset( &si, 0, sizeof(si) );
		si.cb = sizeof(si);

		// We create the new process
		BOOL ret2;
		if ( !(ret2 = CreateProcess( NULL,temp,NULL,NULL,FALSE,CREATE_NEW_PROCESS_GROUP,NULL,pathForProduct,&si,&pi )) )
		{
			DoError(0xbadc0de);
		}
	}
}

bool CMainDlg::CallbackUpdateProgressProduct(const size_t fileIndex,const size_t maxFileIndex)
{
	THREADSAFELOCKCLASS(mDialogControls);
	if (!mWindowActive)
	{
		return false;
	}
//	printf("CallbackUpdateProgressProduct %d:%d\n",fileIndex,maxFileIndex);
	mProductProgress.SetRange32(0,maxFileIndex);
	mProductProgress.SetPos(fileIndex);
	return true;
}

bool CMainDlg::CallbackUpdateDownloadPatch(const char *patchingFile,const size_t filePosition,const size_t fileLength)
{
	THREADSAFELOCKCLASS(mDialogControls);
	if (!mWindowActive)
	{
		return false;
	}
//	printf("CallbackUpdateDownloadPatch %s %d:%d\n",patchingFile,filePosition,fileLength);
	mFileProgress.SetRange32(0,fileLength);
	mFileProgress.SetPos(filePosition);

	mName.SetWindowTextA(patchingFile);
	return true;
}

bool CMainDlg::CallbackUpdateProgressFile(const char *patchingFile,const size_t filePosition,const size_t fileLength)
{
	THREADSAFELOCKCLASS(mDialogControls);
	if (!mWindowActive)
	{
		return false;
	}
//	printf("CallbackUpdateProgressFile %s %d:%d\n",patchingFile,filePosition,fileLength);
	mFileProgress.SetRange32(0,fileLength);
	mFileProgress.SetPos(filePosition);

	mName.SetWindowTextA(patchingFile);

	return true;
}

void CMainDlg::CallbackComplete(const Status status)
{
//	printf("CallbackComplete\n");
	// Here a timer is used to send a message from the patcher thread back to the main window thread and to allow this thread to complete.
	mDialogControls.Lock();
	mWindowActive = false;
	mDialogControls.UnLock();
	mExitStatus = status;
	SetTimer(1,1);
}

void CMainDlg::CallbackFailedToReadFile(const char *file)
{
//	printf("CallbackFailedToReadFile %s\n",file);
}

LRESULT CMainDlg::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Timer message received, this is our signal to close the dialog.
	KillTimer(1);
	CloseDialog(0);
	return 0;
}
