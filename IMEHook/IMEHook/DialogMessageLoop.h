#pragma once

// CDialogMessageLoop 对话框

class CDialogMessageLoop : public CDialog
{
	DECLARE_DYNAMIC(CDialogMessageLoop)

public:
	CDialogMessageLoop(CWnd *pParent = NULL); // 标准构造函数
	virtual ~CDialogMessageLoop();

	// 对话框数据
	enum
	{
		IDD = IDD_DIALOGFORMESSAGELOOP
	};

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
