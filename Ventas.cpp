//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Ventas.h"
#include "Profesores.h"
#include "Alumnos.h"
#include "Ventas.h"
#include "Productos.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVentas *Ventas;
//---------------------------------------------------------------------------
__fastcall TVentas::TVentas(TComponent* Owner)
	: TForm(Owner)    {}
//---------------------------------------------------------------------------

void __fastcall TVentas::BtnVentanaProfesoresClick(TObject *Sender)
{
        Profesores = new TProfesores(this);
	Profesores->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TVentas::BtnVentanaCLientesClick(TObject *Sender)
{
   	 Alumnos = new TAlumnos(this);
	Alumnos->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TVentas::BtnVentanaVentasClick(TObject *Sender)
{
     	 Ventas = new TVentas(this);
	Ventas->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TVentas::BtnVentanaProductosClick(TObject *Sender)
{
     	 Productos = new TProductos(this);
	Productos->Show();


	this->Close();
}
//---------------------------------------------------------------------------

