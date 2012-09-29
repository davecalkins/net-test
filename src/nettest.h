// nettest.h : main header file for the NETTEST application
//

#if !defined(AFX_NETTEST_H__E5A250DC_CA65_4E85_A4EA_5B17C68CA853__INCLUDED_)
#define AFX_NETTEST_H__E5A250DC_CA65_4E85_A4EA_5B17C68CA853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNettestApp:
// See nettest.cpp for the implementation of this class
//

class CNettestApp : public CWinApp
{
public:
	CNettestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNettestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNettestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETTEST_H__E5A250DC_CA65_4E85_A4EA_5B17C68CA853__INCLUDED_)
