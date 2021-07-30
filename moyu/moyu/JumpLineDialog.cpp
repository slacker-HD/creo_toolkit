// JumpLineDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "moyu.h"
#include "JumpLineDialog.h"
#include "afxdialogex.h"


// CJumpLineDialog 对话框

IMPLEMENT_DYNAMIC(CJumpLineDialog, CDialog)

CJumpLineDialog::CJumpLineDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CJumpLineDialog::IDD, pParent)
	, m_LineNumEdit(0)
{

}

CJumpLineDialog::~CJumpLineDialog()
{
}

void CJumpLineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITLINENUM, m_LineNumEdit);
	DDV_MinMaxInt(pDX, m_LineNumEdit, 1, 2000);
}


BEGIN_MESSAGE_MAP(CJumpLineDialog, CDialog)
END_MESSAGE_MAP()


// CJumpLineDialog 消息处理程序
