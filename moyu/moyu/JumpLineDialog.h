#pragma once

// CJumpLineDialog 对话框

class CJumpLineDialog : public CDialog
{
	DECLARE_DYNAMIC(CJumpLineDialog)

public:
	CJumpLineDialog(CWnd *pParent = NULL); // 标准构造函数
	virtual ~CJumpLineDialog();

	// 对话框数据
	enum
	{
		IDD = IDD_DIALOGJUMPLINE
	};
	int MaxLineNum;
	int CurrentLineNum;

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_currentlinenumEdit;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
