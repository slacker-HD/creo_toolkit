// CreoAccess.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CreoAccess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCreoAccessApp, CWinApp)
END_MESSAGE_MAP()

CCreoAccessApp::CCreoAccessApp()
{
}

CCreoAccessApp theApp;

BOOL CCreoAccessApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

_ConnectionPtr m_pConnection;

void ReadAccess()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_RecordsetPtr m_pRecordset;
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	_variant_t value;
	try
	{
		m_pRecordset->Open(_variant_t("SELECT test FROM Tab WHERE ID=1"), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		_variant_t var = (long)0;
		value = m_pRecordset->GetCollect(var);
		AfxMessageBox(CString(value));
		m_pRecordset->Close();
	}
	catch (_com_error e)
	{
		CATCH_ERROR;
	}
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId CreoAccessID;
	CString CSpath;
	ProPath ExecPath;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxOleInit();
	m_pConnection.CreateInstance(__uuidof(Connection));

	status = ProMenubarMenuAdd("CreoAccess", "CreoAccess", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("CreoAccess", "CreoAccess", "CreoAccess", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("CreoAccess_Act", (uiCmdCmdActFn)ReadAccess, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &CreoAccessID);
	status = ProMenubarmenuPushbuttonAdd("CreoAccess", "CreoAccessmenu", "CreoAccessmenu", "CreoAccessmenutips", NULL, PRO_B_TRUE, CreoAccessID, MSGFILE);

	status = ProToolkitApplExecPathGet(ExecPath);
	CSpath = CString(ExecPath);
	CSpath = CSpath.Left(CSpath.GetLength() - 14);
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection"); ///创建Connection对象
		m_pConnection->ConnectionTimeout = 3;			  ///设置超时时间为3秒
		CSpath = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + CSpath + "test.accdb;";
		//CSpath = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source="+CSpath+"test.mdb;";

		m_pConnection->Open((_bstr_t)CSpath, "", "", adModeUnknown);
	}
	catch (_com_error e) ///捕捉异常
	{
		CATCH_ERROR;
		return 0;
	}

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_pConnection->State)
		m_pConnection->Close();
}