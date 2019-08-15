// MyDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "unicodechineseinput.h"
#include "MyDialog.h"

#define WM_MY_ACTIVE WM_USER+100
#define WM_MY_INACTIVE WM_USER+101

// CMyDialog 对话框

IMPLEMENT_DYNAMIC(CMyDialog, CDialog)

	CMyDialog::CMyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDialog::IDD, pParent)
{
	_isMessageHook=false;
}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CMyDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMyDialog::OnBnClickedCancel)
	ON_MESSAGE(WM_MY_ACTIVE,&CMyDialog::OnMyMessageloop)
	ON_MESSAGE(WM_MY_INACTIVE,&CMyDialog::OnMyKillMessageloop)
	ON_WM_ACTIVATE()
	ON_EN_SETFOCUS(IDC_EDIT1, &CMyDialog::OnEnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CMyDialog::OnKillfocusEdit1)
	ON_WM_CLOSE()
	ON_CBN_KILLFOCUS(IDC_COMBO1, &CMyDialog::OnCbnKillfocusCombo1)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CMyDialog::OnCbnSetfocusCombo1)
END_MESSAGE_MAP()

// CMyDialog 消息处理程序

void CMyDialog::OnBnClickedOk()
{
	DestroyWindow();
}

void CMyDialog::OnBnClickedCancel()
{
	DestroyWindow();
}

void CMyDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	switch(nState)
	{
	case WA_CLICKACTIVE:
	case WA_ACTIVE:
		TRACE(_T("dialog actived\n"));
		break;
	case WA_INACTIVE:
		PostMessage(WM_MY_INACTIVE,0,0);
		break;
	default:
		break;
	}
	if(_isMessageHook)
		PostMessage(WM_MY_INACTIVE,0,0);
	this->GetDlgItem(IDOK)->SetFocus();
}

void CMyDialog::OnClose()
{
	PostMessage(WM_MY_INACTIVE,0,0);
	CDialog::OnClose();
}

LRESULT CMyDialog::OnMyMessageloop(WPARAM wParam,LPARAM lParam)
{
	if(!_isMessageHook)
	{
		_isMessageHook = true;
		MSG msg;
		while (GetMessage(&msg,NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	_isMessageHook= false;
	return true;
}

LRESULT CMyDialog::OnMyKillMessageloop(WPARAM wParam,LPARAM lParam)
{
	_isMessageHook = false;
	PostQuitMessage(0);
	return true;
}

void CMyDialog::OnEnSetfocusEdit1()
{
	TRACE(_T("text focus\n"));
	PostMessage(WM_MY_ACTIVE,0,0);
}

void CMyDialog::OnKillfocusEdit1()
{
	TRACE(_T("quit focus\n"));
	PostMessage(WM_MY_INACTIVE,0,0);
}

void CMyDialog::OnCbnSetfocusCombo1()
{
	TRACE(_T("text focus\n"));
	PostMessage(WM_MY_ACTIVE,0,0);
}

void CMyDialog::OnCbnKillfocusCombo1()
{
	TRACE(_T("quit focus\n"));
	PostMessage(WM_MY_INACTIVE,0,0);
}