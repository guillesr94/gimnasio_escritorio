//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Ventas.h"
#include "Profesores.h"
#include "Alumnos.h"
#include "Ventas.h"
#include "Productos.h"


#include "RequestHttp.h"
#include <System.JSON.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVentas *Ventas;
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

__fastcall TVentas::TVentas(TComponent* Owner)
	: TForm(Owner)
{
	// 1. CONFIGURAR LA TABLA VISUALMENTE
	if (GridProductos) {
        // Definimos 3 columnas: Producto, Precio, Stock
        GridProductos->ColCount = 4;

		// Ponemos los títulos
        GridProductos->Cells[0][0] = "NOMBRE PRODUCTO";
        GridProductos->Cells[1][0] = "PRECIO";
		GridProductos->Cells[2][0] = "STOCK";

		// Ajustamos anchos (puedes cambiar los números)
        GridProductos->ColWidths[0] = 200; // Nombre ancho
        GridProductos->ColWidths[1] = 100; // Precio
		GridProductos->ColWidths[2] = 80;  // Stock
		GridProductos->Cells[3][0] = "CANTIDAD";
    }

	// 2. LLAMAR A LA FUNCIÓN DE CARGA
	CargarDatosEnLaGrilla();
}

//---------------------------------------------------------------------------
// FUNCIÓN PARA LEER API Y LLENAR TABLA
//---------------------------------------------------------------------------
void TVentas::CargarDatosEnLaGrilla()
{
	// Llamada a la API
	String respuesta = HttpPost("/gimnasio_api/Admin/consultar_productos.php", "");

    // Validación básica
    if (respuesta.Pos("ERROR") == 1 || respuesta.IsEmpty()) {
        ShowMessage("Error al conectar con la base de datos");
		return;
    }

	// Parseo JSON
	TJSONObject* json = (TJSONObject*)TJSONObject::ParseJSONValue(respuesta);

    if (json) {
        try {
			TJSONArray* data = (TJSONArray*)json->GetValue("data");
			if (data) {
				// Ajustamos la cantidad de filas en la tabla
				// (Cantidad de productos + 1 fila fija de títulos)
				GridProductos->RowCount = data->Count + 1;

				for (int i = 0; i < data->Count; i++) {
					TJSONObject* prod = (TJSONObject*)data->Items[i];

					// La fila 0 es título, empezamos en 1
                    int fila = i + 1;

                    // Llenamos las celdas
					GridProductos->Cells[0][fila] = prod->GetValue("nombre")->Value();
					GridProductos->Cells[1][fila] = "$" + prod->GetValue("precio")->Value();
					GridProductos->Cells[2][fila] = prod->GetValue("stock")->Value();
					GridProductos->Cells[3][fila] = "0";
				}
            }
		}
		__finally {
            json->Free();
		}
	}
}



void __fastcall TVentas::GridProductosSelectCell(TObject *Sender, System::LongInt ACol,
		  System::LongInt ARow, bool &CanSelect)
{
	if (ACol == 3) {
		GridProductos->Options = GridProductos->Options << goEditing;
	}
	else {
		GridProductos->Options = GridProductos->Options >> goEditing;
	}
}
//---------------------------------------------------------------------------



void __fastcall TVentas::CalcularTotalClick(TObject *Sender)
{
    double granTotal = 0;
    String mensajeResumen = "DETALLE DE LA COMPRA:\n\n";
    bool comproAlgo = false;

    // Recorremos desde la fila 1 (saltamos los títulos)
    for (int i = 1; i < GridProductos->RowCount; i++) {

        // 1. OBTENER CANTIDAD
        String sCant = GridProductos->Cells[3][i];
        int cantidad = StrToIntDef(sCant, 0);

        // Si la cantidad es 0, pasamos al siguiente
        if (cantidad <= 0) continue;

        // --- NUEVO: VALIDACIÓN DE STOCK ---
        // Leemos la columna 2 (STOCK)
        String sStock = GridProductos->Cells[2][i];
        int stock = StrToIntDef(sStock, 0);

        // Comprobamos si pide más de lo que hay
	if (cantidad > stock) {
            String nombreProd = GridProductos->Cells[0][i];

            // CONSTRUIMOS EL MENSAJE PASO A PASO (PARA EVITAR EL ERROR)
            String errorMsg = "¡ERROR DE STOCK!\n\n";
            errorMsg = errorMsg + "Producto: " + nombreProd + "\n";
            errorMsg = errorMsg + "Pediste: " + IntToStr(cantidad) + "\n";
            errorMsg = errorMsg + "Disponible: " + IntToStr(stock) + "\n\n";
            errorMsg = errorMsg + "Por favor corrige la cantidad.";

            ShowMessage(errorMsg);
            return; // FRENA TODO
		}
        // ----------------------------------

        // 2. OBTENER PRECIO (Con la limpieza que arreglamos antes)
        String sPrecio = GridProductos->Cells[1][i];

        // Limpieza de signo $ y espacios
        sPrecio = StringReplace(sPrecio, "$", "", TReplaceFlags() << rfReplaceAll);
        sPrecio = sPrecio.Trim();

        // Arreglo de punto/coma según tu Windows
        if (FormatSettings.DecimalSeparator == ',') {
             sPrecio = StringReplace(sPrecio, ".", ",", TReplaceFlags() << rfReplaceAll);
        }
        else if (FormatSettings.DecimalSeparator == '.') {
             sPrecio = StringReplace(sPrecio, ",", ".", TReplaceFlags() << rfReplaceAll);
        }

        double precio = StrToFloatDef(sPrecio, 0);

        // 3. CALCULAR SUBTOTAL
        double subtotal = precio * cantidad;
        granTotal += subtotal;
        comproAlgo = true;

        // 4. AGREGAR AL RESUMEN
        String nombre = GridProductos->Cells[0][i];
        mensajeResumen += nombre + "\n";
        mensajeResumen += "   " + IntToStr(cantidad) + " x $" + FormatFloat("0.00", precio)
                       + " = $" + FormatFloat("0.00", subtotal) + "\n";
    }

    if (!comproAlgo) {
        ShowMessage("No has puesto cantidad en ningún producto.");
        return;
    }

    // 5. MOSTRAR LA VENTANA FINAL
    mensajeResumen += "\n-----------------------------------\n";
    mensajeResumen += "TOTAL A PAGAR: $" + FormatFloat("0.00", granTotal);

    ShowMessage(mensajeResumen);
}

//---------------------------------------------------------------------------

