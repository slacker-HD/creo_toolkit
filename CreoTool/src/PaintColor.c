#include "./includes/PaintColor.h"

void ClearColor()
{
	ProError status;
	status = ProMacroLoad(L"~ Activate `main_dlg_cur` `page_View_control_btn` 1; ~ Select `main_dlg_cur` `View:ProCmdViewGallery`; ~ Select `main_dlg_cur`  `ProCmdViewGallery_layoutph.palette_holder.clearAppearance`; ~ Close `main_dlg_cur`  `ProCmdViewGallery_layoutph.palette_holder.clearAppearance`; ~ Activate `main_dlg_cur` `clearAllAppearance`; ~ FocusIn `UI Message Dialog` `yes`;~ Activate `UI Message Dialog` `yes`;");
}

double _myand()
{
	return (double)(rand() % 1001) * 0.001f;
}

void _repaintPart(ProModelitem modelitem)
{
	ProError status;
	ProSurfaceAppearanceProps proprop_orig;
	status = ProSurfaceAppearancepropsGet(&modelitem, &proprop_orig);
	{
		ProSurfaceAppearanceProps proprop = {0.8, 0.8, 0.7, 0.6, 0.0, {_myand(), _myand(), _myand()}, {0, 0, 1}, proprop_orig.reflection, *proprop_orig.name, *proprop_orig.label, *proprop_orig.description, *proprop_orig.keywords};
		status = ProSurfaceAppearancepropsSet(&modelitem, &proprop);
	}
}

ProError _asmCompFilter(ProFeature *feature, ProAppData app_data)
{
	ProError status;
	ProFeattype ftype;
	status = ProFeatureTypeGet(feature, &ftype);
	if (ftype == PRO_FEAT_COMPONENT)
		return PRO_TK_NO_ERROR;
	return PRO_TK_CONTINUE;
}

ProError _asmVisitAcFn(ProFeature *p_object, ProError status, void *app_data)
{
	ProBoolean isVisible;
	ProFeatStatus p_status;
	ProMdl mdl;
	ProModelitem modelitem;

	status = ProFeatureStatusGet(p_object, &p_status);
	status = ProFeatureVisibilityGet(p_object, &isVisible);
	if (isVisible == PRO_B_TRUE && p_status != PRO_FEAT_SUPPRESSED)
	{
		status = ProAsmcompMdlGet((ProAsmcomp *)(p_object), (ProMdl *)&mdl);
		status = ProMdlToModelitem(mdl, &modelitem);
		if (modelitem.type == PRO_ASSEMBLY)
		{
			status = ProSolidFeatVisit((ProSolid)mdl, (ProFeatureVisitAction)_asmVisitAcFn, _asmCompFilter, NULL);
		}
		else
		{
			_repaintPart(modelitem);
		}
	}
	return PRO_TK_NO_ERROR;
}

void AsmPaintColor_AfterMacro()
{
	ProError status;
	ProMdl mdl;
	status = ProMdlCurrentGet(&mdl);
	srand((unsigned int)time(0));
	status = ProSolidFeatVisit((ProSolid)mdl, (ProFeatureVisitAction)_asmVisitAcFn, _asmCompFilter, NULL);
	status = ProWindowRepaint(PRO_VALUE_UNUSED);
}

void PaintColorAsm()
{
	ProError status;
	hint = AsmPaintColor;
	status = ProMacroLoad(L"~ Command `IMI_ClearColor_Act`;~ Command `IMI_About_Act`;");
}

void PaintColorPrt()
{
	ProError status;
	hint = PrtPaintColor;
	status = ProMacroLoad(L"~ Command `IMI_ClearColor_Act`;~ Command `IMI_About_Act`;");
}

void PrtPaintColor_AfterMacro()
{
	ProError status;
	ProMdl mdl;
	ProModelitem modelitem;

	status = ProMdlCurrentGet(&mdl);
	status = ProMdlToModelitem(mdl, &modelitem);
	srand((unsigned int)time(0));
	_repaintPart(modelitem);
	status = ProWindowRepaint(PRO_VALUE_UNUSED);
}

void PrtSettransSurface()
{
	ProError status;
	ProModelitem modelitem;
	ProSurfaceAppearanceProps appearanceProperties;
	ProSelection *sel_array;
	int i, n_size;

	status = ProMessageDisplay(MSGFILE, "IMI_MESSAGE_SelectSurface");
	status = ProSelect((char *)"sldsurface,surface", -1, NULL, NULL, NULL, NULL, &sel_array, &n_size);
	status = ProArraySizeGet((ProArray *)sel_array, &n_size);
	if (status != PRO_TK_NO_ERROR || n_size < 1)
	{
		return;
	}
	for (i = 0; i < n_size; i++)
	{
		status = ProSelectionModelitemGet(sel_array[i], &modelitem);
		status = ProSurfaceAppearancepropsGet(&modelitem, &appearanceProperties);
		appearanceProperties.transparency = 0.7;
		status = ProSurfaceAppearancepropsSet(&modelitem, &appearanceProperties);
	}
	status = ProUserRefreshAll();
}