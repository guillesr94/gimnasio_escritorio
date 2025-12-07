//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <System.JSON.hpp>

#include "Alumnos.h"
#include "Profesores.h"
#include "Alumnos.h"
#include "Ventas.h"
#include "Productos.h"

#include "RequestHttp.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAlumnos *Alumnos;
//---------------------------------------------------------------------------
__fastcall TAlumnos::TAlumnos(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TAlumnos::BtnVentanaProfesoresClick(TObject *Sender)
{
	  Profesores = new TProfesores(this);
	Profesores->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TAlumnos::BtnVentanaCLientesClick(TObject *Sender)
{
		 Alumnos = new TAlumnos(this);
	Alumnos->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TAlumnos::BtnVentanaVentasClick(TObject *Sender)
{
			 Ventas = new TVentas(this);
	Ventas->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TAlumnos::BtnVentanaProductosClick(TObject *Sender)
{
	 Productos = new TProductos(this);
	Productos->Show();


	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TAlumnos::FormShow(TObject *Sender)
{
	// 1. Limpiar la lista visual anterior para evitar duplicados
	// Recorremos hacia atrás para borrar los componentes hijos del ScrollBox
	for (int i = ScrollBoxAlumnos->ControlCount - 1; i >= 0; i--) {
		delete ScrollBoxAlumnos->Controls[i];
	}

	// 2. Pedir datos
	String json = HttpPost(
		L"/gimnasio_api/Admin/consultar_lista_alumnos.php",
		""
	);

	// 3. Procesar JSON y Generar controles dinámicos
	TJSONObject* jsonRoot = (TJSONObject*)TJSONObject::ParseJSONValue(json);

	if (jsonRoot)
	{
		try
		{
			TJSONArray* dataArray = (TJSONArray*)jsonRoot->GetValue("data");

			if (dataArray && !dataArray->Null)
			{
				// Recorremos cada alumno
				for (int i = 0; i < dataArray->Count; i++)
				{
					TJSONObject* alumno = (TJSONObject*)dataArray->Items[i];

					String nombre = alumno->GetValue("nombre")->Value();
					String apellido = alumno->GetValue("apellido")->Value();
					String dni = alumno->GetValue("dni")->Value();

					// --- CREACIÓN VISUAL DINÁMICA ---

					// A. Crear un Panel contenedor (la "fila")
					TPanel *fila = new TPanel(ScrollBoxAlumnos);
					fila->Parent = ScrollBoxAlumnos;
					fila->Align = alTop;   // Se apilan uno arriba del otro
					fila->Height = 40;     // Altura de la fila
					fila->Caption = "";    // Sin texto de fondo
					fila->BevelOuter = bvNone; // Sin borde 3D para que se vea plano
					fila->Padding->SetBounds(5, 5, 5, 5); // Margen interno

                    // B. Crear Botón Menos (-)
					TButton *btnMenos = new TButton(fila);
					btnMenos->Parent = fila;
					btnMenos->Caption = "-";
					btnMenos->Width = 30;
					btnMenos->Align = alRight; // Pegado a la derecha
					btnMenos->AlignWithMargins = true; // Para separarlo del otro botón
                    // btnMenos->Tag = i; // (Opcional) Útil luego para saber a quién borrar

					// C. Crear Botón Más (+)
					TButton *btnMas = new TButton(fila);
					btnMas->Parent = fila;
					btnMas->Caption = "+";
					btnMas->Width = 30;
					btnMas->Align = alRight; // Pegado a la derecha (antes del menos)

					// D. Crear Etiqueta con el Nombre
					TLabel *lblNombre = new TLabel(fila);
					lblNombre->Parent = fila;
					lblNombre->Caption = nombre + " " + apellido;
					lblNombre->Layout = tlCenter; // Centrar texto verticalmente
					lblNombre->Align = alClient;  // Ocupa todo el espacio restante a la izquierda
                    lblNombre->Font->Size = 10;   // Un poco más grande
				}
			}
		}
		__finally
		{
			delete jsonRoot;
		}
	}
}

//---------------------------------------------------------------------------

