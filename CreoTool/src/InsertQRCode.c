#include "includes/InsertQRCode.h"

ProError ProDtlsymdefNameGet(ProDtlsymdef *p_sym_def, ProName name)
{
	ProDtlsymdefdata data;
	ProError status;
	if (p_sym_def == NULL || p_sym_def->type != PRO_SYMBOL_DEFINITION)
		return PRO_TK_BAD_INPUTS;
	status = ProDtlsymdefDataGet(p_sym_def, &data);
	status = ProDtlsymdefdataNameGet(data, name);
	return status;
}

void DeletePreQrCodeDef()
{
	ProError status;
	ProDrawing drawing;
	ProDtlsymdef *p_symdefs;
	ProName name;
	int i, size;
	int cmpResult;

	if (CurrentMdlType() != PRO_DRAWING)
		return;
	status = ProMdlCurrentGet((ProMdl *)&drawing);
	status = ProDrawingDtlsymdefsCollect(drawing, &p_symdefs);
	status = ProArraySizeGet((ProArray)p_symdefs, &size);
	for (i = 0; i < size; i++)
	{
		status = ProDtlsymdefNameGet(&p_symdefs[i], name);
		status = ProWstringCompare(name, QRCODESYMNAME, PRO_VALUE_UNUSED, &cmpResult);
		if (cmpResult == 0)
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

void QRcodeSymdefCreate(ProDrawing drawing, ProName symdefname, char *message, ProVector position)
{
	ProError status;
	ProDtlsymdefdata sdata;
	ProVector origin = {0, 0, 0}, start;
	ProDtlsymdefattach attach;
	ProDtlsymdef symdef;

	int unWidth, x, y;
	int i = 0, size;
	unsigned char *pSourceData;
	uint8_t qr0[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(message, tempBuffer, qr0, qrcodegen_Ecc_MEDIUM, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

	status = ProDtlsymdefdataAlloc(drawing, &sdata);
	status = ProDtlsymdefdataPathSet(sdata, symdefname);
	status = ProDtlsymdefdataHeighttypeSet(sdata, PRODTLSYMDEFHGHTTYPE_FIXED);
	status = ProDtlsymdefattachAlloc(PROSYMDEFATTACHTYPE_FREE, 0, 0.0, origin, &attach);
	status = ProDtlsymdefdataAttachAdd(sdata, attach);
	status = ProDtlsymdefattachFree(attach);
	status = ProDtlsymdefCreate(drawing, sdata, &symdef);
	status = ProDtlsymdefdataFree(sdata);

	size = qrcodegen_getSize(qr0);
	start[0] = 0.5;
	start[1] = -0.5;
	start[2] = 0.0;

	for (y = 0; y < size; y++)
	{
		for (x = 0; x < size; x++)
		{
			if (qrcodegen_getModule(qr0, x, y))
			{
				PrintQRCodeDot(&symdef, start, PRO_COLOR_LETTER);
			}
			start[0]++;
		}
		start[0] = 0.5;
		start[1]--;
	}
	SymInstCreateFree(drawing, &symdef, position);
}

void _placeQrCodeSymbol(char *Message, ProVector Position)
{
	ProError status;
	ProDrawing drawing;
	if (CurrentMdlType() != PRO_DRAWING)
		return;
	status = ProMdlCurrentGet((ProMdl *)&drawing);
	QRcodeSymdefCreate(drawing, QRCODESYMNAME, Message, Position);
	status = ProMacroLoad(L"~ Command `ProCmdDwgRegenModel` ; ~Command `ProCmdWinActivate`;");
}

void _placeQrCode(char *Message, ProVector MousePosition)
{
	DeletePreQrCodeDef();
	_placeQrCodeSymbol(Message, MousePosition);
}

int ConvertUnicodeToANSI(const wchar_t *unicode, char *ansi, int ansiBufferSize)
{

	int result = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, ansi, ansiBufferSize, NULL, NULL);
	ansi[ansiBufferSize - 1] = 0;
	return result;
}

void ShowQRCodeDialog()
{
	ProError status;
	wchar_t wcontent[127];
	char *content[255];
	ProDrawing drawing;
	ProMouseButton Mousebutton;
	ProVector Mousepos;

	if (CurrentMdlType() != PRO_DRAWING)
		return;

	status = ProMessageDisplay(MSGFILE, "IMI_ENTER_QRCODETEXT");
	status = ProMessageStringRead(QRCODE_MAX_LENGTH, wcontent);

	if (status == PRO_TK_NO_ERROR)
	{
		status = ProMessageDisplay(MSGFILE, "IMI_MESSAGE_LeftMouseSelectPosition");
		status = ProMousePickGet(PRO_LEFT_BUTTON, &Mousebutton, Mousepos);
		if (status == PRO_TK_NO_ERROR)
		{
			ConvertUnicodeToANSI(wcontent, content, sizeof(content));
			_placeQrCode(content,Mousepos);
		}
	}
}
