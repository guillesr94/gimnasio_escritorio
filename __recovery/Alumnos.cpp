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
	// 1. Limpiar lista
	for (int i = ScrollBoxAlumnos->ControlCount - 1; i >= 0; i--) {
		delete ScrollBoxAlumnos->Controls[i];
	}

	// 2. Pedir datos
	String json = HttpPost(L"/gimnasio_api/Admin/consultar_lista_alumnos.php", "");

	// 3. Procesar JSON
	TJSONObject* jsonRoot = (TJSONObject*)TJSONObject::ParseJSONValue(json);

	if (jsonRoot)
	{
		try
		{
			TJSONArray* dataArray = (TJSONArray*)jsonRoot->GetValue("data");

			if (dataArray && !dataArray->Null)
			{
				for (int i = 0; i < dataArray->Count; i++)
				{
					TJSONObject* alumno = (TJSONObject*)dataArray->Items[i];

					// DATOS
					String id = alumno->GetValue("id")->Value();
					String nombre = alumno->GetValue("nombre")->Value();
					String apellido = alumno->GetValue("apellido")->Value();
					String dni = alumno->GetValue("dni")->Value();

					String contrasena = "";
					if (alumno->GetValue("contrasena")) {
						contrasena = alumno->GetValue("contrasena")->Value();
					}

					String rawEmail = alumno->GetValue("email")->Value();
					int corte = rawEmail.Pos("\n");
					if (corte > 0) rawEmail = rawEmail.SubString(1, corte - 1);
					String email = rawEmail.Trim();

					String vencimiento = alumno->GetValue("fecha_vencimiento") ? alumno->GetValue("fecha_vencimiento")->Value() : "-";
					String rutina = alumno->GetValue("id_rutina") ? alumno->GetValue("id_rutina")->Value() : "0";

					// VISUAL
					TPanel *fila = new TPanel(ScrollBoxAlumnos);
					fila->Parent = ScrollBoxAlumnos;
					fila->Align = alTop;
					fila->Height = 40;
					fila->BevelOuter = bvNone;
					fila->Caption = "";
					fila->Padding->Bottom = 2;

					TPanel *header = new TPanel(fila);
					header->Parent = fila;
					header->Align = alTop;
					header->Height = 40;
					header->Caption = "";
					header->BevelOuter = bvNone;
					header->Color = clWhite;
					header->ParentBackground = false;

					TPanel *pnlDetalles = new TPanel(fila);
					pnlDetalles->Parent = fila;
					pnlDetalles->Align = alClient;
					pnlDetalles->BevelOuter = bvNone;
					pnlDetalles->Visible = false;
					pnlDetalles->Tag = 99;
					pnlDetalles->Padding->SetBounds(10, 10, 10, 10);

					// CAMPOS
					CrearCampo(pnlDetalles, "Contraseña", contrasena, 507);
					CrearCampo(pnlDetalles, "Email", email, 504);
					CrearCampo(pnlDetalles, "Rutina ID", rutina, 505);
					CrearCampo(pnlDetalles, "Vencimiento", vencimiento, 506);
					CrearCampo(pnlDetalles, "DNI", dni, 503);
					CrearCampo(pnlDetalles, "Apellido", apellido, 502);
					CrearCampo(pnlDetalles, "Nombre", nombre, 501);

					// Botón Flecha
					TButton *btnAccion = new TButton(header);
					btnAccion->Parent = header;
					btnAccion->Caption = "v";
					btnAccion->Width = 30;
					btnAccion->Align = alRight;
					btnAccion->AlignWithMargins = true;
					btnAccion->OnClick = ClickMostrarMas; // <--- Nombre correcto

					// Botones Acción
					AgregarBotones(header, id); // <--- Nombre correcto

					TLabel *lblNombre = new TLabel(header);
					lblNombre->Parent = header;
					lblNombre->Caption = "  " + nombre + " " + apellido;
					lblNombre->Align = alClient;
					lblNombre->Layout = tlCenter;
					lblNombre->Font->Size = 10;
					lblNombre->Font->Style = TFontStyles() << fsBold;
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

void __fastcall TAlumnos::CrearCampo(TWinControl *parent, String titulo, String valor, int tagId)
{
	TPanel *pnlCampo = new TPanel(parent);
	pnlCampo->Parent = parent;
	pnlCampo->Align = alTop;
	pnlCampo->Height = 25;
	pnlCampo->BevelOuter = bvNone;
	pnlCampo->Caption = "";
	pnlCampo->Padding->Bottom = 2;

	TLabel *lbl = new TLabel(pnlCampo);
	lbl->Parent = pnlCampo;
	lbl->Align = alLeft;
	lbl->Width = 90;
	lbl->Caption = titulo + ":";
	lbl->Layout = tlCenter;
	lbl->Font->Color = clGray;

	TEdit *edt = new TEdit(pnlCampo);
	edt->Parent = pnlCampo;
	edt->Align = alClient;
	edt->Text = valor;

	edt->ReadOnly = true;
	edt->BorderStyle = bsNone;
	edt->Color = clBtnFace;
	edt->Tag = tagId;
}

//---------------------------------------------------------------------------

void __fastcall TAlumnos::AgregarBotones(TPanel *header, String id)
{
	TButton *btnEliminar = new TButton(header);
	btnEliminar->Parent = header;
	btnEliminar->Caption = "X";
	btnEliminar->Width = 30;
	btnEliminar->Align = alRight;
	btnEliminar->AlignWithMargins = true;
	btnEliminar->OnClick = ClickEliminar;
	btnEliminar->Hint = id;
	btnEliminar->ShowHint = true;
	btnEliminar->Visible = false;
	btnEliminar->Tag = 20;

	TButton *btnEditar = new TButton(header);
	btnEditar->Parent = header;
	btnEditar->Caption = "Editar";
	btnEditar->Width = 60;
	btnEditar->Align = alRight;
	btnEditar->AlignWithMargins = true;
	btnEditar->OnClick = ClickEditar; // <--- Nombre correcto
	btnEditar->Hint = id;
	btnEditar->Visible = false;
	btnEditar->Tag = 10;
}

//---------------------------------------------------------------------------
// LÓGICA DE EDITAR Y GUARDAR
//---------------------------------------------------------------------------
void __fastcall TAlumnos::ClickEditar(TObject *Sender)
{
	TButton *btn = (TButton*)Sender;
	TPanel *header = (TPanel*)btn->Parent;
	TPanel *fila = (TPanel*)header->Parent;
	String alumnoID = btn->Hint;

	TPanel *pnlDetalles = NULL;
	for (int i = 0; i < fila->ControlCount; i++) {
		if (fila->Controls[i]->Tag == 99) {
			pnlDetalles = (TPanel*)fila->Controls[i];
			break;
		}
	}

	if (!pnlDetalles) return;

	if (btn->Caption == "Editar")
	{
		// ACTIVAR EDICIÓN
		for (int i = 0; i < pnlDetalles->ControlCount; i++)
		{
			TControl *hijo = pnlDetalles->Controls[i];
			if (hijo->InheritsFrom(__classid(TPanel)))
			{
				TPanel *pnlCampo = (TPanel*)hijo;
				for (int j = 0; j < pnlCampo->ControlCount; j++)
				{
					if (pnlCampo->Controls[j]->InheritsFrom(__classid(TEdit)))
					{
						TEdit *edt = (TEdit*)pnlCampo->Controls[j];
						if (edt->Tag >= 500)
						{
							edt->ReadOnly = false;
							edt->BorderStyle = bsSingle;
							edt->Color = clWhite;
						}
					}
				}
			}
		}
		btn->Caption = "Guardar";
	}
	else
	{
		// GUARDAR CAMBIOS
		String nombre, apellido, dni, email, contrasena, vencimiento, rutina;

		for (int i = 0; i < pnlDetalles->ControlCount; i++)
		{
			TControl *hijo = pnlDetalles->Controls[i];
			if (hijo->InheritsFrom(__classid(TPanel)))
			{
				TPanel *pnlCampo = (TPanel*)hijo;
				for (int j = 0; j < pnlCampo->ControlCount; j++)
				{
					if (pnlCampo->Controls[j]->InheritsFrom(__classid(TEdit)))
					{
						TEdit *edt = (TEdit*)pnlCampo->Controls[j];

						switch(edt->Tag) {
							case 501: nombre = edt->Text; break;
							case 502: apellido = edt->Text; break;
							case 503: dni = edt->Text; break;
							case 504: email = edt->Text; break;
							case 505: rutina = edt->Text; break;
							case 506: vencimiento = edt->Text; break;
							case 507: contrasena = edt->Text; break;
						}
					}
				}
			}
		}

		String datosPost = "id=" + alumnoID +
					       "&nombre=" + nombre +
					       "&apellido=" + apellido +
					       "&dni=" + dni +
					       "&email=" + email +
					       "&contrasena=" + contrasena +
					       "&fecha_vencimiento=" + vencimiento +
					       "&id_rutina=" + rutina;

		String respuesta = HttpPost(L"/gimnasio_api/Admin/guardar_cambios_alumno.php", datosPost);
		ShowMessage("Estado: " + respuesta);

		TimerRecargar->Enabled = true; // Activa el Timer para recargar
	}
}

//---------------------------------------------------------------------------
// TU FUNCIÓN DE TIMER
//---------------------------------------------------------------------------
void __fastcall TAlumnos::AlCompletarGuardado(TObject *Sender)
{
	TimerRecargar->Enabled = false;
	FormShow(this);
}
