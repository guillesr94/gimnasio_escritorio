//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <System.JSON.hpp>

#include "Alumnos.h"
#include "Profesores.h"
#include "Alumnos.h"
#include "Ventas.h"
#include "Productos.h"
#include <System.DateUtils.hpp>
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
	ScrollBoxAlumnos->Height = 400;


	if (this->CanFocus()) this->SetFocus();

	for (int i = ScrollBoxAlumnos->ControlCount - 1; i >= 0; i--) {
		delete ScrollBoxAlumnos->Controls[i];
	}


	String json = HttpPost(L"/gimnasio_api/Admin/consultar_lista_alumnos.php", "");
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


					String id = alumno->GetValue("id")->Value();
					String nombre = alumno->GetValue("nombre")->Value();
					String apellido = alumno->GetValue("apellido")->Value();
					String dni = alumno->GetValue("dni")->Value();
					String contrasena = alumno->GetValue("contrasena")->Value();
					String fecha_venc = alumno->GetValue("fecha_vencimiento")->Value();
					String id_rutina = alumno->GetValue("id_rutina")->Value();


					String email = alumno->GetValue("email")->Value().Trim();
					int posSalto = email.Pos("\n");
					if(posSalto > 0) email = email.SubString(1, posSalto - 1);


					bool estaVencido = false;
					try {
						String fechaLimpia = StringReplace(fecha_venc, "-", "/", TReplaceFlags() << rfReplaceAll);
						TDateTime fechaVencimientoDate = StrToDate(fechaLimpia);
						if (fechaVencimientoDate < Now()) {
							estaVencido = true;
						}
					}
					catch (...) { estaVencido = true; }


					TPanel *fila = new TPanel(ScrollBoxAlumnos);
					fila->Parent = ScrollBoxAlumnos;
					fila->Align = alTop;
					fila->Height = 40;
					fila->Caption = "";
					fila->BevelOuter = bvNone;
					fila->Padding->SetBounds(5, 0, 5, 2);


					TPanel *header = new TPanel(fila);
					header->Parent = fila;
					header->Align = alTop;
					header->Height = 40;
					header->Caption = "";
					header->BevelOuter = bvNone;
					header->Color = clBtnFace;
					header->ParentBackground = false;


					TButton *btnFlecha = new TButton(header);
					btnFlecha->Parent = header; btnFlecha->Caption = "v"; btnFlecha->Width = 30;
					btnFlecha->Align = alRight; btnFlecha->AlignWithMargins = true;
					btnFlecha->OnClick = ClickMostrarMas;

					TButton *btnGuardar = new TButton(header);
					btnGuardar->Parent = header; btnGuardar->Caption = "Guardar"; btnGuardar->Width = 60;
					btnGuardar->Align = alRight; btnGuardar->AlignWithMargins = true;
					btnGuardar->Visible = false; btnGuardar->Tag = 20;
					btnGuardar->OnClick = ClickGuardar;

					TButton *btnEditar = new TButton(header);
					btnEditar->Parent = header; btnEditar->Caption = "Editar"; btnEditar->Width = 60;
					btnEditar->Align = alRight; btnEditar->AlignWithMargins = true;
					btnEditar->Visible = false; btnEditar->Tag = 10;
					btnEditar->OnClick = ClickEditar;

					TButton *btnEliminar = new TButton(header);
					btnEliminar->Parent = header; btnEliminar->Caption = "Eliminar"; btnEliminar->Width = 60;
					btnEliminar->Align = alRight; btnEliminar->AlignWithMargins = true;
					btnEliminar->Visible = false; btnEliminar->Tag = 30;
					btnEliminar->OnClick = ClickEliminar;

					TLabel *lblTitulo = new TLabel(header);
					lblTitulo->Parent = header;
					lblTitulo->Caption = "  " + nombre + " " + apellido;
					lblTitulo->Align = alClient;
					lblTitulo->Layout = tlCenter;
					lblTitulo->Font->Size = 10;
					lblTitulo->Font->Style = TFontStyles() << fsBold;


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


					CrearCampo("Rutina", id_rutina, 8, false);


					TPanel *pnlRowFecha = new TPanel(pnlDetalles);
					pnlRowFecha->Parent = pnlDetalles;
					pnlRowFecha->Align = alTop;
					pnlRowFecha->Height = 45;
					pnlRowFecha->BevelOuter = bvNone;
					pnlRowFecha->Color = clWhite;
					pnlRowFecha->ParentBackground = false;
					pnlRowFecha->Margins->Bottom = 15;
					pnlRowFecha->AlignWithMargins = true;


						TLabel *lblF = new TLabel(pnlRowFecha);
						lblF->Parent = pnlRowFecha;
						lblF->Align = alTop;
						lblF->Caption = "Vencimiento Cuota";
						lblF->Font->Size = 8;
						lblF->Font->Color = clGray;
						lblF->Margins->Bottom = 2;
						lblF->AlignWithMargins = true;


						TPanel *pnlAbajo = new TPanel(pnlRowFecha);
						pnlAbajo->Parent = pnlRowFecha;
						pnlAbajo->Align = alClient;
						pnlAbajo->BevelOuter = bvNone;
						pnlAbajo->Color = clWhite;
						pnlAbajo->ParentBackground = false;


							TCheckBox *chk = new TCheckBox(pnlAbajo);
							chk->Parent = pnlAbajo;
							chk->Align = alRight;
							chk->Width = 20;
							chk->Caption = "";
							chk->Checked = !estaVencido;
							chk->OnClick = ClickRealizarPago;


							TLabel *lblEst = new TLabel(pnlAbajo);
							lblEst->Parent = pnlAbajo;
							lblEst->Align = alRight;
							lblEst->Layout = tlCenter;
							lblEst->AlignWithMargins = true;
							lblEst->Margins->Right = 10;
							lblEst->Font->Style = TFontStyles() << fsBold;
							lblEst->Tag = 50;

							if (estaVencido) {
								lblEst->Caption = "DEBE PAGAR";
								lblEst->Font->Color = clRed;
							} else {
								lblEst->Caption = "AL DÍA";
								lblEst->Font->Color = clGreen;
							}


							TEdit *edF = new TEdit(pnlAbajo);
							edF->Parent = pnlAbajo;
							edF->Align = alClient;
							edF->Text = fecha_venc;
							edF->ReadOnly = true;
							edF->BorderStyle = bsNone;
							edF->Tag = 7;


					CrearCampo("Pass", contrasena, 6, false);
					CrearCampo("ID", id, 5, false);
					CrearCampo("Email", email, 4, true);
					CrearCampo("DNI", dni, 3, true);
					CrearCampo("Apellido", apellido, 2, true);
					CrearCampo("Nombre", nombre, 1, true);
				}
			}
		}
		catch (...) {}
		delete jsonRoot;
	}
}
void __fastcall TAlumnos::ClickRealizarPago(TObject *Sender)
{
	TCheckBox *chk = dynamic_cast<TCheckBox*>(Sender);
	if (!chk) return;


	if (!chk->Checked) return;


	TPanel *pnlAbajo = dynamic_cast<TPanel*>(chk->Parent);
	if (!pnlAbajo) return;


	TEdit *editFecha = NULL;
	for(int i=0; i<pnlAbajo->ControlCount; i++) {
		TEdit *ed = dynamic_cast<TEdit*>(pnlAbajo->Controls[i]);
		if(ed && ed->Tag == 7) editFecha = ed;
	}

	if (editFecha) {
		try {

			TDateTime nuevaFecha = IncMonth(Now(), 1);


			editFecha->Text = FormatDateTime("yyyy-mm-dd", nuevaFecha);
		} catch(...) {}
	}

	for(int i=0; i<pnlAbajo->ControlCount; i++) {
		TLabel *lbl = dynamic_cast<TLabel*>(pnlAbajo->Controls[i]);
		if(lbl && lbl->Tag == 50) {
			lbl->Caption = "AL DÍA";
			lbl->Font->Color = clGreen;
		}
	}


}




void __fastcall TAlumnos::ClickMostrarMas(TObject *Sender)
{
	TButton *btn = (TButton*)Sender;
	TPanel *header = (TPanel*)btn->Parent;
	TPanel *fila = (TPanel*)header->Parent;

	TPanel *detalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) detalles = (TPanel*)fila->Controls[i];
	}

	bool abrir = (fila->Height == 40);

	if (abrir)
	{
		fila->Height = 420;
		btn->Caption = "^";
		if(detalles) detalles->Visible = true;
	}
	else
	{
		if(detalles) detalles->Visible = false;
		fila->Height = 40;
		btn->Caption = "v";
	}

	for(int i=0; i<header->ControlCount; i++) {
		int t = header->Controls[i]->Tag;
		if(t == 10 || t == 20 || t == 30) {
			header->Controls[i]->Visible = abrir;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TAlumnos::ClickEditar(TObject *Sender)
{
	TButton *btn = (TButton*)Sender;
	TPanel *header = (TPanel*)btn->Parent;
	TPanel *fila = (TPanel*)header->Parent;


	for(int i=0; i<header->ControlCount; i++) {
		if(header->Controls[i]->Tag == 20) header->Controls[i]->Visible = true;
	}


	TPanel *pnlDetalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) pnlDetalles = (TPanel*)fila->Controls[i];
	}


	if(pnlDetalles) {
		bool primerFoco = true;
		for(int i=0; i<pnlDetalles->ControlCount; i++) {
			TEdit *ed = dynamic_cast<TEdit*>(pnlDetalles->Controls[i]);


			if(ed && ed->Visible) {
				ed->ReadOnly = false;
				ed->BorderStyle = bsSingle;
				ed->Color = clWindow;


				if(primerFoco) {
					ed->SetFocus();
					primerFoco = false;
				}
			}
		}
	}
}
void __fastcall TAlumnos::ClickGuardar(TObject *Sender)
{
	TButton *btn = (TButton*)Sender;
	TPanel *header = (TPanel*)btn->Parent;
	TPanel *fila = (TPanel*)header->Parent;

	TPanel *pnlDetalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) pnlDetalles = (TPanel*)fila->Controls[i];
	}

	if (pnlDetalles)
	{
		String id, nombre, apellido, dni, email, pass, fecha, rutina;

		// 1. BUSQUEDA SUPERFICIAL
		for (int i = 0; i < pnlDetalles->ControlCount; i++)
		{
			TEdit *ed = dynamic_cast<TEdit*>(pnlDetalles->Controls[i]);
			if (ed)
			{
				switch (ed->Tag) {
					case 1: nombre = ed->Text; break;
					case 2: apellido = ed->Text; break;
					case 3: dni = ed->Text; break;
					case 4: email = ed->Text; break;
					case 5: id = ed->Text; break;
					case 6: pass = ed->Text; break;
					case 8: rutina = ed->Text; break;
				}

				// Restaurar estilo visual
				if(ed->Visible) {
					ed->ReadOnly = true;
					ed->BorderStyle = bsNone;
					ed->Color = clWhite;
				}
			}
		}

		// 2. BUSQUEDA PROFUNDA (Para la FECHA)
		for(int i=0; i < pnlDetalles->ControlCount; i++) {
			TPanel *panelHijo = dynamic_cast<TPanel*>(pnlDetalles->Controls[i]);
			if(panelHijo) {
				for(int j=0; j < panelHijo->ControlCount; j++) {
					TPanel *panelNieto = dynamic_cast<TPanel*>(panelHijo->Controls[j]);
					if(panelNieto) {
						for(int k=0; k < panelNieto->ControlCount; k++) {
							TEdit *edFecha = dynamic_cast<TEdit*>(panelNieto->Controls[k]);
							if(edFecha && edFecha->Tag == 7) {
								fecha = edFecha->Text;
							}
						}
					}
				}
			}
		}

		if (fecha.IsEmpty()) {
			ShowMessage("Error: No se encuentra la fecha o está vacía.");
			return;
		}

		if (id.IsEmpty()) return;

		// ENVIAR A PHP
		String body = "id=" + id + "&nombre=" + nombre + "&apellido=" + apellido +
					  "&dni=" + dni + "&email=" + email + "&contrasena=" + pass +
					  "&fecha_vencimiento=" + fecha + "&id_rutina=" + rutina;

		String respuesta = HttpPost(L"/gimnasio_api/Admin/guardar_cambios_alumno.php", body);

		// -------------------------------------------------------------
		// CAMBIO REALIZADO AQUÍ:
		// Verificamos si la respuesta del servidor contiene la palabra "success"
		// -------------------------------------------------------------
		if (respuesta.Pos("success") > 0)
		{
			ShowMessage("Guardado correctamente");
		}
		else
		{
			// Si falló, mostramos qué pasó para poder corregirlo
			ShowMessage("Hubo un error al guardar: " + respuesta);
		}
		// -------------------------------------------------------------

		// Actualizar titulo visual
		for(int i=0; i<header->ControlCount; i++) {
			TLabel *lbl = dynamic_cast<TLabel*>(header->Controls[i]);
			if(lbl) {
				lbl->Caption = "  " + nombre + " " + apellido;
			}
		}
	}
}






void __fastcall TAlumnos::AgregarAlumnoClick(TObject *Sender)
{

	TForm *ventana = new TForm(this);
	ventana->Caption = "Nuevo Alumno";
	ventana->Width = 350;
	ventana->Height = 450;
	ventana->Position = poMainFormCenter;
	ventana->BorderStyle = bsDialog;

	// --- inputs

	TEdit *eNombre = new TEdit(ventana);
	eNombre->Parent = ventana;
	eNombre->Left = 40; eNombre->Top = 30; eNombre->Width = 250; eNombre->TextHint = "Nombre";


	TEdit *eApellido = new TEdit(ventana);
	eApellido->Parent = ventana;
	eApellido->Left = 40; eApellido->Top = 80; eApellido->Width = 250; eApellido->TextHint = "Apellido";


	TEdit *eDNI = new TEdit(ventana);
	eDNI->Parent = ventana;
	eDNI->Left = 40; eDNI->Top = 130; eDNI->Width = 250; eDNI->TextHint = "DNI";


	TEdit *eEmail = new TEdit(ventana);
	eEmail->Parent = ventana;
	eEmail->Left = 40; eEmail->Top = 180; eEmail->Width = 250; eEmail->TextHint = "Email";


	TEdit *ePass = new TEdit(ventana);
	ePass->Parent = ventana;
	ePass->Left = 40; ePass->Top = 230; ePass->Width = 250; ePass->TextHint = "Contraseña";


	TComboBox *cbProfes = new TComboBox(ventana);
	cbProfes->Parent = ventana;
	cbProfes->Left = 40;
	cbProfes->Top = 280;
	cbProfes->Width = 250;
	cbProfes->TextHint = "Seleccionar Profesor";
	cbProfes->Style = csDropDownList;


	try {
		String jsonProfes = HttpPost(L"/gimnasio_api/Admin/consultar_lista_profesores.php", "");
		TJSONObject* jsonRoot = (TJSONObject*)TJSONObject::ParseJSONValue(jsonProfes);

		if (jsonRoot) {
			TJSONArray* data = (TJSONArray*)jsonRoot->GetValue("data");
			if (data && !data->Null) {
				cbProfes->Items->Clear();

				for(int i = 0; i < data->Count; i++) {
					TJSONObject* profe = (TJSONObject*)data->Items[i];

					String nom = profe->GetValue("nombre")->Value();
					String ape = profe->GetValue("apellido")->Value();


					int idProfe = profe->GetValue("id")->Value().ToInt();


					cbProfes->Items->AddObject(nom + " " + ape, (TObject*)(INT_PTR)idProfe);
				}
			}
			delete jsonRoot;
		}
	}
	catch (...) {
		cbProfes->TextHint = "Error al cargar profes";
	}


	TButton *btnGuardar = new TButton(ventana);
	btnGuardar->Parent = ventana;
	btnGuardar->Left = 40;
	btnGuardar->Top = 340;
	btnGuardar->Width = 250;
	btnGuardar->Height = 45;
	btnGuardar->Caption = "GUARDAR ALUMNO";
	btnGuardar->ModalResult = mrOk;



	if (ventana->ShowModal() == mrOk)
	{
		if(eNombre->Text.IsEmpty() || eApellido->Text.IsEmpty() || eDNI->Text.IsEmpty()) {
			ShowMessage("Faltan datos obligatorios (Nombre, Apellido, DNI).");
		}
		else
		{

			String idProfesorSeleccionado = "0";

			if (cbProfes->ItemIndex >= 0) {

				int id = (int)(INT_PTR)cbProfes->Items->Objects[cbProfes->ItemIndex];
				idProfesorSeleccionado = IntToStr(id);
			}


			String body = "nombre=" + eNombre->Text +
						  "&apellido=" + eApellido->Text +
						  "&dni=" + eDNI->Text +
						  "&email=" + eEmail->Text +
						  "&contrasena=" + ePass->Text +
						  "&id_profesor=" + idProfesorSeleccionado;

			// usamos el php
			String resp = HttpPost(L"/gimnasio_api/Admin/crear_alumno.php", body);

			if (resp.Pos("Duplicate") > 0 && resp.Pos("dni") > 0) {
				ShowMessage("Error: Pusiste el mismo DNI que otro cliente.");
			}
			else if (resp.Pos("success") > 0) {
				ShowMessage("Alumno creado correctamente.");
				FormShow(this);
			}
			else {
				ShowMessage("Hubo un error: " + resp);
			}
		}
	}
	delete ventana;
}



void __fastcall TAlumnos::ClickEliminar(TObject *Sender)
{
	TButton *btn = (TButton*)Sender;
	TPanel *header = (TPanel*)btn->Parent;
	TPanel *fila = (TPanel*)header->Parent;


	TPanel *pnlDetalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) pnlDetalles = (TPanel*)fila->Controls[i];
	}

	if(pnlDetalles) {
		String id = "";
		String nombre = "";
		String apellido = "";


		for(int i=0; i<pnlDetalles->ControlCount; i++) {
			TEdit *ed = dynamic_cast<TEdit*>(pnlDetalles->Controls[i]);
			if(ed) {
				if (ed->Tag == 1) nombre = ed->Text;
				if (ed->Tag == 2) apellido = ed->Text;
				if (ed->Tag == 5) id = ed->Text.Trim();
			}
		}

		if(id.IsEmpty()) return;


		String mensaje = "¿Seguro que quieres eliminar al alumno: " + nombre + " " + apellido + "?";

		if(MessageDlg(mensaje, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
		{

			String body = "id=" + id;


			String respuesta = HttpPost(L"/gimnasio_api/Admin/eliminar_alumno.php", body);


			if (respuesta.Pos("success") > 0)
			{
				ShowMessage("Alumno eliminado correctamente.");


				fila->Visible = false;
				fila->Parent = NULL;

			}
			else
			{
				ShowMessage("Error del servidor: " + respuesta);
			}
		}
	}
}

