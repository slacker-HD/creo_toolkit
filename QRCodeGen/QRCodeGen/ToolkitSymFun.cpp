#include "stdafx.h"
#include "qrencode.h"
#include "CommonToolkitFun.h"

CString DtlsymdefName(ProDtlsymdef *p_sym_def)
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
		if (DtlsymdefName(&p_symdefs[i]) == QRCODESYMNAME)
		{
			status = ProDtlsymdefDelete(&p_symdefs[i]);
		}
	}
	status = ProArrayFree((ProArray *)&p_symdefs);
}

void SymInstCreateFree(ProDrawing drawing, ProDtlsymdef *definition, ProVector position)
{
	ProError status;
	ProDtlsyminstdata sdata;
	ProDtlattach attach;
	ProDtlsyminst syminst;

	status = ProDtlsyminstdataAlloc(drawing, &sdata);
	status = ProDtlsyminstdataDefSet(sdata, definition);
	status = ProDtlsyminstdataAttachtypeSet(sdata, PROSYMDEFATTACHTYPE_FREE);
	status = ProDtlattachAlloc(PRO_DTLATTACHTYPE_FREE, NULL, position, NULL, &attach);
	status = ProDtlsyminstdataAttachmentSet(sdata, attach);
	status = ProDtlattachFree(attach);
	status = ProDtlsyminstCreate(drawing, sdata, &syminst);
	status = ProDtlsyminstdataFree(sdata);
	status = ProDtlsyminstShow(&syminst);
}

void PrintQRCodeDot(ProDtlsymdef *symdef, ProVector start, ProColortype color)
{
	ProError status;
	ProDtlentity entity;
	ProDtlentitydata edata;
	ProCurvedata *curve;
	ProColor entity_color;
	ProVector end;
	end[0] = start[0];
	end[1] = start[1] + 1;
	end[2] = start[2];
	status = ProDtlentitydataAlloc(symdef->owner, &edata);
	status = ProCurvedataAlloc(&curve);
	status = ProLinedataInit(start, end, curve);
	status = ProDtlentitydataCurveSet(edata, curve);
	entity_color.method = PRO_COLOR_METHOD_TYPE;
	entity_color.value.type = color;
	status = ProDtlentitydataColorSet(edata, &entity_color);
	status = ProDtlentitydataWidthSet(edata, 1.0);
	status = ProDtlentityCreate(symdef->owner, symdef, edata, &entity);
	status = ProDtlentitydataFree(edata);
}

void QRcodeSymdefCreate(ProDrawing drawing, CString name, CString message, ProVector position)
{
	ProError status;
	ProDtlsymdefdata sdata;
	ProVector origin = {0, 0, 0}, start;
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
	status = ProDtlsymdefdataFree(sdata);
	if (pQRC = QRcode_encodeString(message, 0, QR_ECLEVEL_L, QR_MODE_8, 1))
	{
		start[0] = 0.5;
		start[1] = -0.5;
		start[2] = 0.0;
		pSourceData = pQRC->data;
		unWidth = pQRC->width;
		start[1] += unWidth;
		for (y = 0; y < unWidth; y++)
		{
			for (x = 0; x < unWidth; x++)
			{
				if (*pSourceData & 1)
				{
					PrintQRCodeDot(&symdef, start, PRO_COLOR_LETTER);
				}
				start[0]++;
				pSourceData++;
			}
			start[0] = 0.5;
			start[1]--;
		}
		QRcode_free(pQRC);
	}
	SymInstCreateFree(drawing, &symdef, position);
}

void PlaceQRCodeSymbol(CString message, ProVector position)
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
	ProVector pos = {10, 10, 0};
	//删除symdef自动会把inst也删除
	DeletePreQrCodeDef();
	PlaceQRCodeSymbol(_T("这是测试的二维码"), pos);
}
