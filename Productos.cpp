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

__fastcall TProductos::TProductos(TComponent* Owner)
	: TForm(Owner)
{
	// Verificamos que el componente exista para evitar errores
	if (listProducts) {
		listProducts->Items->Clear(); // Limpiamos cualquier texto viejo

		// Agregamos un encabezado
		listProducts->Items->Add("ID   PRODUCTO                  PRECIO");
		listProducts->Items->Add("-----------------------------------------");

		// Agregamos los productos de prueba
		listProducts->Items->Add("1    Botella de Agua 500ml     $500.00");
		listProducts->Items->Add("2    Gatorade                  $1200.00");
		listProducts->Items->Add("3    Proteína Whey             $2500.00");
		listProducts->Items->Add("4    Toalla de Mano            $3000.00");
		listProducts->Items->Add("5    Barrita de Cereal         $800.00");
	}
}
