 #include <vcl.h>
#pragma hdrstop

#include "RequestHttp.h"
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

String HttpPost(String endpoint, String data)
{
    HINTERNET hInternet = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;
    String respuesta = "";

    try {
        hInternet = InternetOpen(L"GimnasioApp",
                                 INTERNET_OPEN_TYPE_DIRECT,
                                 NULL, NULL, 0);

        if (!hInternet)
            return "ERROR: InternetOpen";

        hConnect = InternetConnect(hInternet,
                                   L"localhost",
                                   80,
                                   NULL, NULL,
                                   INTERNET_SERVICE_HTTP,
                                   0, 0);

        if (!hConnect)
            return "ERROR: InternetConnect";

        hRequest = HttpOpenRequest(
            hConnect,
            L"POST",
            endpoint.w_str(),
            NULL, NULL, NULL,
            0, 0
        );

        if (!hRequest)
            return "ERROR: HttpOpenRequest";

        AnsiString a = data;
        LPCSTR body = a.c_str();
        LPCSTR headers = "Content-Type: application/x-www-form-urlencoded\r\n";

        if (!HttpSendRequestA(hRequest, headers, strlen(headers),
                              (LPVOID)body, strlen(body)))
        {
            return "ERROR: HttpSendRequest";
        }

        char buffer[2048];
        DWORD bytesRead = 0;

        while (InternetReadFile(hRequest, buffer, sizeof(buffer)-1, &bytesRead))
        {
            if (bytesRead == 0) break;
            buffer[bytesRead] = 0;
            respuesta += String(buffer);
        }
    }
    catch (...) {
        respuesta = "ERROR DESCONOCIDO";
    }

    if (hRequest) InternetCloseHandle(hRequest);
    if (hConnect) InternetCloseHandle(hConnect);
    if (hInternet) InternetCloseHandle(hInternet);

    return respuesta;
}

