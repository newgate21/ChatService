// CClientAppMFC.cpp

#include "pch.h"
#include "framework.h"
#include "CClientAppMFC.h"
#include "CClientAppMFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientAppMFCApp
//*********************************************************
//*
//*********************************************************
CClientAppMFCApp::CClientAppMFCApp()
{
    // Restart Manager Support
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

BEGIN_MESSAGE_MAP(CClientAppMFCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// App Object CClientAppMFCApp
CClientAppMFCApp theApp;

//*********************************************************
//*
//*********************************************************
BOOL CClientAppMFCApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Attiva gestore visualizzazione "Windows nativo" per abilitare i temi nei controlli MFC
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	CClientAppMFCDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal(); // Execution of Dialog App
	if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Avviso: creazione della finestra di dialogo non riuscita. L'applicazione verrà terminata in modo imprevisto.\n");
		TRACE(traceAppMsg, 0, "Avviso: se si utilizzano i controlli MFC nella finestra di dialogo, non è possibile utilizzare #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	return FALSE;
}

