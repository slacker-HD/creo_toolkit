// TestDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Protktooltips.h"
#include "TestDialog.h"
#include "afxdialogex.h"

// CTestDialog 对话框

IMPLEMENT_DYNAMIC(CTestDialog, CDialog)

CTestDialog::CTestDialog(CWnd *pParent /*=NULL*/)
	: CDialog(CTestDialog::IDD, pParent)
{
}

CTestDialog::~CTestDialog()
{
}

void CTestDialog::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDialog, CDialog)
END_MESSAGE_MAP()

// CTestDialog 消息处理程序

BOOL CTestDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUTTONTEST), _T("这是测试TOOLTIPS字符串"));
	m_ToolTipCtrl.SetMaxTipWidth(300);
	m_ToolTipCtrl.Activate(TRUE);

	return TRUE;
}
