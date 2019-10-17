#include "stdafx.h"
#include "ColorPaint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CColorPaintApp, CWinApp)
END_MESSAGE_MAP()

CColorPaintApp::CColorPaintApp()
{
}

CColorPaintApp theApp;

BOOL CColorPaintApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}

typedef enum _hint
{
	Fun = 0,   
	About = 1,
} HINT;
HINT hint;

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

int CurrentMdlType()
{
	ProMdl mdl;
	ProMdlType mdltype;
	ProError status;
	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return -1;
	status = ProMdlTypeGet(mdl, &mdltype);
	if (status != PRO_TK_NO_ERROR)
		return -1;
	else
		return mdltype;
}

static uiCmdAccessState AccessAssemble(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_ASSEMBLY)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

double _myand()
{
	return (double)(rand() % 1001) * 0.001f;
}

void RepaintPart(ProModelitem modelitem)
{
	ProError status;
	ProSurfaceAppearanceProps proprop_orig;
	status = ProSurfaceAppearancepropsGet(&modelitem, &proprop_orig);
	ProSurfaceAppearanceProps proprop = {0.8, 0.8, 0.7, 0.6, 0.0, {_myand(), _myand(), _myand()}, {0, 0, 1}, proprop_orig.reflection, *proprop_orig.name, *proprop_orig.label, *proprop_orig.description, *proprop_orig.keywords};
	status = ProSurfaceAppearancepropsSet(&modelitem, &proprop);
}

ProError AsmCompFilter(ProFeature *feature, ProAppData app_data)
{
	ProError status;
	ProFeattype ftype;
	status = ProFeatureTypeGet(feature, &ftype);
	if (ftype == PRO_FEAT_COMPONENT)
		return PRO_TK_NO_ERROR;
	return PRO_TK_CONTINUE;
}

ProError FeatureAsmVisitAcFn(ProFeature *p_object, ProError status, CArray<ProFeature, ProFeature> *app_data)
{
	ProBoolean isVisible;
	int *r_failed_feat_ids;
	int num = 0;
	ProBoolean p_is_incomplete;
	ProFeatStatus p_status;
	ProMdl mdl;
	ProModelitem modelitem;

	status = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&r_failed_feat_ids);
	status = ProSolidFailedFeatsList((ProSolid)(p_object->owner), &r_failed_feat_ids);
	status = ProArraySizeGet((ProArray)r_failed_feat_ids, &num);
	for (int i = 0; i < num; i++)
	{
		if (p_object->id == r_failed_feat_ids[i])
		{
			ProArrayFree((ProArray *)&r_failed_feat_ids);
			return PRO_TK_NO_ERROR;
		}
	}
	ProArrayFree((ProArray *)&r_failed_feat_ids);

	status = ProFeatureIsIncomplete(p_object, &p_is_incomplete);
	if (p_is_incomplete != PRO_B_FALSE)
		return PRO_TK_NO_ERROR;

	status = ProFeatureStatusGet(p_object, &p_status);
	if (p_status == PRO_FEAT_SUPPRESSED)
		return PRO_TK_NO_ERROR;
	if (p_status != PRO_FEAT_ACTIVE)
		return PRO_TK_NO_ERROR;

	status = ProFeatureVisibilityGet(p_object, &isVisible);
	if (isVisible == PRO_B_TRUE)
	{
		status = ProAsmcompMdlGet((ProAsmcomp *)(p_object), (ProMdl *)&mdl);
		status = ProMdlToModelitem(mdl, &modelitem);
		if (modelitem.type == PRO_ASSEMBLY)
		{
			status = ProSolidFeatVisit((ProSolid)mdl, (ProFeatureVisitAction)FeatureAsmVisitAcFn, AsmCompFilter, app_data);
		}
		else
		{
			RepaintPart(modelitem);
		}
		app_data->Add(*p_object);
	}
	return PRO_TK_NO_ERROR;
}

void ColorPaint()
{
	ProError status;
	CString Macro = _T("~ Select `main_dlg_cur` `View:ProCmdViewGallery`;~ Select `main_dlg_cur` `ProCmdViewGallery_layoutph.palette_holder.clearAppearance`;~ Close `main_dlg_cur` `ProCmdViewGallery_layoutph.palette_holder.clearAppearance`;~ Activate `main_dlg_cur` `clearAllAppearance`;~ FocusIn `UI Message Dialog` `yes`;~ Activate `UI Message Dialog` `yes`;");
	Macro += _T("~ Command `About_Act`;");
	wchar_t *macro = Macro.AllocSysString();
	hint = Fun;
	status = ProMacroLoad(macro);
	SysFreeString(macro);
}

void _ColorPaint()
{
	ProError status;
	ProMdl mdl;
	CArray<ProFeature, ProFeature> feat_list;
	status = ProMdlCurrentGet(&mdl);
	srand((unsigned int)time(0));
	status = ProSolidFeatVisit((ProSolid)mdl, (ProFeatureVisitAction)FeatureAsmVisitAcFn, AsmCompFilter, (CArray<ProFeature, ProFeature> *)&feat_list);
	status = ProWindowRepaint(PRO_VALUE_UNUSED);
}

void about()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (hint == Fun)
	{
		_ColorPaint();
	}
	else
	{
		ShowDialog(L"访问我的博客获取更多信息：\nhttp://www.hudi.site");
	}
	hint = About;
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId ColorPaintID, AboutId;

	status = ProMenubarMenuAdd("ColorPaint", "ColorPaint", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("ColorPaint", "ColorPaint", "ColorPaint", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("ColorPaint_Act", (uiCmdCmdActFn)ColorPaint, uiProeImmediate, AccessAssemble, PRO_B_TRUE, PRO_B_TRUE, &ColorPaintID);
	status = ProMenubarmenuPushbuttonAdd("ColorPaint", "ColorPaintmenu", "ColorPaintmenu", "ColorPaintmenutips", NULL, PRO_B_TRUE, ColorPaintID, MSGFILE);

	status = ProCmdActionAdd("About_Act", (uiCmdCmdActFn)about, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AboutId);
	status = ProMenubarmenuPushbuttonAdd("ColorPaint", "About", "About", "Abouttips", NULL, PRO_B_TRUE, AboutId, MSGFILE);

	hint = About;
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}