#include <windows.h>

#include "./includes/main.h"
#include "./includes/cJSON.h"
#include "./includes/stockinfo.h"
#include "./includes/ReadTxt.h"

char g_stock_codes[MAX_LINES][MAX_LINE_LEN] = {0}; // 多字节股票代码
int g_stock_line_count = 0;                        // 读取到的股票代码行数
int g_current_index = 0;                           // 记录当前显示的索引，初始为0

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}
void StockInfo()
{
    ProError status;
    wchar_t w_code[256] = {0};
    wchar_t *info = NULL;
    wchar_t w_error_msg[512] = {0};
    const wchar_t *demoCode = L"sh600733";

    if (g_stock_line_count > 0)
    {
        /* 跳过空行（避免无效代码） */
        while (strlen(g_stock_codes[g_current_index]) == 0)
        {
            g_current_index++;
            if (g_current_index >= g_stock_line_count)
            {
                g_current_index = 0;
            }
        }

        MultiByteToWideChar(CP_ACP, 0, g_stock_codes[g_current_index], -1, w_code, 256);

        info = GetStockInfoStringByCode(w_code);
        if (info)
        {
            status = ProMessageDisplay(MSGFILE, "IMI_PrompInfo", info);
            free(info);
            info = NULL;
        }

        g_current_index++;
        if (g_current_index >= g_stock_line_count)
        {
            g_current_index = 0;
        }
    }
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_StockInfoID;
    char filePath[MAX_PATH] = {0};
    int pathErr;

    pathErr = get_full_file_path(filePath, MAX_PATH);
    if (pathErr == 0)
    {
        g_stock_line_count = read_stock_file(g_stock_codes, MAX_LINES, MAX_LINE_LEN, filePath);
    }
    g_current_index = 0;

    status = ProMenubarMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "About", PRO_B_TRUE, MSGFILE);
    status = ProCmdActionAdd("IMI_StockInfo_Act", (uiCmdCmdActFn)StockInfo, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_StockInfoID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_StockInfomenu", "IMI_StockInfomenu", "IMI_StockInfomenutips", NULL, PRO_B_TRUE, IMI_StockInfoID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}