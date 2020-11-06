#include "stdafx.h"
#include "FitTol.h"

ProError GToInterCal()
{
	ProError status;
	int n_parts, i;
	double volume;
	ProMdl mdl;
	ProModelitem part1, part2;
	ProName name1, name2;
	ProMdlType mdltype;
	ProInterferenceInfo *interf_info_arr;
	status = ProMdlCurrentGet(&mdl);
	status = ProFitGlobalinterferenceCompute((ProAssembly)mdl, PRO_FIT_SUB_ASSEMBLY, PRO_B_FALSE, PRO_B_FALSE, PRO_B_FALSE, &interf_info_arr);
	if (status != PRO_TK_NO_ERROR)
		return PRO_TK_GENERAL_ERROR;

	status = ProArraySizeGet(interf_info_arr, &n_parts);
	for (i = 0; i < n_parts; i++)
	{
		status = ProSelectionModelitemGet(interf_info_arr[i].part_1, &part1);
		status = ProMdlNameGet(part1.owner, name1);
		status = ProSelectionModelitemGet(interf_info_arr[i].part_2, &part2);
		status = ProMdlNameGet(part2.owner, name2);
		status = ProFitInterferencevolumeCompute(interf_info_arr[i].interf_data, &volume);
		CString a = CString(name1);
		CString b = CString(name2);
		CString c;
		c.Format(_T("%lf"), volume);
		AfxMessageBox(a + _T("和") + b + _T("发生干涉，干涉量为") + c);
	}
	status = ProInterferenceInfoProarrayFree(interf_info_arr);
	return PRO_TK_NO_ERROR;
}

