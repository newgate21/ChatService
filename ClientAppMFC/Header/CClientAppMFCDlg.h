// ClientAppMFCDlg.h: file di intestazione
#pragma once
#include "CClientService.h"

class CClientAppMFCDlg : public CDialogEx
{
public:
	CClientAppMFCDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTAPPMFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	// Callback function
	void OnServiceMessage(const std::string& message, bool status);

	void UpdateButtonConnectText();
	void UpdateOutputText(const std::string& message);

	//{ AFX_MSG
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonClose();

	DECLARE_MESSAGE_MAP()
	// AFX_MSG }

private:
	CEdit m_editOutput, m_editInput, m_editUsername;
	CButton m_connectButton;
	CClientService m_clientService;
};
