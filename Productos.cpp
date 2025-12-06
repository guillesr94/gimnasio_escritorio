//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Productos.h"
#include "Profesores.h"
#include "Alumnos.h"
#include "Ventas.h"
#include "Productos.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProductos *Productos;
//---------------------------------------------------------------------------
__fastcall TProductos::TProductos(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TProductos::BtnVentanaProfesoresClick(TObject *Sender)
{
   Profesores = new TProfesores(this);
	Profesores->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TProductos::BtnVentanaCLientesClick(TObject *Sender)
{
        	 Alumnos = new TAlumnos(this);
	Alumnos->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TProductos::BtnVentanaVentasClick(TObject *Sender)
{
    Ventas = new TVentas(this);
	Ventas->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TProductos::BtnVentanaProductosClick(TObject *Sender)
{
    Productos = new TProductos(this);
	Productos->Show();


	this->Close();
}
//---------------------------------------------------------------------------

