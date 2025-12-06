//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Profesores.h"
#include "Alumnos.h"
#include "Ventas.h"
#include "Productos.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProfesores *Profesores;
//---------------------------------------------------------------------------
__fastcall TProfesores::TProfesores(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TProfesores::BtnVentanaProfesoresClick(TObject *Sender)
{
    Profesores = new TProfesores(this);
	Profesores->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TProfesores::BtnVentanaCLientesClick(TObject *Sender)
{
   Alumnos = new TAlumnos(this);
	Alumnos->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TProfesores::BtnVentanaVentasClick(TObject *Sender)
{
    Ventas = new TVentas(this);
	Ventas->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TProfesores::BtnVentanaProductosClick(TObject *Sender)
{
    	 Productos = new TProductos(this);
	Productos->Show();


	this->Close();
}
//---------------------------------------------------------------------------

