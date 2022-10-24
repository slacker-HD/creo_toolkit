#include "./includes/RelFunctions.h"

ProError StrToDouble(ProRelset *relset, ProMdl mdl, char *ext_func_name, ProParamvalue *args, ProAppData data, ProParamvalue *result)
{
	result->type = PRO_PARAM_DOUBLE;
	result->value.d_val = _wtof(args[0].value.s_val);
	return PRO_TK_NO_ERROR;
}

ProError DoubleToStr(ProRelset *relset, ProMdl mdl, char *ext_func_name, ProParamvalue *args, ProAppData data, ProParamvalue *result)
{
	result->type = PRO_PARAM_STRING;
	swprintf(result->value.s_val, L"%f", args[0].value.d_val);
	return PRO_TK_NO_ERROR;
}

ProError StrToInt(ProRelset *relset, ProMdl mdl, char *ext_func_name, ProParamvalue *args, ProAppData data, ProParamvalue *result)
{
	result->type = PRO_PARAM_INTEGER;
	result->value.i_val = _wtoi(args[0].value.s_val);
	return PRO_TK_NO_ERROR;
}

ProError IntToStr(ProRelset *relset, ProMdl mdl, char *ext_func_name, ProParamvalue *args, ProAppData data, ProParamvalue *result)
{
	result->type = PRO_PARAM_STRING;
	swprintf(result->value.s_val, L"%d", args[0].value.d_val);
	return PRO_TK_NO_ERROR;
}
