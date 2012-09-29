// nettestDlg.h : header file
//

#include "afxwin.h"
#if !defined(AFX_NETTESTDLG_H__AF4E4C08_8377_4046_95C8_7BE2CB77289A__INCLUDED_)
#define AFX_NETTESTDLG_H__AF4E4C08_8377_4046_95C8_7BE2CB77289A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNettestDlg dialog

class CNettestDlg;

typedef struct
{
	int m_port;
	CString m_host;
	long m_numbytes;

	CNettestDlg * inst;

	SOCKET s;
	bool useTCP;

} threadctx;


class CNettestDlg : public CDialog
{
// Construction
public:
	void log(const char * src, int line, const char * msg);
	CNettestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNettestDlg)
	enum { IDD = IDD_NETTEST_DIALOG };
	CButton	m_killserverBtn;
	CButton	m_killclientBtn;
	CButton	m_serverBtn;
	CButton	m_clientBtn;
	CListBox	m_log;
	CString	m_host;
	int		m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNettestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CFont m_ffont;
	HANDLE m_clientThread, m_serverThread;
	threadctx * m_clientCtx, * m_serverCtx;

	HWND m_hTCP, m_hUDP;

	static DWORD WINAPI clientThread(LPVOID lpParm);
	static DWORD WINAPI serverThread(LPVOID lpParm);

	// Generated message map functions
	//{{AFX_MSG(CNettestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClient();
	afx_msg void OnServer();
	afx_msg void OnKillclient();
	afx_msg void OnKillserver();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_numbytes_ctl;
	CStatic m_numbyteslabel_ctl;
	long m_numbytes;
	afx_msg void OnBnClickedTcp();
	afx_msg void OnBnClickedUdp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETTESTDLG_H__AF4E4C08_8377_4046_95C8_7BE2CB77289A__INCLUDED_)
