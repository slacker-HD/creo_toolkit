// ImiDialogTest.cpp : 实现文件
//

#include "stdafx.h"
#include "ImiProTKDLl.h"
#include "ImiDialogTest.h"
#include "afxdialogex.h"

// ImiDialogTest 对话框

IMPLEMENT_DYNAMIC(ImiDialogTest, CDialog)

ImiDialogTest::ImiDialogTest(CWnd *pParent /*=NULL*/)
	: CDialog(ImiDialogTest::IDD, pParent)
{
}

ImiDialogTest::~ImiDialogTest()
{
}

void ImiDialogTest::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ImiDialogTest, CDialog)

ON_BN_CLICKED(IDC_BUTTON1, &ImiDialogTest::OnBnClickedButton1)
END_MESSAGE_MAP()

// ImiDialogTest 消息处理程序

void ImiDialogTest::OnBnClickedButton1()
{
	AfxMessageBox(_T("点击了按钮"));
}