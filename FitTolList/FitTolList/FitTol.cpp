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


ProError SelpartInterference()
{
	ProError status;
	int nSels = 0;
	ProSelection *sel_array;
	ProModelitem part1, part2;
	ProName name1, name2;
	ProInterferenceData interf_data;
    double volume;
	CString inter;

	status = ProSelect("prt_or_asm", 2, NULL, NULL, NULL, NULL, &sel_array, &nSels);
	if (status != PRO_TK_NO_ERROR || nSels <= 0)
	{
		return PRO_TK_GENERAL_ERROR;
	}
	status = ProFitInterferenceCompute(sel_array[0], sel_array[1], PRO_B_FALSE, PRO_B_FALSE, &interf_data);
	if (interf_data == NULL)
    {
		AfxMessageBox(_T("未发生干涉。"));
        return PRO_TK_NO_ERROR;
    }

	status = ProSelectionModelitemGet(sel_array[0], &part1);
	status = ProMdlNameGet(part1.owner, name1);
	status = ProSelectionModelitemGet(sel_array[1], &part2);
	status = ProMdlNameGet(part2.owner, name2);
	status = ProFitInterferencevolumeCompute(interf_data, &volume);
	CString a = CString(name1);
	CString b = CString(name2);
	CString c;
	c.Format(_T("%lf"), volume);
	AfxMessageBox(a + _T("和") + b + _T("发生干涉，干涉量为") + c);
	status = ProInterferenceDataFree(interf_data);
    return PRO_TK_NO_ERROR;
}
