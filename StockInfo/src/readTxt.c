#include "includes/readTxt.h"
int wchar_to_mb(const wchar_t *wstr, char *mstr, size_t mstr_len)
{
    int ret;
    if (wstr == NULL || mstr == NULL || mstr_len == 0)
    {
        return -1;
    }
    // 转换宽字符到多字节（UTF-8/GBK，适配Windows）
    ret = WideCharToMultiByte(CP_ACP, 0, wstr, -1, mstr, (int)mstr_len, NULL, NULL);
    if (ret == 0)
    {
        printf("宽字符转多字节失败！错误码：%d\n", GetLastError());
        return GetLastError();
    }
    return 0;
}

int get_full_file_path(char *filePath, size_t bufferSize)
{

    ProPath pro_path;
    ProError status;
    wchar_t w_text_path[260] = {0};
    wchar_t w_full_path[512] = {0};
    char text_path[MAX_PATH] = {0};
    int convert_err;
    errno_t err;

    memset(filePath, 0, bufferSize);

    status = ProToolkitApplTextPathGet(pro_path);
    if (status != PRO_TK_NO_ERROR)
    {
        printf("ProToolkitApplTextPathGet调用失败！错误码：%d\n", status);
        return status;
    }

    wcscpy_s(w_text_path, _countof(w_text_path), pro_path);
    wprintf(L"获取到的宽字符文本路径：%s\n", w_text_path);

    wcscpy_s(w_full_path, _countof(w_full_path), w_text_path);
    wcscat_s(w_full_path, _countof(w_full_path), L"/text/stock.txt");

    convert_err = wchar_to_mb(w_full_path, filePath, bufferSize);
    if (convert_err != 0)
    {
        return convert_err;
    }
    return 0;
}

int read_stock_file(char lines[][MAX_LINE_LEN], int maxLines, int maxLineLen, const char *filePath)
{

    FILE *fp = NULL;
    errno_t err;
    char buffer[MAX_LINE_LEN] = {0};
    int lineCount = 0;
    size_t len;

    err = fopen_s(&fp, filePath, "r");
    if (err != 0 || fp == NULL)
    {
        printf("打开文件失败！路径：%s，错误码：%d\n", filePath, err);
        return -1;
    }

    while (fgets(buffer, maxLineLen, fp) != NULL && lineCount < maxLines)
    {
        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }

        strcpy_s(lines[lineCount], maxLineLen, buffer);
        lineCount++;
    }

    fclose(fp);
    return lineCount;
}
