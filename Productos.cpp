//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

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
