// DialogInput.cpp : 实现文件
//

#include "stdafx.h"
#include "IMEHook.h"
#include "DialogInput.h"
#include "afxdialogex.h"

// CDialogInput 对话框

IMPLEMENT_DYNAMIC(CDialogInput, CDialog)

CDialogInput::CDialogInput(CWnd *pParent /*=NULL*/)
	: CDialog(CDialogInput::IDD, pParent)
{
}

CDialogInput::~CDialogInput()
{
}

void CDialogInput::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogInput, CDialog)
END_MESSAGE_MAP()

// CDialogInput 消息处理程序
