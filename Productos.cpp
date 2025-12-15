//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Productos.h"
#include "Profesores.h"
#include "Alumnos.h"
#include "Ventas.h"
#include "Productos.h"


#include "RequestHttp.h"
#include <System.JSON.hpp>

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

	if (GridInventario) {
		// el id queda oculto

		GridInventario->ColCount = 4;
		GridInventario->FixedCols = 0;
		GridInventario->RowCount = 2;

		// Títulos  del cuadro
		GridInventario->Cells[0][0] = "NOMBRE PRODUCTO";
		GridInventario->Cells[1][0] = "PRECIO";
		GridInventario->Cells[2][0] = "STOCK";
		GridInventario->Cells[3][0] = "ID";

		// Medida chequear
		GridInventario->ColWidths[0] = 250;
		GridInventario->ColWidths[1] = 100;
		GridInventario->ColWidths[2] = 100;


		GridInventario->ColWidths[3] = 0;
	}

	CargarInventario();
}

//---------------------------------------------------------------------------
void TProductos::CargarInventario()
{

	if (!GridInventario) return;

	GridInventario->RowCount = 2;
	GridInventario->FixedRows = 1;


	GridInventario->Cells[0][1] = "";
	GridInventario->Cells[1][1] = "";
	GridInventario->Cells[2][1] = "";
	GridInventario->Cells[3][1] = "";


	String respuesta = HttpPost("/gimnasio_api/Admin/consultar_productos.php", "");


	if (respuesta.IsEmpty() || respuesta.Pos("ERROR") == 1) return;


	TJSONObject* json = (TJSONObject*)TJSONObject::ParseJSONValue(respuesta);

	if (json) {
		try {

			TJSONValue* valData = json->GetValue("data");

			if (valData && valData->ClassNameIs("TJSONArray")) {
				TJSONArray* data = (TJSONArray*)valData;


				if (data->Count > 0) {
					GridInventario->RowCount = data->Count + 1;
				}

				for (int i = 0; i < data->Count; i++) {
					TJSONObject* prod = (TJSONObject*)data->Items[i];
					int fila = i + 1;


					TJSONValue* vNombre = prod->GetValue("nombre");
					if (vNombre && !vNombre->Null) {
						GridInventario->Cells[0][fila] = vNombre->Value();
					} else {
						GridInventario->Cells[0][fila] = "(Sin Nombre)";
					}


					TJSONValue* vPrecio = prod->GetValue("precio");
					if (vPrecio && !vPrecio->Null) {
						GridInventario->Cells[1][fila] = vPrecio->Value();
					} else {
						GridInventario->Cells[1][fila] = "0";
					}


					TJSONValue* vStock = prod->GetValue("stock");
					if (vStock && !vStock->Null) {
						GridInventario->Cells[2][fila] = vStock->Value();
					} else {
						GridInventario->Cells[2][fila] = "0";
					}


					TJSONValue* vID = prod->GetValue("id");
					if (vID && !vID->Null) {
						GridInventario->Cells[3][fila] = vID->Value();
					}
				}
			}
		}
		catch (...) {

		}


		json->Free();
	}
}


void __fastcall TProductos::actualizarDatosProductosClick(TObject *Sender)
{

	if (GridInventario->EditorMode) {
		GridInventario->EditorMode = false;
	}

	int productosGuardados = 0;


	for (int i = 1; i < GridInventario->RowCount; i++)
	{
		String id = GridInventario->Cells[3][i];


		if (id.IsEmpty()) continue;


		String nombre  = GridInventario->Cells[0][i];
		String sPrecio = GridInventario->Cells[1][i];
		String sStock  = GridInventario->Cells[2][i];


		String precioLimpio = StringReplace(sPrecio, "$", "", TReplaceFlags() << rfReplaceAll).Trim();
		precioLimpio = StringReplace(precioLimpio, ",", ".", TReplaceFlags() << rfReplaceAll);


		String nombreParaEnviar = StringReplace(nombre, " ", "%20", TReplaceFlags() << rfReplaceAll);

		String body = "id=" + id +
					  "&nombre=" + nombreParaEnviar +
					  "&precio=" + precioLimpio +
					  "&stock=" + sStock;


		String resp = HttpPost("/gimnasio_api/Admin/actualizar_producto.php", body);


		if (resp.Pos("success") > 0) {
			productosGuardados++;
		}

	}


	if (productosGuardados > 0) {
		ShowMessage("¡Cambios guardados correctamente!");
		CargarInventario();
	}
	else {

		ShowMessage("No se detectaron cambios guardados.");
	}
}
//---------------------------------------------------------------------------


void __fastcall TProductos::agregarProductoClick(TObject *Sender)
{

	TForm *ventana = new TForm(this);
	ventana->Caption = "Nuevo Producto";
	ventana->Width = 350;
	ventana->Height = 350;
	ventana->Position = poMainFormCenter;
	ventana->BorderStyle = bsDialog;


	TLabel *lNombre = new TLabel(ventana);
	lNombre->Parent = ventana;
	lNombre->Left = 40; lNombre->Top = 20;
	lNombre->Caption = "Nombre del Producto";

	TEdit *eNombre = new TEdit(ventana);
	eNombre->Parent = ventana;
	eNombre->Left = 40; eNombre->Top = 40;
	eNombre->Width = 250;
	eNombre->TextHint = "Ej: Proteina Whey";


	TLabel *lPrecio = new TLabel(ventana);
	lPrecio->Parent = ventana;
	lPrecio->Left = 40; lPrecio->Top = 80;
	lPrecio->Caption = "Precio";

	TEdit *ePrecio = new TEdit(ventana);
	ePrecio->Parent = ventana;
	ePrecio->Left = 40; ePrecio->Top = 100;
	ePrecio->Width = 250;
	ePrecio->TextHint = "Ej: 15000";
	ePrecio->NumbersOnly = true;


	TLabel *lStock = new TLabel(ventana);
	lStock->Parent = ventana;
	lStock->Left = 40; lStock->Top = 140;
	lStock->Caption = "Stock Inicial";

	TEdit *eStock = new TEdit(ventana);
	eStock->Parent = ventana;
	eStock->Left = 40; eStock->Top = 160;
	eStock->Width = 250;
	eStock->TextHint = "Ej: 50";
	eStock->NumbersOnly = true;


	TButton *btnGuardar = new TButton(ventana);
	btnGuardar->Parent = ventana;
	btnGuardar->Left = 40;
	btnGuardar->Top = 220;
	btnGuardar->Width = 250;
	btnGuardar->Height = 45;
	btnGuardar->Caption = "GUARDAR PRODUCTO";
	btnGuardar->ModalResult = mrOk;



	if (ventana->ShowModal() == mrOk)
	{

		if(eNombre->Text.IsEmpty() || ePrecio->Text.IsEmpty() || eStock->Text.IsEmpty()) {
			ShowMessage("Debes completar todos los campos.");
		}
		else
		{

			String precioLimpio = StringReplace(ePrecio->Text, ",", ".", TReplaceFlags() << rfReplaceAll);


			String nombreCodificado = StringReplace(eNombre->Text, " ", "%20", TReplaceFlags() << rfReplaceAll);


			String body = "nombre=" + nombreCodificado +
						  "&precio=" + precioLimpio +
						  "&stock=" + eStock->Text +
						  "&descripcion=";


			String resp = HttpPost("/gimnasio_api/Admin/crear_producto.php", body);

			if (resp.Pos("success") > 0) {
				ShowMessage("¡Producto creado correctamente!");
				CargarInventario();
			}
			else {
				ShowMessage("Error al guardar: " + resp);
			}
		}
	}


	delete ventana;
}
//---------------------------------------------------------------------------

void __fastcall TProductos::eliminarProductoClick(TObject *Sender)
{

    int fila = GridInventario->Row;


	if (fila < 1) {
        ShowMessage("Por favor, selecciona un producto de la lista primero.");
        return;
    }


	String id = GridInventario->Cells[3][fila];
	String nombre = GridInventario->Cells[0][fila];


    if (id.IsEmpty()) {
        ShowMessage("No hay ningún producto seleccionado en esta fila.");
        return;
    }

	String pregunta = "¿Estás seguro que deseas eliminar el producto: " + nombre + "?";

    if (MessageDlg(pregunta, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
    {

        String body = "id=" + id;
        String resp = HttpPost("/gimnasio_api/Admin/eliminar_producto.php", body);


        if (resp.Pos("success") > 0) {
            ShowMessage("Producto eliminado correctamente.");
			CargarInventario();
        }
        else {
            ShowMessage("Error al eliminar. El servidor respondió: " + resp);
        }
    }
}
//---------------------------------------------------------------------------

