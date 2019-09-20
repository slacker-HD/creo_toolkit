#pragma once


// CMyEbdDialog 对话框

class CMyEbdDialog : public CDialog
{
	DECLARE_DYNAMIC(CMyEbdDialog)

public:
	CMyEbdDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyEbdDialog();

// 对话框数据
	enum { IDD = IDD_EBDDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnMcnSelectMonthcalendar(NMHDR *pNMHDR, LRESULT *pResult);
	CMonthCalCtrl m_MonthCalCtrl;
	CEdit m_Edit;
	void SetEdit(CString Value);
};
