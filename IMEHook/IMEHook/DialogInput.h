#pragma once

// CDialogInput 对话框

class CDialogInput : public CDialog
{
	DECLARE_DYNAMIC(CDialogInput)

public:
	CDialogInput(CWnd *pParent = NULL); // 标准构造函数
	virtual ~CDialogInput();

	// 对话框数据
	enum
	{
		IDD = IDD_DIALOGINPUT
	};

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
