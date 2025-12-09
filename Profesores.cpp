//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Profesores.h"
#include "Alumnos.h"
#include "Ventas.h"
#include "Productos.h"

#include <System.JSON.hpp>
#include "RequestHttp.h"
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

void __fastcall TProfesores::FormShow(TObject *Sender)
{
	if (this->CanFocus()) this->SetFocus();

	// 1. Limpiar lista anterior
	for (int i = ScrollBoxProfesores->ControlCount - 1; i >= 0; i--) {
		delete ScrollBoxProfesores->Controls[i];
	}

	// 2. Pedir datos al PHP de Profesores
	String json = HttpPost(L"/gimnasio_api/Admin/consultar_lista_profesores.php", "");

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
					TJSONObject* profe = (TJSONObject*)dataArray->Items[i];

					// --- EXTRAER DATOS ---
					String id = profe->GetValue("id")->Value();
					String nombre = profe->GetValue("nombre")->Value();
					String apellido = profe->GetValue("apellido")->Value();
					String dni = profe->GetValue("dni")->Value();
					String contrasena = profe->GetValue("contrasena")->Value();

					String email = profe->GetValue("email")->Value().Trim();
					int posSalto = email.Pos("\n");
					if(posSalto > 0) email = email.SubString(1, posSalto - 1);

					// --- FILA PRINCIPAL ---
					TPanel *fila = new TPanel(ScrollBoxProfesores);
					fila->Parent = ScrollBoxProfesores;
					fila->Align = alTop;
					fila->Height = 40;
					fila->Caption = "";
					fila->BevelOuter = bvNone;
					fila->Padding->SetBounds(5, 0, 5, 2);

					// --- CABECERA ---
					TPanel *header = new TPanel(fila);
					header->Parent = fila;
					header->Align = alTop;
					header->Height = 40;
					header->Caption = "";
					header->BevelOuter = bvNone;
					header->Color = clBtnFace;
					header->ParentBackground = false;

					// --- BOTONES ---

					// 1. Flecha
					TButton *btnFlecha = new TButton(header);
					btnFlecha->Parent = header; btnFlecha->Caption = "v"; btnFlecha->Width = 30;
					btnFlecha->Align = alRight; btnFlecha->AlignWithMargins = true;
					btnFlecha->OnClick = ClickMostrarMas; // <--- AQUÍ LA USAS

					// 2. Guardar
					TButton *btnGuardar = new TButton(header);
					btnGuardar->Parent = header; btnGuardar->Caption = "Guardar"; btnGuardar->Width = 60;
					btnGuardar->Align = alRight; btnGuardar->AlignWithMargins = true;
					btnGuardar->Visible = false; btnGuardar->Tag = 20;
					btnGuardar->OnClick = ClickGuardar;

					// 3. Editar
					TButton *btnEditar = new TButton(header);
					btnEditar->Parent = header; btnEditar->Caption = "Editar"; btnEditar->Width = 60;
					btnEditar->Align = alRight; btnEditar->AlignWithMargins = true;
					btnEditar->Visible = false; btnEditar->Tag = 10;
					btnEditar->OnClick = ClickEditar;

					// 4. Eliminar
					TButton *btnEliminar = new TButton(header);
					btnEliminar->Parent = header; btnEliminar->Caption = "Eliminar";
					btnEliminar->Width = 60; btnEliminar->Align = alRight;
					btnEliminar->AlignWithMargins = true; btnEliminar->Visible = false;
					btnEliminar->Tag = 30;
					btnEliminar->OnClick = ClickEliminar;

					TLabel *lblTitulo = new TLabel(header);
					lblTitulo->Parent = header;
					lblTitulo->Caption = "  " + nombre + " " + apellido;
					lblTitulo->Align = alClient;
					lblTitulo->Layout = tlCenter;
					lblTitulo->Font->Size = 10;
					lblTitulo->Font->Style = TFontStyles() << fsBold;

					// --- DETALLES ---
					TPanel *pnlDetalles = new TPanel(fila);
					pnlDetalles->Parent = fila;
					pnlDetalles->Align = alClient;
					pnlDetalles->Caption = "";
					pnlDetalles->BevelOuter = bvNone;
					pnlDetalles->Visible = false;
					pnlDetalles->Tag = 99;
					pnlDetalles->Color = clWhite;
					pnlDetalles->ParentBackground = false;
					pnlDetalles->Padding->SetBounds(20, 10, 20, 10);

					auto CrearCampo = [&](String titulo, String valor, int tagID, bool visible) {
						TEdit *e = new TEdit(pnlDetalles);
						e->Parent = pnlDetalles;
						e->Align = alTop;
						e->Text = valor;
						e->ReadOnly = true;
						e->BorderStyle = bsNone;
						e->Color = clWhite;
						e->Margins->Bottom = 15;
						e->AlignWithMargins = true;
						e->Tag = tagID;
						e->Visible = visible;
						if (!visible) e->TabStop = false;

						if(visible) {
							TLabel *l = new TLabel(pnlDetalles);
							l->Parent = pnlDetalles;
							l->Align = alTop;
							l->Caption = titulo;
							l->Font->Size = 8;
							l->Font->Color = clGray;
							l->Margins->Bottom = 2;
							l->AlignWithMargins = true;
						}
					};

					// --- CAMPOS ---
					CrearCampo("Pass", contrasena, 6, false);
					CrearCampo("ID", id, 5, false);
					CrearCampo("Email", email, 4, true);
					CrearCampo("DNI", dni, 3, true);
					CrearCampo("Apellido", apellido, 2, true);
					CrearCampo("Nombre", nombre, 1, true);
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
// ESTA ES LA FUNCIÓN QUE TE FALTABA
//---------------------------------------------------------------------------
void __fastcall TProfesores::ClickMostrarMas(TObject *Sender)
{
	TButton *btn = (TButton*)Sender;
	TPanel *header = (TPanel*)btn->Parent;
	TPanel *fila = (TPanel*)header->Parent;

	// Buscar panel detalles
	TPanel *detalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) detalles = (TPanel*)fila->Controls[i];
	}

	bool abrir = (fila->Height == 40);

	if (abrir)
	{
		fila->Height = 250; // Altura expandida
		btn->Caption = "^";
		if(detalles) detalles->Visible = true;
	}
	else
	{
		if(detalles) detalles->Visible = false;
		fila->Height = 40;
		btn->Caption = "v";
	}

	// Mostrar/Ocultar los botones segun Tags
	for(int i=0; i<header->ControlCount; i++) {
		int t = header->Controls[i]->Tag;
		if(t == 10 || t == 20 || t == 30) {
			header->Controls[i]->Visible = abrir;
		}
	}
}

//---------------------------------------------------------------------------
// FUNCIONES DE LOS BOTONES
//---------------------------------------------------------------------------
void __fastcall TProfesores::ClickEditar(TObject *Sender)
{
	TButton *btn = (TButton*)Sender;
	TPanel *header = (TPanel*)btn->Parent;
	TPanel *fila = (TPanel*)header->Parent;

	// 1. Mostrar el botón "Guardar" (Tag 20)
	// No ocultamos el botón Editar, para que el usuario sepa que está en modo edición
	for(int i=0; i<header->ControlCount; i++) {
		if(header->Controls[i]->Tag == 20) header->Controls[i]->Visible = true;
	}

	// 2. Buscar el panel de detalles (Tag 99)
	TPanel *pnlDetalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) pnlDetalles = (TPanel*)fila->Controls[i];
	}

	// 3. Convertir los TEdit en campos editables
	if(pnlDetalles) {
		bool primerFoco = true; // Para poner el cursor en el primer campo

		for(int i=0; i<pnlDetalles->ControlCount; i++) {
			// Intentamos ver si el control es un TEdit
			TEdit *ed = dynamic_cast<TEdit*>(pnlDetalles->Controls[i]);

			// Solo modificamos si es un TEdit y si es VISIBLE
			// (No queremos desbloquear el ID o el Password oculto accidentalmente si no se ven)
			if(ed && ed->Visible) {
				ed->ReadOnly = false;       // Permite escribir
				ed->BorderStyle = bsSingle; // Le pone borde de cajita (visual)
				ed->Color = clWindow;       // Le pone fondo blanco estándar

				// Poner el foco en el primer campo que encontremos (usualmente Nombre)
				if(primerFoco) {
					ed->SetFocus();
					primerFoco = false;
				}
			}
		}
	}
}

void __fastcall TProfesores::ClickGuardar(TObject *Sender)
{
	TButton *btn = (TButton*)Sender;
	TPanel *header = (TPanel*)btn->Parent;
	TPanel *fila = (TPanel*)header->Parent;

	// 1. Buscar el panel de detalles
	TPanel *pnlDetalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) pnlDetalles = (TPanel*)fila->Controls[i];
	}

	if (pnlDetalles)
	{
		// Variables exclusivas de Profesores (Sin fecha ni rutina)
		String id, nombre, apellido, dni, email, contrasena;

		// 2. Leer datos y BLOQUEAR LOS CAMPOS (Efecto visual de guardado)
		for (int i = 0; i < pnlDetalles->ControlCount; i++)
		{
			TEdit *ed = dynamic_cast<TEdit*>(pnlDetalles->Controls[i]);
			if (ed)
			{
				// Leemos el valor según los Tags que pusimos en FormShow
				switch (ed->Tag) {
					case 1: nombre = ed->Text; break;
					case 2: apellido = ed->Text; break;
					case 3: dni = ed->Text; break;
					case 4: email = ed->Text; break;
					case 5: id = ed->Text; break;
					case 6: contrasena = ed->Text; break;
				}

				// --- TRUCO VISUAL (Igual que en Alumnos) ---
				// Volvemos a poner los campos "lindos" (sin bordes y bloqueados)
				if(ed->Visible) {
					ed->ReadOnly = true;         // Bloqueamos escritura
					ed->BorderStyle = bsNone;    // Quitamos borde
					ed->Color = clWhite;         // Fondo blanco limpio
				}
			}
		}

		if (id.IsEmpty()) return;

		// 3. Enviar al servidor (Adaptado a tu PHP de Profesores)
		String body = "id=" + id +
					  "&nombre=" + nombre +
					  "&apellido=" + apellido +
					  "&dni=" + dni +
					  "&email=" + email +
					  "&contrasena=" + contrasena;

		// Llamada al script que ya probaste en Postman
		String respuesta = HttpPost(L"/gimnasio_api/Admin/guardar_cambios_profesor.php", body);
		ShowMessage(respuesta);

		// 4. ACTUALIZAR EL TITULO DEL PROFESOR (Por si cambiaste el nombre)
		for(int i=0; i<header->ControlCount; i++) {
			TLabel *lbl = dynamic_cast<TLabel*>(header->Controls[i]);
			if(lbl) {
				lbl->Caption = "  " + nombre + " " + apellido;
			}
		}
	}
}



void __fastcall TProfesores::ClickEliminar(TObject *Sender)
{
	ShowMessage("Funcionalidad: Aquí se eliminaría al profesor.");
}
