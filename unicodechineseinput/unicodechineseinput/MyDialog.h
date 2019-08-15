#pragma once

// CMyDialog 对话框

class CMyDialog : public CDialog
{
	DECLARE_DYNAMIC(CMyDialog)

public:
	CMyDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEnSetfocusEdit1();
	afx_msg LRESULT OnMyMessageloop(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMyKillMessageloop(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnClose();
	afx_msg void OnCbnKillfocusCombo1();
	afx_msg void OnCbnSetfocusCombo1();

private:
	bool _isMessageHook;


};
