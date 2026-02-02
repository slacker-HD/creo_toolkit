#include "./includes/stockinfo.h"
static wchar_t *ReadHttpResponse(HINTERNET hRequest)
{
    char *buffer = NULL;
    DWORD readSize = 0;
    DWORD totalSize = 0;
    int needed = 0;
    wchar_t *wbuf = NULL;
    char contentType[256] = {0};
    DWORD ctLen = sizeof(contentType);
    int codepage = CP_ACP; // default to system ANSI
    char *p = NULL;
    char *q = NULL;

    do
    {
        char *newbuf = (char *)realloc(buffer, totalSize + 2048 + 1);
        if (!newbuf)
        {
            free(buffer);
            return NULL;
        }
        buffer = newbuf;

        if (!InternetReadFile(hRequest, buffer + totalSize, 2048, &readSize))
        {
            free(buffer);
            return NULL;
        }

        totalSize += readSize;
        buffer[totalSize] = '\0';
    } while (readSize > 0);

    if (!buffer)
        return NULL;

    if (HttpQueryInfoA(hRequest, HTTP_QUERY_CONTENT_TYPE, contentType, &ctLen, NULL))
    {
        p = NULL;
        for (q = contentType; *q; ++q)
        {
            if (_strnicmp(q, "charset=", 8) == 0)
            {
                p = q;
                break;
            }
        }
        if (p)
        {
            p += strlen("charset=");
            // trim
            while (*p && isspace((unsigned char)*p))
                p++;
            if (_strnicmp(p, "utf-8", 5) == 0 || _strnicmp(p, "utf8", 4) == 0)
            {
                codepage = CP_UTF8;
            }
            else if (_strnicmp(p, "gb", 2) == 0 || _strnicmp(p, "gbk", 3) == 0 || _strnicmp(p, "gb2312", 6) == 0)
            {
                codepage = 936; // GBK/GB2312
            }
        }
    }

    needed = MultiByteToWideChar(codepage, 0, buffer, -1, NULL, 0);
    if (needed == 0 && codepage != CP_UTF8)
    {
        needed = MultiByteToWideChar(CP_UTF8, 0, buffer, -1, NULL, 0);
        codepage = CP_UTF8;
    }
    if (needed == 0)
    {
        free(buffer);
        return NULL;
    }

    wbuf = (wchar_t *)malloc(needed * sizeof(wchar_t));
    if (!wbuf)
    {
        free(buffer);
        return NULL;
    }

    if (MultiByteToWideChar(codepage, 0, buffer, -1, wbuf, needed) == 0)
    {
        free(buffer);
        free(wbuf);
        return NULL;
    }

    free(buffer);
    return wbuf;
}

static wchar_t *dupwstr(const wchar_t *s)
{
    size_t n;
    wchar_t *r;
    if (!s)
        return NULL;
    n = wcslen(s) + 1;
    r = (wchar_t *)malloc(n * sizeof(wchar_t));
    if (r)
        memcpy(r, s, n * sizeof(wchar_t));
    return r;
}

wchar_t *ParseStockInfoToString(const wchar_t *response)
{
    const wchar_t *start;
    const wchar_t *end;
    size_t data_len;
    wchar_t *data;
    wchar_t temp[64];
    wchar_t stockName[64];
    wchar_t stockCode[16];
    double currentPrice;
    double openPrice;
    double highPrice;
    double lowPrice;
    double changePrice;
    double changeRate;
    int parsed;
    size_t outsz;
    wchar_t *out;

    if (!response)
        return dupwstr(L"响应数据为空！\n");

    start = wcschr(response, L'"');
    end = wcsrchr(response, L'"');
    if (!start || !end || start >= end)
        return dupwstr(L"解析股票数据失败！未找到有效数据段\n");

    data_len = (size_t)(end - start - 1);
    data = (wchar_t *)malloc((data_len + 1) * sizeof(wchar_t));
    if (!data)
        return dupwstr(L"内存分配失败！\n");
    wcsncpy(data, start + 1, data_len);
    data[data_len] = L'\0';

    memset(temp, 0, sizeof(temp));
    memset(stockName, 0, sizeof(stockName));
    memset(stockCode, 0, sizeof(stockCode));

    parsed = swscanf(data, L"%63[^~]~%63[^~]~%15[^~]~%lf~%lf~%lf~%lf", temp, stockName, stockCode, &currentPrice, &openPrice, &highPrice, &lowPrice);
    free(data);

    if (parsed < 7 || currentPrice <= 0)
    {
        wchar_t buf[128];
        swprintf_s(buf, sizeof(buf) / sizeof(wchar_t), L"解析股票数据失败！字段解析不完整（解析到%d个字段）\n", parsed);
        return dupwstr(buf);
    }

    changePrice = currentPrice - openPrice;
    changeRate = (openPrice != 0.0) ? (changePrice / openPrice * 100.0) : 0.0;

    outsz = 512;
    out = (wchar_t *)malloc(outsz * sizeof(wchar_t));
    if (!out)
        return dupwstr(L"内存分配失败！\n");

    _snwprintf_s(out, outsz, _TRUNCATE,
                 L"股票代码：%ls   "
                 L"股票名称：%ls   "
                 L"当前价格：%.2f 元   "
                 L"开盘价格：%.2f 元   "
                 L"最高价格：%.2f 元   "
                 L"最低价格：%.2f 元   "
                 L"涨跌金额：%.2f 元   "
                 L"涨跌幅  ：%.2f %%",
                 stockCode, stockName, currentPrice, openPrice, highPrice, lowPrice, changePrice, changeRate);
    return out;
}

// 申请了内存，调用者负责释放
wchar_t *GetStockInfoStringByCode(const wchar_t *stockCode)
{
    wchar_t requestPath[64];
    HINTERNET hInternet = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;
    wchar_t *response = NULL;
    wchar_t *result = NULL;
    wchar_t buf[128];

    if (!stockCode || wcslen(stockCode) != 8)
        return dupwstr(L"输入错误！股票代码格式不正确，请按示例格式输入。\n");
    if (wcsncmp(stockCode, L"sh", 2) != 0 && wcsncmp(stockCode, L"sz", 2) != 0)
        return dupwstr(L"输入错误！股票代码格式不正确，请按示例格式输入。\n");

    swprintf_s(requestPath, sizeof(requestPath) / sizeof(wchar_t), L"/q=%ls", stockCode);

    hInternet = InternetOpenW(L"StockInfoClient", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet)
    {
        swprintf_s(buf, sizeof(buf) / sizeof(wchar_t), L"初始化网络失败，错误码：%lu\n", GetLastError());
        return dupwstr(buf);
    }

    hConnect = InternetConnectW(hInternet, L"qt.gtimg.cn", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect)
    {
        swprintf_s(buf, sizeof(buf) / sizeof(wchar_t), L"连接API服务器失败，错误码：%lu\n", GetLastError());
        InternetCloseHandle(hInternet);
        return dupwstr(buf);
    }

    hRequest = HttpOpenRequestW(hConnect, L"GET", requestPath, NULL, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_SECURE, 0);
    if (!hRequest)
    {
        swprintf_s(buf, sizeof(buf) / sizeof(wchar_t), L"创建HTTP请求失败，错误码：%lu\n", GetLastError());
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return dupwstr(buf);
    }

    if (!HttpSendRequestW(hRequest, NULL, 0, NULL, 0))
    {
        swprintf_s(buf, sizeof(buf) / sizeof(wchar_t), L"发送HTTP请求失败，错误码：%lu\n", GetLastError());
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return dupwstr(buf);
    }

    response = ReadHttpResponse(hRequest);
    if (!response)
    {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return dupwstr(L"读取响应数据失败！\n");
    }

    result = ParseStockInfoToString(response);
    free(response);

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return result;
}
