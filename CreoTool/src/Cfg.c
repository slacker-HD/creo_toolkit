#include "./includes/cfg.h"

#define MAX_LINE 256

int read_config_file(wchar_t *filename /*in*/, wchar_t key[] /*in*/, wchar_t value[] /*in out*/, int *value_len /*out*/)
{
    int ret = -2;
    FILE *fp = NULL;
    wchar_t *p = NULL;
    wchar_t *start = NULL;
    wchar_t *end = NULL;
    wchar_t line_buffer[MAX_LINE];
    _wfopen_s(&fp, filename, L"r");
    if (fp == NULL)
    {
        ret = -1;
        return ret;
    }
    while (!feof(fp))
    {
        memset(line_buffer, 0, sizeof(line_buffer));
        fgetws(line_buffer, MAX_LINE, fp);
        p = wcschr(line_buffer, L'=');
        if (p == NULL)
        {
            continue;
        }
        p = wcsstr(line_buffer, key);
        if (p == NULL)
        {
            continue;
        }
        p = p + wcslen(key);
        p = wcschr(p, L'=');
        if (p == NULL)
        {
            continue;
        }
        else
        {
            p = p + 1;
            for (;;)
            {
                if (*p == ' ')
                {
                    p++;
                }
                else
                {
                    start = p;
                    if (*start == '\n')
                    {
                        goto End;
                    }
                    break;
                }
            }
            for (;;)
            {
                if ((*p == ' ' || *p == '\n'))
                {
                    break;
                }
                else
                {
                    p++;
                }
            }
            end = p;
            *value_len = end - start;
            wmemcpy(value, start, end - start);
            ret = 0;
            break;
        }
    }
End:
    if (!(fp == NULL))
    {
        fclose(fp);
    }
    return ret;
}

int write_or_update_config_file(wchar_t *filename /*in*/, wchar_t key[] /*in*/, wchar_t value[] /*in*/, int value_len /*in*/)
{
    int ret = 0;
    int key_exist = 0;
    int file_length = 0;
    FILE *fp = NULL;
    wchar_t line_buffer[MAX_LINE];
    wchar_t *p = NULL;
    wchar_t file_buffer[1024 * 4] = {0};
    if (filename == NULL || key == NULL || value == NULL)
    {
        ret = -1;
        goto End;
    }
    _wfopen_s(&fp, filename, L"r+");
    if (fp == NULL)
    {
        ret = -2;
    }
    if (fp == NULL)
    {
        _wfopen_s(&fp, filename, L"w+t");
        if (fp == NULL)
        {
            ret = -3;
            goto End;
        }
    }
    fseek(fp, 0L, SEEK_END);
    file_length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    if (file_length > 1024 * 4)
    {
        ret = -3;
        goto End;
    }
    while (!feof(fp))
    {
        memset(line_buffer, 0, sizeof(line_buffer));
        p = fgetws(line_buffer, MAX_LINE, fp);
        if (p == NULL)
        {
            break;
        }
        p = wcsstr(line_buffer, key);
        if (p == NULL)
        {
            wcscat(file_buffer, line_buffer);
            continue;
        }
        else
        {
            swprintf(line_buffer, L"%ls = %ls\n", key, value);
            wcscat(file_buffer, line_buffer);
            key_exist = 1;
        }
    }
    if (key_exist == 0)
    {
        fwprintf(fp, L"%ls = %ls\n", key, value);
    }
    else
    {
        if (fp != NULL)
        {
            fclose(fp);
            fp = NULL;
        }
        _wfopen_s(&fp, filename, L"r+");
        if (fp == NULL)
        {
            ret = -4;
            goto End;
        }
        fputws(file_buffer, fp);
    }
End:
    if (!(fp == NULL))
    {
        fclose(fp);
    }
    return ret;
}

int ReadConfig(wchar_t *FileName, wchar_t *Key, wchar_t *Value, int *ValueLen)
{
    int ret = 0;
    ret = read_config_file(FileName, Key, Value, ValueLen);
    return ret;
}

int WriteOrUpdateConfig(wchar_t *FileName, wchar_t *Key, wchar_t *Value)
{
    int ret = 0;
    ret = write_or_update_config_file(FileName, Key, Value, wcslen(Value));
    return ret;
}
