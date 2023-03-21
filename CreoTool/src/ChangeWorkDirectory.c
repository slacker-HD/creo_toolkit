#include "./includes/ChangeWorkDirectory.h"

void _commitChangeDir()
{
	ProError status;
	int n_selection, i, pathLength;
	char **selections = NULL, *stringTmp = NULL;
	status = ProUIListSelectednamesGet("ChangeWorkDirectory", "WorkDirList", &n_selection, &selections);
	if (n_selection > 0)
	{
		status = ProArraySizeGet(CurrentWorkDirectoryList, &pathLength);
		for (i = 0; i < pathLength; i++)
		{
			stringTmp = (char *)calloc(PRO_PATH_SIZE * 2 + 1, sizeof(char));
			ProWstringToString(stringTmp, CurrentWorkDirectoryList[i]);
			if (strcmp(selections[0], stringTmp) == 0)
			{
				status = ProDirectoryChange(CurrentWorkDirectoryList[i]);
				free(stringTmp);
				break;
			}

			free(stringTmp);
		}
	}
	status = ProStringarrayFree(selections, n_selection);
	status = ProUIDialogExit("ChangeWorkDirectory", 1);
}

void _commitCancelChangeDir()
{
	ProError status;
	status = ProUIDialogExit("ChangeWorkDirectory", 0);
}

void ShowChangeWorkDirectoryDialog()
{
	ProError status;
	char **charNameList;
	int i, pathLength;
	status = ProUIDialogCreate("ChangeWorkDirectory", "ChangeWorkDirectory");

	status = ProArraySizeGet(CurrentWorkDirectoryList, &pathLength);

	status = ProArrayAlloc(pathLength, sizeof(char *), pathLength, (ProArray *)&charNameList);

	for (i = 0; i < pathLength; i++)
	{
		charNameList[i] = (char *)calloc(PRO_PATH_SIZE * 2 + 1, sizeof(char));
		ProWstringToString(charNameList[i], CurrentWorkDirectoryList[i]);
	}

	status = ProUIListNamesSet("ChangeWorkDirectory", "WorkDirList", pathLength, charNameList);
	status = ProUIListLabelsSet("ChangeWorkDirectory", "WorkDirList", pathLength, (wchar_t **)(&CurrentWorkDirectoryList)
	);

	status = ProUIPushbuttonActivateActionSet("ChangeWorkDirectory", "CommitChangeDir", (ProUIAction)_commitChangeDir, NULL);
	status = ProUIPushbuttonActivateActionSet("ChangeWorkDirectory", "CommitCancelChangeDir", (ProUIAction)_commitCancelChangeDir, NULL);
	// status = ProUIListSelectActionSet("ChangeWorkDirectory", "WorkDirList", (ProUIAction)_selectDirectory, NULL);

	status = ProUIDialogActivate("ChangeWorkDirectory", NULL);
	status = ProUIDialogDestroy("ChangeWorkDirectory");

	for (i = 0; i < pathLength; i++)
	{
		free(charNameList[i]);
	}
	status = ProArrayFree((ProArray *)&charNameList);
}
