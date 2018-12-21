#pragma once


// ImiDialogTest 对话框

class ImiDialogTest : public CDialog
{
	DECLARE_DYNAMIC(ImiDialogTest)

public:
	ImiDialogTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ImiDialogTest();

// 对话框数据
	enum { IDD = IDD_IMIDIALOGTEST };
	CString text;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

};
