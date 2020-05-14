// FrmReplacer.h : FrmReplacer DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

class CFrmReplacerApp : public CWinApp
{
public:
	CFrmReplacerApp();
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

void GetFrmlist(CString Path, CArray<CString, CString &> *lst);
void ReplaceFrm(CString Drw,int SheetNum,CString Frm,bool DelTab);


typedef struct CCurrentSheet
{
    CString Drw;
    CString Format;
	int SheetNum;
}CURRENTSHT;