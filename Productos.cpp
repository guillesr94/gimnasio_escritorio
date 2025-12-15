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
	// CONFIGURACION DE LA TABLA
	if (GridInventario) {
		// ¡IMPORTANTE! 4 Columnas:
		// 0:Nombre, 1:Precio, 2:Stock, 3:ID (Oculto)
		GridInventario->ColCount = 4;
		GridInventario->FixedCols = 0;
		GridInventario->RowCount = 2;

		// Títulos
		GridInventario->Cells[0][0] = "NOMBRE PRODUCTO";
		GridInventario->Cells[1][0] = "PRECIO";
		GridInventario->Cells[2][0] = "STOCK";
		GridInventario->Cells[3][0] = "ID";

		// Anchos
		GridInventario->ColWidths[0] = 250;
		GridInventario->ColWidths[1] = 100;
		GridInventario->ColWidths[2] = 100;

		// Ancho 0 para que el ID no se vea, pero exista
		GridInventario->ColWidths[3] = 0;
	}

	CargarInventario();
}

//---------------------------------------------------------------------------
void TProductos::CargarInventario()
{
	// 1. Limpiamos visualmente la tabla
	if (!GridInventario) return;

	GridInventario->RowCount = 2;
	GridInventario->FixedRows = 1;

	// Limpiar la fila 1 por si acaso
	GridInventario->Cells[0][1] = "";
	GridInventario->Cells[1][1] = "";
	GridInventario->Cells[2][1] = "";
	GridInventario->Cells[3][1] = "";

	// 2. Pedimos los datos
	String respuesta = HttpPost("/gimnasio_api/Admin/consultar_productos.php", "");

	// Si no hay respuesta o da error, salimos
	if (respuesta.IsEmpty() || respuesta.Pos("ERROR") == 1) return;

	// 3. Parseamos el JSON
	TJSONObject* json = (TJSONObject*)TJSONObject::ParseJSONValue(respuesta);

	if (json) {
		try {
			// Verificamos que "data" exista
			TJSONValue* valData = json->GetValue("data");

			if (valData && valData->ClassNameIs("TJSONArray")) {
				TJSONArray* data = (TJSONArray*)valData;

				// Ajustamos la tabla
				if (data->Count > 0) {
					GridInventario->RowCount = data->Count + 1;
				}

				for (int i = 0; i < data->Count; i++) {
					TJSONObject* prod = (TJSONObject*)data->Items[i];
					int fila = i + 1;

					// --- LECTURA SEGURA ---

					// 1. NOMBRE
					TJSONValue* vNombre = prod->GetValue("nombre");
					if (vNombre && !vNombre->Null) {
						GridInventario->Cells[0][fila] = vNombre->Value();
					} else {
						GridInventario->Cells[0][fila] = "(Sin Nombre)";
					}

					// 2. PRECIO
					TJSONValue* vPrecio = prod->GetValue("precio");
					if (vPrecio && !vPrecio->Null) {
						GridInventario->Cells[1][fila] = vPrecio->Value();
					} else {
						GridInventario->Cells[1][fila] = "0";
					}

					// 3. STOCK
					TJSONValue* vStock = prod->GetValue("stock");
					if (vStock && !vStock->Null) {
						GridInventario->Cells[2][fila] = vStock->Value();
					} else {
						GridInventario->Cells[2][fila] = "0";
					}

					// 4. ID (CRUCIAL)
					TJSONValue* vID = prod->GetValue("id");
					if (vID && !vID->Null) {
						GridInventario->Cells[3][fila] = vID->Value();
					}
				}
			}
		}
		catch (...) {
			// Si falla, no hacemos nada para no colgar el programa
		}

		// --- CORRECCIÓN AQUÍ ---
		// Quitamos el __finally y liberamos la memoria aquí, al final
		json->Free();
	}
}


void __fastcall TProductos::actualizarDatosProductosClick(TObject *Sender)
{
	// 1. Forzar fin de edición
	if (GridInventario->EditorMode) {
		GridInventario->EditorMode = false;
	}

	int productosGuardados = 0;

	// Recorremos todas las filas de datos
	for (int i = 1; i < GridInventario->RowCount; i++)
	{
		String id = GridInventario->Cells[3][i]; // Columna oculta

		// Si no hay ID, saltamos
		if (id.IsEmpty()) continue;

		// --- PREPARAR DATOS ---
		String nombre  = GridInventario->Cells[0][i];
		String sPrecio = GridInventario->Cells[1][i];
		String sStock  = GridInventario->Cells[2][i];

		// Limpieza de Precio
		String precioLimpio = StringReplace(sPrecio, "$", "", TReplaceFlags() << rfReplaceAll).Trim();
		precioLimpio = StringReplace(precioLimpio, ",", ".", TReplaceFlags() << rfReplaceAll);

		// Codificación de Nombre (Espacios -> %20)
		String nombreParaEnviar = StringReplace(nombre, " ", "%20", TReplaceFlags() << rfReplaceAll);

		String body = "id=" + id +
					  "&nombre=" + nombreParaEnviar +
					  "&precio=" + precioLimpio +
					  "&stock=" + sStock;

		// --- ENVIAR AL SERVIDOR ---
		String resp = HttpPost("/gimnasio_api/Admin/actualizar_producto.php", body);

		// Contamos si fue exitoso
		if (resp.Pos("success") > 0) {
			productosGuardados++;
		}
		// Si falla, NO hacemos nada ni mostramos error. Silencio total.
	}

	// --- REPORTE FINAL ---
	// Si al menos UNO se guardó, mostramos Éxito y refrescamos.
	// Ignoramos si hubo errores intermedios.
	if (productosGuardados > 0) {
		ShowMessage("¡Cambios guardados correctamente!");
		CargarInventario();
	}
	else {
		// Opcional: Si NO se guardó NADA en absoluto (ej. internet desconectado)
		// Puedes dejar esto o quitarlo si quieres silencio absoluto.
		ShowMessage("No se detectaron cambios guardados.");
	}
}
//---------------------------------------------------------------------------

