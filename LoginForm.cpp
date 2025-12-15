#include <vcl.h>
#include "RequestHttp.h"



#include "LoginForm.h"
#pragma hdrstop

#include "LoginForm.h"

#include "Menu.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLoginForm *LoginForm;
//---------------------------------------------------------------------------
__fastcall TLoginForm::TLoginForm(TComponent* Owner)
	: TForm(Owner)
{
}



void __fastcall TLoginForm::IniciarClick(TObject *Sender)
{
	 String datos = "email=" + email->Text.Trim() +
				   "&contrasena=" + password->Text.Trim();


	String respuesta = HttpPost(
		L"/gimnasio_api/Admin/consultar_admin.php",
		datos
	);

String low = respuesta.LowerCase();

	if (low.Pos("\"status\":\"success\"") > 0)
	{
		  MenuForm->Show();


        this->Hide();
	}
	else
	{
		ShowMessage("Login incorrecto:\n" + respuesta);
	}
}



















