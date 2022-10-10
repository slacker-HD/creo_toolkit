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
ProError ProAddLayer(ProMdl Mdl, ProName LayerName, ProLayer *Layer)
{
    ProError status;
    status = ProLayerCreate(Mdl, LayerName, Layer);
    return status;
}

ProError ProAddItemstoLayer(ProMdl mdl, ProLayer Layer, ProModelitem *Items)
{
    ProError status;
    int i, n_size;
    ProMdlType mdltype;
    ProLayerItem layeritem;
    ProMdl CurrentDrawing;
    ProBoolean isshown;

    status = ProArraySizeGet((ProArray)Items, &n_size);

    status = ProMdlTypeGet(mdl, &mdltype);
    for (i = 0; i < n_size; i++)
    {
        if (mdltype != PRO_MDL_DRAWING)
        {
            status = ProMdlCurrentGet(&CurrentDrawing);
            status = ProAnnotationIsShown(&(Items[i]), CurrentDrawing, &isshown);
            if (isshown == PRO_B_FALSE)
            {
                continue;
            }
        }

        status = ProLayerItemInit((Items[i]).type, (Items[i]).id, mdl, &layeritem);
        if (status == PRO_TK_NO_ERROR)
        {
            status = ProLayerItemAdd(&Layer, &layeritem);
        }
    }
    return status;
}

ProError DimensionVisitAction(ProDimension *dimension, ProError status, ProAppData data)
{
    ProDimension **p_dims = (ProDimension **)data;
    status = ProArrayObjectAdd((ProArray *)p_dims, PRO_VALUE_UNUSED, 1, dimension);
    return status;
}

ProError GtolVisitAction(ProGtol *gtol, ProError status, ProAppData data)
{
    ProDimension **p_gtols = (ProGtol **)data;
    status = ProArrayObjectAdd((ProArray *)p_gtols, PRO_VALUE_UNUSED, 1, gtol);
    return status;
}

ProError SurffinVisitAction(ProSurfFinish *surffin, ProError status, ProAppData data)
{
    ProDimension **p_surffins = (ProGtol **)data;
    status = ProArrayObjectAdd((ProArray *)p_surffins, PRO_VALUE_UNUSED, 1, surffin);
    return status;
}


void CreateLayers()
{
    ProMdl mdl;
    ProError status;
    ProLayer *p_layer, layer;
    int n_size, i, j;
    ProDwgtable *tables;
    ProDtlsyminst *syminsts;
    ProDtlnote *notes;
    ProDimension *dimensions;
    ProGtol *gtols;
    ProSurfFinish *surffins;
    ProSolid solid, *solids = NULL;

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
            status = DeleteLayerByName(p_layer[i], L"GTOL");
            status = DeleteLayerByName(p_layer[i], L"SURFFIN");
        }
        status = ProArrayFree((ProArray *)&p_layer);

        status = ProDrawingTablesCollect((ProDrawing)mdl, &tables);
        status = ProAddLayer(mdl, L"TABLE", &layer);
        status = ProAddItemstoLayer(mdl, layer, tables);
        status = ProArrayFree((ProArray *)&tables);

        status = ProDrawingDtlsyminstsCollect((ProDrawing)mdl, PRO_VALUE_UNUSED, &syminsts);
        status = ProAddLayer(mdl, L"SYMBOL", &layer);
        status = ProAddItemstoLayer(mdl, layer, syminsts);
        status = ProArrayFree((ProArray *)&syminsts);

        status = ProDrawingDtlnotesCollect((ProDrawing)mdl, NULL, PRO_VALUE_UNUSED, &notes);
        status = ProAddLayer(mdl, L"NOTE", &layer);
        status = ProAddItemstoLayer(mdl, layer, notes);
        status = ProArrayFree((ProArray *)&notes);

        status = ProArrayAlloc(0, sizeof(ProDimension), 1, (ProArray *)&dimensions);
        status = ProDrawingDimensionVisit((ProDrawing)mdl, PRO_DIMENSION, (ProDimensionVisitAction)DimensionVisitAction, NULL, (ProAppData)&dimensions);
        status = ProDrawingDimensionVisit((ProDrawing)mdl, PRO_REF_DIMENSION, (ProDimensionVisitAction)DimensionVisitAction, NULL, (ProAppData)&dimensions);
        status = ProAddLayer(mdl, L"DIMENSION", &layer);
        status = ProAddItemstoLayer(mdl, layer, dimensions);
        status = ProArrayFree((ProArray *)&dimensions);

        status = ProDrawingSolidsCollect((ProDrawing)mdl, &solids);
        status = ProArraySizeGet(solids, &n_size);

        for (i = 0; i < n_size; i++)
        {
            status = ProArrayAlloc(0, sizeof(ProDimension), 1, (ProArray *)&dimensions);
            status = ProSolidDimensionVisit(solids[i], PRO_B_FALSE, (ProDimensionVisitAction)DimensionVisitAction, NULL, (ProAppData)&dimensions);
            status = ProSolidDimensionVisit(solids[i], PRO_B_TRUE, (ProDimensionVisitAction)DimensionVisitAction, NULL, (ProAppData)&dimensions);
            status = ProAddItemstoLayer((ProMdl)(solids[i]), layer, dimensions);
            status = ProArrayFree((ProArray *)&dimensions);
        }

        status = ProArrayAlloc(0, sizeof(ProGtol), 1, (ProArray *)&gtols);
        status = ProMdlGtolVisit((ProDrawing)mdl, (ProGtolVisitAction)GtolVisitAction, NULL, &gtols);
        status = ProAddLayer(mdl, L"GTOL", &layer);
        status = ProAddItemstoLayer(mdl, layer, gtols);
        status = ProArrayFree((ProArray *)&gtols);

        for (i = 0; i < n_size; i++)
        {
            status = ProArrayAlloc(0, sizeof(ProGtol), 1, (ProArray *)&gtols);
            status = ProMdlGtolVisit((ProMdl)(solids[i]), (ProGtolVisitAction)GtolVisitAction, NULL, &gtols);
            status = ProAddItemstoLayer((ProMdl)(solids[i]), layer, gtols);
            status = ProArrayFree((ProArray *)&gtols);
        }

        status = ProAddLayer(mdl, L"SURFFIN", &layer);
        for (i = 0; i < n_size; i++)
        {
            status = ProArrayAlloc(0, sizeof(ProSurfFinish), 1, (ProArray *)&surffins);
            status = ProSolidSurffinishVisit(solids[i], (ProSurffinishVisitAction)SurffinVisitAction, NULL, &surffins);
            status = ProAddItemstoLayer((ProMdl)(solids[i]), layer, surffins);
            status = ProArrayFree((ProArray *)&surffins);
        }

        status = ProArrayFree((ProArray *)&solids);
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
