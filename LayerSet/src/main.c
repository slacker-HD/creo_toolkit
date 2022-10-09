#include "./includes/main.h"

int CurrentMdlType()
{
    ProMdl mdl;
    ProMdlType mdlType;
    ProError status;
    status = ProMdlCurrentGet(&mdl);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    status = ProMdlTypeGet(mdl, &mdlType);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    else
        return mdlType;
}

static uiCmdAccessState AccessDRW(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_DRAWING)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

ProError DeleteLayerByName(ProLayer Layer, ProName Name)
{
    ProError status;
    int result;
    ProName layname;
    status = ProModelitemNameGet(&Layer, layname);
    status = ProWstringCompare(Name, layname, PRO_VALUE_UNUSED, &result);
    if (result == 0)
    {
        status = ProLayerDelete(&Layer);
        return status;
    }
    else
    {
        return PRO_TK_E_NOT_FOUND;
    }
}

ProError AddLayerAndItem(ProMdl mdl, ProName LayerName, ProModelitem *Items)
{
    ProError status;
    int i, n_size;
    ProLayerItem layer_item;
    ProLayer layer;

    status = ProLayerCreate(mdl, LayerName, &layer);
    status = ProArraySizeGet((ProArray)Items, &n_size);
    for (i = 0; i < n_size; i++)
    {
        status = ProLayerItemInit((Items[i]).type, (Items[i]).id, mdl, &layer_item);
        status = ProLayerItemAdd(&layer, &layer_item);
    }
    return status;
}
ProError DimensionVisitAction(ProDimension *dimension, ProError status, ProAppData data)
{
    ProDimension **p_dims = (ProDimension **)data;
    status = ProArrayObjectAdd((ProArray *)p_dims, PRO_VALUE_UNUSED, 1, dimension);
    return status;
}

void CreateLayers()
{
    ProMdl mdl;
    ProError status;
    ProLayer *p_layer;
    int n_size, i, item_size;
    ProDwgtable *tables;
    ProDtlsyminst *syminsts;
    ProDtlnote *notes;
    ProDimension *dimensions;
    status = ProMdlCurrentGet(&mdl);
    if (CurrentMdlType() == PRO_DRAWING)
    {
        status = ProMdlLayersCollect(mdl, &p_layer);
        status = ProArraySizeGet((ProArray *)p_layer, &n_size);
        for (i = 0; i < n_size; i++)
        {
            status = DeleteLayerByName(p_layer[i], L"DIMENSION");
            status = DeleteLayerByName(p_layer[i], L"TABLE");
            status = DeleteLayerByName(p_layer[i], L"NOTE");
            status = DeleteLayerByName(p_layer[i], L"SYMBOL");
        }

        status = ProDrawingTablesCollect(mdl, &tables);
        status = AddLayerAndItem(mdl, L"TABLE", tables);
        status = ProArrayFree((ProArray *)&tables);

        status = ProDrawingDtlsyminstsCollect((ProDrawing)mdl, PRO_VALUE_UNUSED, &syminsts);
        status = AddLayerAndItem(mdl, L"SYMBOL", syminsts);
        status = ProArrayFree((ProArray *)&syminsts);

        status = ProDrawingDtlnotesCollect((ProDrawing)mdl, NULL, PRO_VALUE_UNUSED, &notes);
        status = AddLayerAndItem(mdl, L"NOTE", notes);
        status = ProArrayFree((ProArray *)&notes);

        status = ProArrayAlloc(0, sizeof(ProDimension), 1, (ProArray *)&dimensions);

        status = ProDrawingDimensionVisit((ProDrawing)mdl, PRO_DIMENSION, (ProDimensionVisitAction)DimensionVisitAction, NULL, (ProAppData)&dimensions);
        status = ProDrawingDimensionVisit((ProDrawing)mdl, PRO_REF_DIMENSION, (ProDimensionVisitAction)DimensionVisitAction, NULL, (ProAppData)&dimensions);
        status = AddLayerAndItem(mdl, L"DIMENSION", dimensions);
        status = ProArrayFree((ProArray *)&dimensions);

        status = ProArrayFree((ProArray *)&p_layer);
    }
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_LayerSetID;

    status = ProMenubarMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_LayerSet_Act", (uiCmdCmdActFn)CreateLayers, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_LayerSetID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_LayerSetmenu", "IMI_LayerSetmenu", "IMI_LayerSetmenutips", NULL, PRO_B_TRUE, IMI_LayerSetID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
