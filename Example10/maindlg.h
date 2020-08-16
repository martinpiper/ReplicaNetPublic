// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__B406ACDD_0812_4D21_930F_698797BA1792__INCLUDED_)
#define AFX_MAINDLG_H__B406ACDD_0812_4D21_930F_698797BA1792__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RNLobby/Inc/ProductPatcher.h"

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler , public RNReplicaNet::RNLobby::ProductPatcher
{
public:
	CMainDlg();
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);

private:
	RNReplicaNet::MutexClass mDialogControls;
	volatile bool mWindowActive;
	bool CallbackUpdateProgressProduct(const size_t fileIndex,const size_t maxFileIndex);

	bool CallbackUpdateDownloadPatch(const char *patchingFile,const size_t filePosition,const size_t fileLength);

	bool CallbackUpdateProgressFile(const char *patchingFile,const size_t filePosition,const size_t fileLength);

	void CallbackComplete(const Status status);

	void CallbackFailedToReadFile(const char *file);

	CProgressBarCtrl mProductProgress;
	CProgressBarCtrl mFileProgress;
	CStatic mName;
	volatile RNReplicaNet::RNLobby::ProductPatcher::Status mExitStatus;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__B406ACDD_0812_4D21_930F_698797BA1792__INCLUDED_)
