#include "stdafx.h"
#include "qrencode.h"
#include "CommonToolkitFun.h"

CString DtlsymdefNameGet(ProDtlsymdef *p_sym_def)
{
	ProDtlsymdefdata data;
	ProError status;
	ProName name;
	if (p_sym_def == NULL || p_sym_def->type != PRO_SYMBOL_DEFINITION)
		return "";

	status = ProDtlsymdefDataGet(p_sym_def, &data);
	status = ProDtlsymdefdataNameGet(data, name);
	return CString(name);
}

void DeletePreQrCodeDef()
{
	ProError status;
	ProDrawing drawing;
	ProDtlsymdef *p_symdefs;
	int size;
	CString SymName;
	if (CurrentMdlType() != PRO_DRAWING)
		return;
	status = ProMdlCurrentGet((ProMdl *)&drawing);
	status = ProDrawingDtlsymdefsCollect(drawing, &p_symdefs);
	status = ProArraySizeGet((ProArray)p_symdefs, &size);
	for (int i = 0; i < size; i++)
	{
		if (DtlsymdefNameGet(&p_symdefs[i]) == QRCODESYMNAME)
		{
			status = ProDtlsymdefDelete(&p_symdefs[i]);
		}
	}
	status = ProArrayFree((ProArray *)&p_symdefs);
}

CString DtlsyminstNameGet(ProDtlsyminst *p_sym_def)
{

	ProError status;
	ProDtlsyminstdata instdata;
	ProDtlsymdef symdef;
	if (p_sym_def == NULL || p_sym_def->type != PRO_SYMBOL_DEFINITION)
		return "";
	status = ProDtlsyminstDataGet(p_sym_def, PRODISPMODE_SYMBOLIC, &instdata);
	status = ProDtlsyminstdataDefGet(instdata, &symdef);
	return DtlsymdefNameGet(&symdef);
}

void DeletePreQrCodeInst()
{
	ProError status;
	ProDrawing drawing;
	ProDtlsyminst *p_syminsts;
	int size;
	CString SymName;
	if (CurrentMdlType() != PRO_DRAWING)
		return;
	status = ProMdlCurrentGet((ProMdl *)&drawing);
	status = ProDrawingDtlsyminstsCollect(drawing, PRO_VALUE_UNUSED, &p_syminsts);
	status = ProArraySizeGet((ProArray)p_syminsts, &size);
	for (int i = 0; i < size; i++)
	{
		if (DtlsyminstNameGet(&p_syminsts[i]) == QRCODESYMNAME)
		{
			status = ProDtlsyminstDelete(&p_syminsts[i]);
		}
	}
	status = ProArrayFree((ProArray *)&p_syminsts);
}

void SymInstCreateFree(ProDrawing drawing, ProDtlsymdef *definition, ProVector position)
{
	ProError status;
	ProDtlsyminstdata sdata;
	ProDtlattach attach;
	ProDtlsyminst syminst;

	status = ProDtlsyminstdataAlloc(drawing, &sdata);
	if (status != PRO_TK_NO_ERROR)
		return;
	status = ProDtlsyminstdataDefSet(sdata, definition);
	if (status != PRO_TK_NO_ERROR)
	{
		status = ProDtlsyminstdataFree(sdata);
		return;
	}
	status = ProDtlsyminstdataAttachtypeSet(sdata, PROSYMDEFATTACHTYPE_FREE);
	if (status != PRO_TK_NO_ERROR)
	{
		status = ProDtlsyminstdataFree(sdata);
		return;
	}
	status = ProDtlattachAlloc(PRO_DTLATTACHTYPE_FREE, NULL, position, NULL, &attach);
	if (status != PRO_TK_NO_ERROR)
	{
		status = ProDtlsyminstdataFree(sdata);
		return;
	}
	status = ProDtlsyminstdataAttachmentSet(sdata, attach);
	if (status != PRO_TK_NO_ERROR)
	{
		status = ProDtlsyminstdataFree(sdata);
		return;
	}
	ProDtlattachFree(attach);
	if (status != PRO_TK_NO_ERROR)
	{
		status = ProDtlsyminstdataFree(sdata);
		return;
	}

	status = ProDtlsyminstCreate(drawing, sdata, &syminst);
	if (status != PRO_TK_NO_ERROR)
	{
		status = ProDtlsyminstdataFree(sdata);
		return;
	}
	status = ProDtlsyminstdataFree(sdata);
	if (status != PRO_TK_NO_ERROR)
		return;
	status = ProDtlsyminstShow(&syminst);
}

void QRCodeDotPrint(ProDtlsymdef *symdef, ProVector start, ProVector end, ProColortype color)
{
	ProError status;
	ProDtlentity entity;
	ProDtlentitydata edata;
	ProCurvedata *curve;
	ProColor entity_color;

	ProDtlentitydataAlloc(symdef->owner, &edata);
	ProCurvedataAlloc(&curve);
	ProLinedataInit(start, end, curve);
	ProDtlentitydataCurveSet(edata, curve);
	entity_color.method = PRO_COLOR_METHOD_TYPE;
	entity_color.value.type = color;
	status = ProDtlentitydataColorSet(edata, &entity_color);
	status = ProDtlentitydataWidthSet(edata, 1.0);
	status = ProDtlentityCreate(symdef->owner, symdef, edata, &entity);

	ProDtlentitydataFree(edata);
}

void QRcodeSymdefCreate(ProDrawing drawing, CString name, CString message, ProVector position)
{
	ProError status;
	ProDtlsymdefdata sdata;
	ProVector origin= {0, 0, 0}, e1, e2;
	ProDtlsymdefattach attach;
	ProDtlsymdef symdef;
	wchar_t *symdefname;

	int unWidth, x, y;
	unsigned char *pSourceData;
	QRcode *pQRC;

	status = ProDtlsymdefdataAlloc(drawing, &sdata);
	symdefname = name.AllocSysString();
	status = ProDtlsymdefdataPathSet(sdata, symdefname);
	SysFreeString(symdefname);
	status = ProDtlsymdefdataHeighttypeSet(sdata, PRODTLSYMDEFHGHTTYPE_FIXED);
	status = ProDtlsymdefattachAlloc(PROSYMDEFATTACHTYPE_FREE, 0, 0.0, origin, &attach);
	status = ProDtlsymdefdataAttachAdd(sdata, attach);
	status = ProDtlsymdefattachFree(attach);
	status = ProDtlsymdefCreate(drawing, sdata, &symdef);
	if (status != PRO_TK_NO_ERROR)
		return;
	status = ProDtlsymdefdataFree(sdata);

	if (pQRC = QRcode_encodeString(message, 0, QR_ECLEVEL_L, QR_MODE_8, 1))
	{
		e1[0] = 0.5;
		e1[1] = -0.5;
		e1[2] = 0.0;
		e2[0] = 0.5;
		e2[1] = 0.5;
		e2[2] = 0.0;
		pSourceData = pQRC->data;
		unWidth = pQRC->width;
		e1[1] += unWidth;
		e2[1] += unWidth;
		for (y = 0; y < unWidth; y++)
		{
			for (x = 0; x < unWidth; x++)
			{
				if (*pSourceData & 1)
				{
					QRCodeDotPrint(&symdef, e1, e2, PRO_COLOR_LETTER);
				}
				e1[0]++;
				e2[0]++;
				pSourceData++;
			}
			e1[0] = 0.5;
			e2[0] = 0.5;
			e1[1]--;
			e2[1]--;
		}
		QRcode_free(pQRC);
	}
	SymInstCreateFree(drawing, &symdef, position);
}

void QRCodeToSymbol(CString message, ProVector position)
{
	ProError status;
	ProDrawing drawing;
	if (CurrentMdlType() != PRO_DRAWING)
		return;
	status = ProMdlCurrentGet((ProMdl *)&drawing);
	QRcodeSymdefCreate(drawing, QRCODESYMNAME, message, position);
	status = ProMacroLoad(L"~ Command `ProCmdDwgRegenModel` ; ~Command `ProCmdWinActivate`;");
}

void PlaceQrCode()
{
	DeletePreQrCodeInst();
	DeletePreQrCodeDef();
	ProVector pos = {10, 10, 0};
	QRCodeToSymbol(_T("这是测试的二维码"), pos);
}
