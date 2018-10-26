#include "stdafx.h"
#include "TimerSaver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTimerSaverApp, CWinApp)
END_MESSAGE_MAP()

CTimerSaverApp::CTimerSaverApp()
{
}

CTimerSaverApp theApp;

BOOL CTimerSaverApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}

int minute = 10;
ProBoolean saveAll = PRO_B_FALSE;
ProBoolean autoSave = PRO_B_FALSE;

ProError ProCollect2ParamDBVisitAction(void *p_object, ProAppData app_data)
{
	ProError status;
	ProArray *p_array;
	p_array = (ProArray *)((void **)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_object);
	return (status);
}

ProError ProCollect2ParamIdVisitAction(int id, ProAppData app_data)
{
	return (ProCollect2ParamDBVisitAction((void *)&id, app_data));
}

ProError ProCollectWindowIds(int **p_winid)
{
	ProError status;
	if (p_winid != NULL)
	{
		status = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)p_winid);
		if (status == PRO_TK_NO_ERROR)
		{
			status = ProWindowsVisit((ProWindowVisitAction)ProCollect2ParamIdVisitAction, (ProAppData)&p_winid);
			if (status != PRO_TK_NO_ERROR)
			{
				ProArrayFree((ProArray *)p_winid);
				*p_winid = NULL;
			}
		}
	}
	else
		status = PRO_TK_BAD_INPUTS;
	return status;
}

void SaveMdl(BOOL saveall)
{
	ProError status;
	int *p_array = NULL;
	int i, ArraySize = 0;
	ProMdl mdl;
	int CurrentWid;
	ProBoolean save_allowed;
	status = ProWindowCurrentGet(&CurrentWid);
	if (saveall)
	{
		status = ProCollectWindowIds(&p_array);
		status = ProArraySizeGet(p_array, &ArraySize);
		for (i = 0; i < ArraySize; i++)
		{
			if (CurrentWid != p_array[i])
			{
				status = ProWindowMdlGet(p_array[i], &mdl);
				if (status != PRO_TK_NO_ERROR)
					return;
				status = ProMdlIsSaveAllowed(mdl, PRO_B_TRUE, &save_allowed);
				if (status != PRO_TK_NO_ERROR)
				{
					status = ProArrayFree((ProArray *)&p_array);
					return;
				}
				if (save_allowed == PRO_B_TRUE)
					status = ProMdlSave(mdl);
			}
		}
		status = ProArrayFree((ProArray *)&p_array);
	}
	ProMacroLoad(L"~ Command `ProCmdModelSave` ;~ Activate `file_saveas` `OK`;");
	status = ProMessageDisplay(MSGFILE, "AutoSaved");
}

static VOID CALLBACK SaveProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if (autoSave)
	{
		SaveMdl(saveAll);
	}
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

ProError ShowDialog(wchar_t *Message)
{
	ProUIMessageButton *buttons;
	ProUIMessageButton user_choice;
	ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
	buttons[0] = PRO_UI_MESSAGE_OK;
	ProUIMessageDialogDisplay(PROUIMESSAGE_INFO, L"提示", Message, buttons, PRO_UI_MESSAGE_OK, &user_choice);
	ProArrayFree((ProArray *)&buttons);
	return PRO_TK_NO_ERROR;
}

void about()
{
	ShowDialog(_T("访问我的博客获得更多信息：\nhttp://www.hudi.site"));
}

void CommitOK()
{
	ProError status;
	wchar_t *value;
	int ntime;

	status = ProUIInputpanelValueGet("TimerSaver", "InputTime", &value);
	ntime = _wtoi(value);
	if (ntime >= 5 && ntime <= 60)
	{
		minute = ntime;
		status = ProUICheckbuttonGetState("TimerSaver", "CBAutoSave", &autoSave);
		status = ProUICheckbuttonGetState("TimerSaver", "CBSaveAll", &saveAll);
	}
	else
	{
		ShowDialog(_T("时间范围必须设定在5~60分钟以内。"));
		return;
	}
	status = ProWstringFree(value);
	SetTimer(NULL, TIMERID, minute * 60000, (TIMERPROC)SaveProc);
	status = ProUIDialogExit("TimerSaver", 1);
}

void CommitCancel()
{
	ProError status;
	SetTimer(NULL, TIMERID, minute * 60000, (TIMERPROC)SaveProc);
	status = ProUIDialogExit("TimerSaver", 0);
}

void ShowConfig()
{
	ProError status;
	wchar_t text[5];
	KillTimer(NULL, TIMERID);
	status = ProUIDialogCreate("TimerSaver", "TimerSaver");
	status = ProUIInputpanelInputtypeSet("TimerSaver", "InputTime", PROUIINPUTTYPE_INTEGER);
	if (autoSave)
		status = ProUICheckbuttonSet("TimerSaver", "CBAutoSave");
	if (saveAll)
		status = ProUICheckbuttonSet("TimerSaver", "CBSaveAll");
	_itow_s(minute, text, 5, 10);
	status = ProUIInputpanelValueSet("TimerSaver", "InputTime", text);

	status = ProUIPushbuttonActivateActionSet("TimerSaver", "CommitOK", ProUIAction(CommitOK), NULL);
	status = ProUIPushbuttonActivateActionSet("TimerSaver", "CommitCancel", ProUIAction(CommitCancel), NULL);

	status = ProUIDialogActivate("TimerSaver", NULL);
	status = ProUIDialogDestroy("TimerSaver");
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId TImerSaveID, AboutId;

	status = ProMenubarMenuAdd("Time", "Time", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("Time", "Time", "Time", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("TImerSave_Act", (uiCmdCmdActFn)ShowConfig, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &TImerSaveID);
	status = ProMenubarmenuPushbuttonAdd("Time", "TImerSave", "TImerSave", "TImerSavetips", NULL, PRO_B_TRUE, TImerSaveID, MSGFILE);

	status = ProCmdActionAdd("About_Act", (uiCmdCmdActFn)about, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AboutId);
	status = ProMenubarmenuPushbuttonAdd("Time", "About", "About", "Abouttips", NULL, PRO_B_TRUE, AboutId, MSGFILE);

	SetTimer(NULL, TIMERID, 60000, (TIMERPROC)SaveProc);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
	KillTimer(NULL, TIMERID);
}