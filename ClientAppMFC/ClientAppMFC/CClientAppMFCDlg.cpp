// ClientAppMFCDlg.cpp

#include "pch.h"
#include "framework.h"
#include "CClientAppMFC.h"
#include "CClientAppMFCDlg.h"
#include "afxdialogex.h"
#include <cstdlib>
#include <ctime> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CONN_BUTTON_TEXT(b) ((b) ? _T("Disconnect") : _T("Connect"))

//*********************************************************
//*
//*********************************************************
CString generateUsername() {
	srand(time(nullptr));

	CString formattedNumber;
	formattedNumber.Format(_T("%03d"), rand() % 100 + 1);

	// Construct the username -> userXXX (XXX = 001 - 100)
	CString username = _T("user") + formattedNumber;

	return username;
}

// CClientAppMFCDlg  class
//*********************************************************
//*
//*********************************************************
CClientAppMFCDlg::CClientAppMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTAPPMFC_DIALOG, pParent)
{
	m_clientService.SetCallback([this](const std::string& message, bool status) {
		OnServiceMessage(message, status);
		});
}

//*********************************************************
//*
//*********************************************************
void CClientAppMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_editOutput);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_editInput);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUsername);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_connectButton);
}

BEGIN_MESSAGE_MAP(CClientAppMFCDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CClientAppMFCDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CClientAppMFCDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CClientAppMFCDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()

//*********************************************************
//*
//*********************************************************
BOOL CClientAppMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// IDM_ABOUTBOX check
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	m_editUsername.SetWindowText(generateUsername());
	m_editUsername.EnableWindow(FALSE);

	m_editInput.SetFocus();

	return TRUE;
}

//*********************************************************
//*
//*********************************************************
void CClientAppMFCDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	OnBnClickedButtonConnect();
}

//*********************************************************
//*
//*********************************************************
void CClientAppMFCDlg::OnServiceMessage(const std::string& message, bool status)
{
	if (status)
	{
		UpdateButtonConnectText();
		m_editUsername.EnableWindow(!m_clientService.IsRunning());
		UpdateOutputText(message);
	}
	else
	{
		UpdateOutputText(message);
	}
}

//*********************************************************
//*
//*********************************************************
void CClientAppMFCDlg::OnBnClickedButtonConnect()
{
	if (m_clientService.IsRunning())
	{
		m_clientService.Disconnect();
	}
	else
	{
		m_clientService.Connect();
	}
}

//*********************************************************
//*
//*********************************************************
void CClientAppMFCDlg::OnBnClickedButtonSend()
{
	CString currentUsername, currentInput;
	m_editUsername.GetWindowText(currentUsername);
	
	m_editInput.GetWindowText(currentInput);
	m_editInput.SetWindowText(L"");

	CString msg = currentUsername + ": " + currentInput;
	std::string str(CW2A(msg.GetString()));
	m_clientService.Send(str);
}

//*********************************************************
//*
//*********************************************************
void CClientAppMFCDlg::UpdateButtonConnectText()
{
	m_connectButton.SetWindowTextW(CONN_BUTTON_TEXT(m_clientService.IsRunning()));
}

//*********************************************************
//*
//*********************************************************
void CClientAppMFCDlg::UpdateOutputText(const std::string& message)
{
	CString msg(message.c_str());
	msg += _T("\r\n");

	CString currentText;
	m_editOutput.GetWindowText(currentText);

	int length = currentText.GetLength();
	m_editOutput.SetSel(length, length);

	// Replace the selection with the new text
	m_editOutput.ReplaceSel(msg);
}

//*********************************************************
//*
//*********************************************************
void CClientAppMFCDlg::OnOK()
{
	OnBnClickedButtonSend();
}

//*********************************************************
//*
//*********************************************************
void CClientAppMFCDlg::OnCancel()
{
	// Do Nothing, avoid dialog close on Esc button
}

//*********************************************************
//*
//*********************************************************
void CClientAppMFCDlg::OnBnClickedButtonClose()
{
	CDialogEx::OnCancel();
}
