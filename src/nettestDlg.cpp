// nettestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nettest.h"
#include "nettestDlg.h"
#include ".\nettestdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UDP_NUM_BYTES 100000

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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
// CNettestDlg dialog

CNettestDlg::CNettestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNettestDlg::IDD, pParent)
	, m_numbytes(0)
{
	//{{AFX_DATA_INIT(CNettestDlg)
	m_host = _T("");
	m_port = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_clientThread = 0;
	m_serverThread = 0;

	m_clientCtx = 0;
	m_serverCtx = 0;

	m_hTCP = m_hUDP = 0;
}

void CNettestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNettestDlg)
	DDX_Control(pDX, IDC_KILLSERVER, m_killserverBtn);
	DDX_Control(pDX, IDC_KILLCLIENT, m_killclientBtn);
	DDX_Control(pDX, IDC_SERVER, m_serverBtn);
	DDX_Control(pDX, IDC_CLIENT, m_clientBtn);
	DDX_Control(pDX, IDC_LOG, m_log);
	DDX_Text(pDX, IDC_HOST, m_host);
	DDX_Text(pDX, IDC_PORT, m_port);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_NUMBYTES, m_numbytes_ctl);
	DDX_Control(pDX, IDC_NUMBYTES_LABEL, m_numbyteslabel_ctl);
	DDX_Text(pDX, IDC_NUMBYTES, m_numbytes);
}

BEGIN_MESSAGE_MAP(CNettestDlg, CDialog)
	//{{AFX_MSG_MAP(CNettestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CLIENT, OnClient)
	ON_BN_CLICKED(IDC_SERVER, OnServer)
	ON_BN_CLICKED(IDC_KILLCLIENT, OnKillclient)
	ON_BN_CLICKED(IDC_KILLSERVER, OnKillserver)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TCP, OnBnClickedTcp)
	ON_BN_CLICKED(IDC_UDP, OnBnClickedUdp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNettestDlg message handlers

BOOL CNettestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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
	
	// TODO: Add extra initialization here
	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	sprintf(lf.lfFaceName,"Courier New");
	m_ffont.CreateFontIndirect(&lf);
	m_log.SetFont(&m_ffont);

	m_killclientBtn.EnableWindow(FALSE);
	m_killserverBtn.EnableWindow(FALSE);

	m_hTCP = ::GetDlgItem(GetSafeHwnd(),IDC_TCP);
	m_hUDP = ::GetDlgItem(GetSafeHwnd(),IDC_UDP);
	::SendMessage(m_hTCP,BM_SETCHECK,BST_CHECKED,0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNettestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNettestDlg::OnPaint() 
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

HBRUSH CNettestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	hbr = (HBRUSH)::GetStockObject(BLACK_BRUSH);

	pDC->SetTextColor(RGB(0,255,0));
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNettestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNettestDlg::log(const char *src, int line, const char *msg)
{
	CTime t = CTime::GetCurrentTime();
	CString ts = t.Format("%H:%M:%S");

	CString z;
	z.Format("%s-%s(%d): %s", ts, src, line, msg);
	m_log.AddString(z);

	m_log.SetTopIndex(m_log.GetCount()-1);
}

#define LOG(x,y) ctx->inst->log((x),__LINE__,(y))
#define CLOG(x) LOG("CLIENT",(x))
#define SLOG(x) LOG("SERVER",(x))
#define MLOG(x) log("MAIN",__LINE__,(x))

void CNettestDlg::OnClient() 
{
	UpdateData(TRUE);
	m_clientBtn.EnableWindow(FALSE);
	::EnableWindow(m_hTCP,FALSE);
	::EnableWindow(m_hUDP,FALSE);

	MLOG("starting client");

	threadctx * ctx = new threadctx;
	ctx->m_port = m_port;
	ctx->m_host = m_host;
	ctx->m_numbytes = m_numbytes;
	ctx->inst = this;
	ctx->s = 0;
	ctx->useTCP = (::SendMessage(m_hTCP,BM_GETCHECK,0,0) == BST_CHECKED);
	m_clientCtx = ctx;

	DWORD threadID;
	m_clientThread = CreateThread(0,0,clientThread,(LPVOID)ctx,0,&threadID);
	m_killclientBtn.EnableWindow(TRUE);
}

void CNettestDlg::OnServer() 
{
	UpdateData(TRUE);
	m_serverBtn.EnableWindow(FALSE);
	::EnableWindow(m_hTCP,FALSE);
	::EnableWindow(m_hUDP,FALSE);

	MLOG("starting server");

	threadctx * ctx = new threadctx;
	ctx->m_port = m_port;
	ctx->m_host = m_host;
	ctx->m_numbytes = m_numbytes;
	ctx->inst = this;
	ctx->s = 0;
	ctx->useTCP = (::SendMessage(m_hTCP,BM_GETCHECK,0,0) == BST_CHECKED);
	m_serverCtx = ctx;

	DWORD threadID;
	m_serverThread = CreateThread(0,0,serverThread,(LPVOID)ctx,0,&threadID);
	m_killserverBtn.EnableWindow(TRUE);
}

void CNettestDlg::OnKillclient() 
{
	m_killclientBtn.EnableWindow(FALSE);

	if (m_clientThread != 0)
	{
		TerminateThread(m_clientThread,0);
		m_clientThread = 0;
	}

	if (m_clientCtx != 0)
	{
		closesocket(m_clientCtx->s);
		m_clientCtx = 0;
	}

	m_clientBtn.EnableWindow(TRUE);

	if (m_serverThread == 0)
	{
		::EnableWindow(m_hTCP,TRUE);
		::EnableWindow(m_hUDP,TRUE);
	}

	MLOG("killed client");
}

void CNettestDlg::OnKillserver() 
{
	m_killserverBtn.EnableWindow(FALSE);

	if (m_serverThread != 0)
	{
		TerminateThread(m_serverThread,0);
		m_serverThread = 0;
	}

	if (m_serverCtx != 0)
	{
		closesocket(m_serverCtx->s);
		m_clientCtx = 0;
	}

	if (m_clientThread == 0)
	{
		::EnableWindow(m_hTCP,TRUE);
		::EnableWindow(m_hUDP,TRUE);
	}

	m_serverBtn.EnableWindow(TRUE);

	MLOG("killed server");
}

DWORD WINAPI CNettestDlg::clientThread(LPVOID lpParm)
{
	threadctx * ctx = (threadctx *)lpParm;

	if ((ctx->m_host.GetLength() > 0) && (ctx->m_port > 0))
	{
		hostent * he = 0;
		if (isdigit(ctx->m_host[0]) && strchr(ctx->m_host,'.'))
		{
			unsigned long iaddr = inet_addr(ctx->m_host);
			if (iaddr != INADDR_NONE)
			{
				he = gethostbyaddr((const char *)(&iaddr),sizeof(iaddr),AF_INET);
			}
			else
			{
				CLOG("invalid IP address");
			}
		}
		else
		{
			he = gethostbyname(ctx->m_host);
		}

		if (he != 0)
		{
			CString z;

			if (ctx->useTCP)
			{
				z.Format("connecting to %s:%d", ctx->m_host, ctx->m_port);
				CLOG(z);
			}
			else
			{
				z.Format("sending to %s:%d", ctx->m_host, ctx->m_port);
				CLOG(z);
			}

			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons((u_short)ctx->m_port);
			memcpy(&addr.sin_addr,he->h_addr_list[0],sizeof(in_addr));

			SOCKET s = INVALID_SOCKET;

			if (ctx->useTCP)
			{
				s = socket(AF_INET,SOCK_STREAM,0);
			}
			else
			{
				s = socket(AF_INET,SOCK_DGRAM,0);
			}

			if (s != INVALID_SOCKET)
			{
				ctx->s = s;

				if (ctx->useTCP)
				{
					if (connect(s,(sockaddr *)&addr, sizeof(sockaddr)) == 0)
					{
						CLOG("connected");
					}
					else
					{
						CLOG("failed to connect");
					}
				}
				else
				{
					char * buf = new char[ctx->m_numbytes];

					int bytesSent = sendto(s,buf,ctx->m_numbytes,0,(sockaddr*)&addr,sizeof(sockaddr));

					if ((bytesSent != SOCKET_ERROR) && (bytesSent == ctx->m_numbytes))
					{
						CLOG("successfully sent data");
					}
					else
					{
						CLOG("failed to send data");
					}

					delete [] buf;
				}

				closesocket(s);
				ctx->s = -1;
				CLOG("socket closed");
			}
			else
			{
				CLOG("unable to create the socket");
			}
		}
		else
		{
			CLOG("invalid host");
		}
	}
	else
	{
		CLOG("please provide a host and port");
	}

	ctx->inst->m_killclientBtn.EnableWindow(FALSE);
	ctx->inst->m_clientBtn.EnableWindow(TRUE);
	ctx->inst->m_clientThread = 0;

	if (ctx->inst->m_serverThread == 0)
	{
		::EnableWindow(ctx->inst->m_hTCP,TRUE);
		::EnableWindow(ctx->inst->m_hUDP,TRUE);
	}

	delete ctx;
	return 0;
}

DWORD WINAPI CNettestDlg::serverThread(LPVOID lpParm)
{
	threadctx * ctx = (threadctx *)lpParm;

	SLOG("running as a server");

	SOCKET s = INVALID_SOCKET;

	if (ctx->useTCP)
	{
		s = socket(AF_INET,SOCK_STREAM,0);
	}
	else
	{
		s = socket(AF_INET,SOCK_DGRAM,0);
	}

	if (s != INVALID_SOCKET)
	{
		ctx->s = s;

		BOOL b = true;
		setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char *)&b,sizeof(BOOL));

		sockaddr_in addr;

		addr.sin_family = AF_INET;
		addr.sin_port = htons((u_short)ctx->m_port);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		
		if (bind(s,(sockaddr*)&addr,sizeof(addr)) == 0)
		{
			CString z;
			z.Format("bound server socket to port %d", ctx->m_port);
			SLOG(z);

			if (ctx->useTCP)
			{
				if (listen(s,SOMAXCONN) == 0)
				{
					SLOG("listening for incoming connections");

					sockaddr_in clientaddr;
					int clientaddr_len = sizeof(clientaddr);

					SOCKET cs;

					do
					{
						cs = accept(s,(sockaddr*)&clientaddr,&clientaddr_len);

						if (cs != INVALID_SOCKET)
						{
							SLOG("accepted client connection");

							closesocket(cs);
							SLOG("closed client socket");
						}
						else
						{
							SLOG("accept() failed");
						}

					} while (cs != INVALID_SOCKET);
				}
				else
				{
					SLOG("listen() failed");
				}
			}
			else
			{
				while(true)
				{
					char * buf = new char[ctx->m_numbytes];

					sockaddr_in fromaddr;
					int fromaddr_size = sizeof(fromaddr);

					int bytesRead = recvfrom(s,buf,ctx->m_numbytes,0,(sockaddr*)&fromaddr,&fromaddr_size);

					if ((bytesRead != SOCKET_ERROR) && (bytesRead == ctx->m_numbytes))
					{
						SLOG("received data from client");
					}
					else
					{
						SLOG("recvfrom() failed");
					}

					delete [] buf;
				}
			}
		}
		else
		{
			SLOG("unable to bind to specified port");
		}

		closesocket(s);
		ctx->s = -1;
		SLOG("socket closed");
	}
	else
	{
		SLOG("unable to create socket");
	}

	ctx->inst->m_killserverBtn.EnableWindow(FALSE);
	ctx->inst->m_serverBtn.EnableWindow(TRUE);
	ctx->inst->m_serverThread = 0;

	if (ctx->inst->m_clientThread == 0)
	{
		::EnableWindow(ctx->inst->m_hTCP,TRUE);
		::EnableWindow(ctx->inst->m_hUDP,TRUE);
	}

	delete ctx;
	return 0;
}

void CNettestDlg::OnBnClickedTcp()
{
	m_numbytes_ctl.ShowWindow(SW_HIDE);
	m_numbyteslabel_ctl.ShowWindow(SW_HIDE);
}

void CNettestDlg::OnBnClickedUdp()
{
	m_numbytes_ctl.ShowWindow(SW_SHOW);
	m_numbyteslabel_ctl.ShowWindow(SW_SHOW);
}
