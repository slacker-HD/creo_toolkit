// MyEbdDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "originview.h"
#include "MyEbdDialog.h"
#include "afxdialogex.h"


// CMyEbdDialog 对话框

IMPLEMENT_DYNAMIC(CMyEbdDialog, CDialog)

CMyEbdDialog::CMyEbdDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMyEbdDialog::IDD, pParent)
{

}

CMyEbdDialog::~CMyEbdDialog()
{
}

void CMyEbdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHCALENDAR, m_MonthCalCtrl);
	DDX_Control(pDX, IDC_EDIT, m_Edit);
}


BEGIN_MESSAGE_MAP(CMyEbdDialog, CDialog)
	ON_NOTIFY(MCN_SELECT, IDC_MONTHCALENDAR, &CMyEbdDialog::OnMcnSelectMonthcalendar)
END_MESSAGE_MAP()


// CMyEbdDialog 消息处理程序


void CMyEbdDialog::OnOK()
{
	//CDialog::OnOK();
}


void CMyEbdDialog::OnCancel()
{
	//CDialog::OnCancel();
}


void CMyEbdDialog::OnMcnSelectMonthcalendar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	CTime month;
	m_MonthCalCtrl.GetCurSel(month);
	//AfxMessageBox(month.Format("%Y-%m-%d"));
	CalendarSet(month.Format("%Y-%m-%d"));
	*pResult = 0;
}


void CMyEbdDialog::SetEdit(CString Value)
{
	m_Edit.SetWindowTextW(Value);
}
