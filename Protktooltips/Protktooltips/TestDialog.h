#pragma once

// CTestDialog 对话框

class CTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CTestDialog)

  public:
	CTestDialog(CWnd *pParent = NULL); // 标准构造函数
	virtual ~CTestDialog();

	// 对话框数据
	enum
	{
		IDD = IDD_DIALOGTESTTOOLTIPS
	};

  protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
  public:
	virtual BOOL OnInitDialog();
	CToolTipCtrl m_ToolTipCtrl;
};
