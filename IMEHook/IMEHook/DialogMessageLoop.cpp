// DialogMessageLoop.cpp : 实现文件
//

#include "stdafx.h"
#include "IMEHook.h"
#include "DialogMessageLoop.h"
#include "afxdialogex.h"

// CDialogMessageLoop 对话框

IMPLEMENT_DYNAMIC(CDialogMessageLoop, CDialog)

CDialogMessageLoop::CDialogMessageLoop(CWnd *pParent /*=NULL*/)
	: CDialog(CDialogMessageLoop::IDD, pParent)
{
}

CDialogMessageLoop::~CDialogMessageLoop()
{
}

void CDialogMessageLoop::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogMessageLoop, CDialog)
ON_WM_TIMER()
END_MESSAGE_MAP()

// CDialogMessageLoop 消息处理程序

BOOL CDialogMessageLoop::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(1, 1, NULL);

	return TRUE; // return TRUE unless you set the focus to a control
				 // 异常: OCX 属性页应返回 FALSE
}

void CDialogMessageLoop::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)

	{
		KillTimer(1);
		OnOK();
	}
	CDialog::OnTimer(nIDEvent);
}
