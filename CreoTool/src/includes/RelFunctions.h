#ifndef __RELFUNCTIONS__
#define __RELFUNCTIONS__
#include "main.h"
#include "CommonFuns.h"

ProError StrToDouble(ProRelset *relset, ProMdl mdl, char *ext_func_name, ProParamvalue *args, ProAppData data, ProParamvalue *result);
ProError DoubleToStr(ProRelset *relset, ProMdl mdl, char *ext_func_name, ProParamvalue *args, ProAppData data, ProParamvalue *result);
ProError StrToInt(ProRelset *relset, ProMdl mdl, char *ext_func_name, ProParamvalue *args, ProAppData data, ProParamvalue *result);
ProError IntToStr(ProRelset *relset, ProMdl mdl, char *ext_func_name, ProParamvalue *args, ProAppData data, ProParamvalue *result);

#endif
