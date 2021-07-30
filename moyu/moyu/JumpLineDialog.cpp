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
	, m_currentlinenumEdit(0)
{

}

CJumpLineDialog::~CJumpLineDialog()
{
}

void CJumpLineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITLINENUM, m_currentlinenumEdit);
	DDV_MinMaxInt(pDX, m_currentlinenumEdit, 1, 2000);
}

BEGIN_MESSAGE_MAP(CJumpLineDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CJumpLineDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CJumpLineDialog 消息处理程序


BOOL CJumpLineDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_currentlinenumEdit = CurrentLineNum>2000? 2000:CurrentLineNum;
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CJumpLineDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}
