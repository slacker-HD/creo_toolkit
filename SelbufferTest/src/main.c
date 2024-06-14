#include "./includes/main.h"

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

static uiCmdAccessState AccessPRT(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_PART)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

static uiCmdAccessState AccessASM(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_ASSEMBLY)
        return ACCESS_AVAILABLE;
        
    else
        return ACCESS_INVISIBLE;
}

ProError FeatVisitActFn(ProFeature *p_feature, ProError status, ProAppData p_features)
{
    ProBoolean isVisible;
    ProFeatStatus p_status;
    ProModelitem modelitem;

    status = ProFeatureStatusGet(p_feature, &p_status);
    status = ProFeatureVisibilityGet(p_feature, &isVisible);
    if (isVisible == PRO_B_TRUE && p_status != PRO_FEAT_SUPPRESSED)
    {
        status = ProArrayObjectAdd((ProArray *)p_features, PRO_VALUE_UNUSED, 1, p_feature);
    }
    return PRO_TK_NO_ERROR;
}

ProError FeatVisitFilter(ProFeature *feature, ProAppData app_data)
{
    ProError status;
    ProFeattype ftype;
    status = ProFeatureTypeGet(feature, &ftype);
    if (ftype == PRO_FEAT_CSYS)
        return PRO_TK_NO_ERROR;
    return PRO_TK_CONTINUE;
}

void SelCoord()
{
    ProError status;
    ProMdl mdl;
    ProSelection selection;
    ProFeature *p_features;
    int i, n_size;

    status = ProMdlCurrentGet(&mdl);
    status = ProArrayAlloc(0, sizeof(ProFeature), 1, (ProArray *)&p_features);

    status = ProSolidFeatVisit((ProSolid)mdl, (ProFeatureVisitAction)FeatVisitActFn, FeatVisitFilter, (ProAppData)&p_features);
    status = ProSelbufferClear();
    status = ProArraySizeGet(p_features, &n_size);

    for (i = 0; i < n_size; i++)
    {
        status = ProSelectionAlloc(NULL, &(p_features[i]), &selection);
        status = ProSelbufferSelectionAdd(selection);
    }

    status = ProArrayFree(&p_features);
}

ProError AsmCompPathVisitActFn(ProAsmcomppath *path, ProSolid solid, ProBoolean down, ProAppData p_comppaths)
{
    ProError status;
    // ProSolidDispCompVisit同时往up和down两个方向检索，所以只看down方向,up方向跳过
    if (down == PRO_B_TRUE)
    {
        status = ProArrayObjectAdd((ProArray *)p_comppaths, PRO_VALUE_UNUSED, 1, path);
    }
    return PRO_TK_NO_ERROR;
}

ProError AsmCompPathVisitFilter(ProAsmcomppath *p_path, ProSolid solid, ProAppData app_data)
{
    // 这里遍历，所以没有filter
    return PRO_TK_NO_ERROR;
}

ProError AsmCompVisitFilter(ProFeature *feature, ProAppData app_data)
{
    ProError status;
    ProFeattype ftype;
    status = ProFeatureTypeGet(feature, &ftype);
    if (ftype == PRO_FEAT_COMPONENT)
        return PRO_TK_NO_ERROR;
    return PRO_TK_CONTINUE;
}

ProError AsmCompVisitActFn(ProFeature *p_comp, ProError status, void *p_comps)
{
    ProBoolean isVisible;
    ProFeatStatus p_status;
    ProMdl mdl;
    ProModelitem modelitem;

    status = ProFeatureStatusGet(p_comp, &p_status);
    status = ProFeatureVisibilityGet(p_comp, &isVisible);
    if (isVisible == PRO_B_TRUE && p_status != PRO_FEAT_SUPPRESSED)
    {
        status = ProArrayObjectAdd((ProArray *)p_comps, PRO_VALUE_UNUSED, 1, p_comp);
        status = ProAsmcompMdlGet((ProAsmcomp *)(p_comp), (ProMdl *)&mdl);
        status = ProMdlToModelitem(mdl, &modelitem);
        if (modelitem.type == PRO_ASSEMBLY)
        {
            status = ProSolidFeatVisit((ProSolid)mdl, (ProFeatureVisitAction)AsmCompVisitActFn, AsmCompVisitFilter, p_comps);
        }
    }
    return PRO_TK_NO_ERROR;
}

void SelComp()
{
    ProError status;
    int j, i, n_compsize, n_pathsize;
    ProMdl mdl;
    ProSolid solid;
    ProAsmcomppath *p_comppaths;
    ProModelitem *p_comps;
    ProSelection selection;

    status = ProMdlCurrentGet(&mdl);
    // status = ProSelbufferClear();
    status = ProArrayAlloc(0, sizeof(ProAsmcomppath), 1, (ProArray *)&p_comppaths);
    status = ProSolidDispCompVisit((ProSolid)mdl, AsmCompPathVisitActFn, AsmCompPathVisitFilter, (ProAppData)&p_comppaths);
    status = ProArraySizeGet(p_comppaths, &n_pathsize);

    status = ProArrayAlloc(0, sizeof(ProModelitem), 1, (ProArray *)&p_comps);
    status = ProSolidFeatVisit((ProSolid)mdl, (ProFeatureVisitAction)AsmCompVisitActFn, AsmCompVisitFilter, (ProAppData)&p_comps);
    status = ProArraySizeGet(p_comps, &n_compsize);

    // 没有一一对应，p_comppaths比p_comps多了一个装配体的根节点
    // ProSelectionAlloc第一个参数是其父节点的ProAsmcomppath，所以p_comppaths的根节点很重要不能删除
    // ProSelectionAlloc前两个参数如果不对应会返回PRO_TK_BAD_INPUTS，如果此时执行ProSelbufferSelectionAdd会导致Creo异常退出。
    // 这里没有记录p_comppaths比p_comps的对应关系，故使用双循环遍历后判断添加，效率很低，仅做演示，作为全选的功能够用了
    // 建议定义一个记录装配体树形结构数据结构同时保存节点位置和其对应的p_comppaths和p_comps以便完成选取对应的组件功能
    for (j = 0; j < n_pathsize; j++)
    {
        for (i = 0; i < n_compsize; i++)
        {
            status = ProSelectionAlloc(&(p_comppaths[j]), &(p_comps[i]), &selection);
            if (status == PRO_TK_NO_ERROR)
                status = ProSelbufferSelectionAdd(selection);
        }
    }
    status = ProArrayFree(&p_comppaths);
    status = ProArrayFree(&p_comps);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_SelCoordmenuID, IMI_SelCompmenuID;

    status = ProMenubarMenuAdd("IMI_SelbufferTestmenu", "IMI_SelbufferTestmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_SelCoord_Act", (uiCmdCmdActFn)SelCoord, uiProeImmediate, AccessPRT, PRO_B_TRUE, PRO_B_TRUE, &IMI_SelCoordmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_SelbufferTestmenu", "IMI_SelCoordMenu", "IMI_SelCoordMenu", "IMI_SelCoordMenutips", NULL, PRO_B_TRUE, IMI_SelCoordmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_SelComp_Act", (uiCmdCmdActFn)SelComp, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_SelCompmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_SelbufferTestmenu", "IMI_SelCompMenu", "IMI_SelCompMenu", "IMI_SelCompMenutips", NULL, PRO_B_TRUE, IMI_SelCompmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
