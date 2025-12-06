//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>


#include "LoginForm.h"
#include "Profesores.h"
#include "Alumnos.h"
#include "Productos.h"
#include "Ventas.h"
//---------------------------------------------------------------------------
USEFORM("Productos.cpp", Form4);
USEFORM("Profesores.cpp", Form2);
USEFORM("Ventas.cpp", Form5);
USEFORM("LoginForm.cpp", Form1);
USEFORM("Alumnos.cpp", Form3);
USEFORM("Menu.cpp", MenuForm);

//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TLoginForm), &LoginForm);
		Application->CreateForm(__classid(TProfesores), &Profesores);
		Application->CreateForm(__classid(TAlumnos), &Alumnos);
		Application->CreateForm(__classid(TProductos), &Productos);
		Application->CreateForm(__classid(TVentas), &Ventas);
		Application->CreateForm(__classid(TMenuForm), &MenuForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
