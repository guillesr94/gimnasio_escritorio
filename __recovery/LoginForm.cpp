//---------------------------------------------------------------------------

#include <vcl.h>

#include "LoginForm.h"

#pragma hdrstop

#include "LoginForm.h"






//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLoginForm *LoginForm;
//---------------------------------------------------------------------------
__fastcall TLoginForm::TLoginForm(TComponent* Owner)
	: TForm(Owner)
{
}
void __fastcall TLoginForm::FormCreate(TObject *Sender)
{
    // Crear el conector HTTP cuando se crea el formulario
    api = new HTTPConnector();
}

void __fastcall TLoginForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    // Liberar memoria cuando se cierra el formulario
    delete api;
}

void __fastcall TLoginForm::IniciarClick(TObject *Sender){
// 1. Deshabilitar botón para evitar doble clic
    btnIniciar->Enabled = false;

    try {
        // 2. Preparar datos para login
        String datosLogin = "email=" + email->Text.Trim() +
                           "&contrasena=" + password->Text.Trim();

        // 3. Llamar a la API usando nuestro conector
        String respuesta = api->POST(
            "http://localhost/gimnasio_api/Admin/consultar_admin.php",
            datosLogin
        );

        // 4. Mostrar la respuesta cruda (para depuración)
        ShowMessage("Respuesta API:\n" + respuesta);

        // 5. (OPCIONAL) Parsear JSON para verificar login
        // Si quieres procesar la respuesta automáticamente:
        TJSONObject *json = (TJSONObject*)TJSONObject::ParseJSONValue(respuesta);
        if (json) {
            try {
                String status = json->GetValue("status")->Value();
                String message = json->GetValue("message")->Value();

                if (status == "success") {
                    ShowMessage("¡Login exitoso!\n" + message);
                    // Aquí podrías abrir el formulario principal
                    // MainForm->Show();
                    // this->Hide();
                } else {
                    ShowMessage("Error: " + message);
                }
            }
            __finally {
                delete json;
            }
        }

    } catch (Exception &e) {
        ShowMessage("Error en la aplicación: " + e.Message);
    }
    __finally {
        // 6. Rehabilitar botón
        btnIniciar->Enabled = true;
	}
}













//---------------------------------------------------------------------------
//funcionando!!
//void __fastcall TLoginForm::IniciarClick(TObject *Sender)
//{
//	HINTERNET hInternet = NULL;
//	HINTERNET hConnect = NULL;
//	HINTERNET hRequest = NULL;
//
//	try {
//		// 1. Inicializar WinINET
//		hInternet = InternetOpen(L"GimnasioApp",
//								INTERNET_OPEN_TYPE_DIRECT,
//								NULL, NULL, 0);
//
//		if (!hInternet) throw Exception("No se pudo inicializar Internet");
//
//        // 2. Conectar al servidor
//        hConnect = InternetConnect(hInternet,
//                                  L"localhost",
//                                  80,  // Puerto HTTP explícito
//                                  NULL, NULL,
//                                  INTERNET_SERVICE_HTTP,
//                                  0, 0);
//
//        if (!hConnect) throw Exception("No se pudo conectar al servidor");
//
//        // 3. Crear request POST
//        hRequest = HttpOpenRequest(hConnect,
//								  L"POST",
//                                  L"/gimnasio_api/Admin/consultar_admin.php",
//                                  NULL, NULL, NULL,
//                                  0, 0);
//
//        if (!hRequest) throw Exception("No se pudo crear la solicitud");
//
//        // 4. Preparar datos - CORRECCIÓN IMPORTANTE
//        String datos = "email=" + email->Text.Trim() +
//                      "&contrasena=" + password->Text.Trim();
//
//        // Convertir a ANSI (la API PHP espera esto)
//        AnsiString datosAnsi = datos;
//		LPCSTR datosPtr = datosAnsi.c_str();
//        DWORD datosLength = strlen(datosPtr);  // Longitud REAL sin nulo
//
//		// 5. Enviar request - CORRECCIÓN AQUÍ
//        LPCSTR headers = "Content-Type: application/x-www-form-urlencoded\r\n";
//
//        if (!HttpSendRequestA(hRequest,  // Usar versión ANSI
//                             headers,
//                             strlen(headers),
//							 (LPVOID)datosPtr,
//                             datosLength)) {
//            DWORD error = GetLastError();
//            // CORRECCIÓN AQUÍ:
//            char errorMsg[100];
//            sprintf(errorMsg, "Error enviando solicitud. Código: %lu", error);
//            throw Exception(errorMsg);
//        }
//
//		// 6. Leer respuesta
//		char buffer[4096];
//        DWORD bytesRead;
//        String respuesta = "";
//
//        while (InternetReadFile(hRequest, buffer, sizeof(buffer)-1, &bytesRead)) {
//            if (bytesRead == 0) break;
//            buffer[bytesRead] = 0;
//            respuesta += String(buffer);
//		}
//
//        // 7. Mostrar respuesta
//		ShowMessage("API respondió:\n" + respuesta);
//
//    } catch (Exception &e) {
//        ShowMessage("Error: " + e.Message);
//    }
//
//	// 8. Limpiar
//	if (hRequest) InternetCloseHandle(hRequest);
//	if (hConnect) InternetCloseHandle(hConnect);
//	if (hInternet) InternetCloseHandle(hInternet);
//}
