#include "./includes/AlignSymDim.h"

typedef enum _alignmode
{
	Vertical = 0,
	Horizon = 1,
} ALIGNMENT;

void _align(ALIGNMENT alignment)
{
	ProError status;
	ProSelection *SelBuffer = NULL;
	int i, size, sheet_id;
	ProModelitem Modelitem;
	ProVector Dimlocation;
	ProMdl mdl;
	ProVector Mousepos;
	ProMouseButton Mousebutton;
	ProDtlsyminstdata SymData;
	ProDtlattach SymAttachOld, SymAttachNew;
	ProDtlattachType SymType;
	ProView SymView;
	ProVector location;
	ProSelection attach_point;

	status = ProMdlCurrentGet(&mdl);
	status = ProSelbufferSelectionsGet(&SelBuffer);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProArraySizeGet(SelBuffer, &size);
		if (status == PRO_TK_NO_ERROR && size > 0)
		{
			status = ProMessageDisplay(MSGFILE, "IMI_MESSAGE_LeftMouseSelectPosition");
			status = ProMousePickGet(PRO_LEFT_BUTTON, &Mousebutton, Mousepos);

			for (i = 0; i < size; i++)
			{
				status = ProSelectionModelitemGet(SelBuffer[i], &Modelitem);
				if (status == PRO_TK_NO_ERROR)
				{
					if (Modelitem.type == PRO_DIMENSION || Modelitem.type == PRO_REF_DIMENSION)
					{
						status = ProDrawingDimensionPosGet((ProDrawing)mdl, &Modelitem, Dimlocation);
						if (alignment == Vertical)
							Dimlocation[1] = Mousepos[1];
						else
							Dimlocation[0] = Mousepos[0];
						status = ProDrawingDimensionMove((ProDrawing)mdl, &Modelitem, Dimlocation);
					}
					else if (Modelitem.type == PRO_SYMBOL_INSTANCE)
					{
						status = ProDtlsyminstDataGet(&Modelitem, PRODISPMODE_SYMBOLIC, &SymData);
						status = ProDtlsyminstdataAttachmentGet(SymData, &SymAttachOld);
						status = ProDtlattachGet(SymAttachOld, &SymType, &SymView, location, &attach_point);
						status = ProDtlattachGet(SymAttachOld, &SymType, &SymView, location, &attach_point);
						if (status == PRO_TK_NO_ERROR)
						{
							if (alignment == Vertical)
								location[1] = Mousepos[1];
							else
								location[0] = Mousepos[0];
							status = ProDtlattachAlloc(SymType, SymView, location, attach_point, &SymAttachNew);
							status = ProDtlsyminstdataAttachmentSet(SymData, SymAttachNew);
							status = ProDtlsyminstModify(&Modelitem, SymData);
							status = ProDtlattachFree(SymAttachNew);
						}
					}
					else
						continue;
				}
			}
		}
		status = ProSelectionarrayFree(SelBuffer);
		status = ProDrawingCurrentSheetGet((ProDrawing)mdl, &sheet_id);
		status = ProDwgSheetRegenerate((ProDrawing)mdl, sheet_id);
	}
}

void VerticalAlign()
{
	_align(Vertical);
}

void HorizonAlign()
{
	_align(Horizon);
}