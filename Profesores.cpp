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


	for (int i = ScrollBoxProfesores->ControlCount - 1; i >= 0; i--) {
		delete ScrollBoxProfesores->Controls[i];
	}


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


					String id = profe->GetValue("id")->Value();
					String nombre = profe->GetValue("nombre")->Value();
					String apellido = profe->GetValue("apellido")->Value();
					String dni = profe->GetValue("dni")->Value();
					String contrasena = profe->GetValue("contrasena")->Value();

					// Limpieza del email (quitar saltos de linea si hay)
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
					btnEliminar->Parent = header; btnEliminar->Caption = "Eliminar";
					btnEliminar->Width = 60; btnEliminar->Align = alRight;
					btnEliminar->AlignWithMargins = true; btnEliminar->Visible = false;
					btnEliminar->Tag = 30;
					btnEliminar->OnClick = ClickEliminar;

					TButton *btnAgAlumno = new TButton(header);
					btnAgAlumno->Parent = header; btnAgAlumno->Caption = "Alumnos";
					btnAgAlumno->Width = 70;
					btnAgAlumno->Align = alRight;
					btnAgAlumno->AlignWithMargins = true;
					btnAgAlumno->Visible = false;
					btnAgAlumno->Tag = 40;
					btnAgAlumno->OnClick = ClickAgregarAlumnoAProfesor;

					// --- TITULO (AQUÍ SOLO NOMBRE Y APELLIDO, SIN EMAIL) ---
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

					// --- CAMPOS (EL EMAIL ESTÁ AQUÍ Y ES EDITABLE) ---
					// Se añaden de abajo hacia arriba visualmente
					CrearCampo("Pass", contrasena, 6, false); // Oculto
					CrearCampo("ID", id, 5, false);           // Oculto

					// Visibles:
					CrearCampo("Email", email, 4, true);      // <--- AQUÍ ESTÁ EL EMAIL EDITABLE
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

	// Buscar panel detalles (Tag 99)
	TPanel *detalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) detalles = (TPanel*)fila->Controls[i];
	}

	bool abrir = (fila->Height == 40);

	if (abrir)
	{
		// 1. Mostrar panel de detalles
		btn->Caption = "^";
		if(detalles) detalles->Visible = true;

		// ---------------------------------------------------------
		// CARGAR ALUMNOS SI NO EXISTEN (Tag 500)
		// ---------------------------------------------------------
		bool listaYaCargada = false;
		if(detalles) {
			for(int k=0; k < detalles->ControlCount; k++) {
				if(detalles->Controls[k]->Tag == 500) listaYaCargada = true;
			}
		}

		if(detalles && !listaYaCargada)
		{
			// A. Obtener ID
			String idProfesor = "";
			for(int k=0; k < detalles->ControlCount; k++) {
				TEdit *ed = dynamic_cast<TEdit*>(detalles->Controls[k]);
				if(ed && ed->Tag == 5) idProfesor = ed->Text;
			}

			// B. Etiqueta "Alumnos Asignados"
			TLabel *lblAlumnos = new TLabel(detalles);
			lblAlumnos->Parent = detalles;
			lblAlumnos->Align = alTop;
			lblAlumnos->Top = 1000; // Forzar al fondo
			lblAlumnos->Caption = "Alumnos Asignados:";
			lblAlumnos->Font->Style = TFontStyles() << fsBold;
			lblAlumnos->Margins->Top = 25;
			lblAlumnos->Margins->Bottom = 5;
			lblAlumnos->AlignWithMargins = true;

			// C. Crear Lista con Scroll (ListBox)
			TListBox *lb = new TListBox(detalles);
			lb->Parent = detalles;
			lb->Align = alTop;
			lb->Top = 1001; // Forzar debajo del label

			// --- AQUÍ ESTÁ EL TRUCO VISUAL ---
			lb->Height = 150;     // Altura fija: si hay más alumnos, sale scroll interno
			lb->ItemHeight = 20;  // Altura cómoda por renglón
			lb->Tag = 500;
			lb->AlignWithMargins = true;
			// ---------------------------------

			// D. Llamar al PHP
			if(!idProfesor.IsEmpty()) {
				String body = "profesor_id=" + idProfesor;
				String json = HttpPost(L"/gimnasio_api/Admin/consultar_alumnos_de_profesor.php", body);

				TJSONObject* jsonRoot = (TJSONObject*)TJSONObject::ParseJSONValue(json);
				if(jsonRoot) {
					try {
						TJSONArray* arr = (TJSONArray*)jsonRoot->GetValue("data");
						if(arr && !arr->Null) {
							for(int i=0; i < arr->Count; i++) {
								TJSONObject* obj = (TJSONObject*)arr->Items[i];
								String nom = obj->GetValue("nombre")->Value();
								String ape = obj->GetValue("apellido")->Value();
								lb->Items->Add(" - " + nom + " " + ape);
							}
						}
						if(lb->Items->Count == 0) lb->Items->Add("(Sin alumnos asignados)");
					}
					__finally {
						delete jsonRoot;
					}
				}
			}
		}

		// ---------------------------------------------------------
		// 2. CALCULAR ALTURA AUTOMÁTICA (Para que nada se tape)
		// ---------------------------------------------------------
		int nuevaAltura = header->Height; // Empezamos con la cabecera (40)

		if (detalles) {
			// Sumamos un poco de padding base del panel
			nuevaAltura += detalles->Padding->Top + detalles->Padding->Bottom + 20;

			// Sumamos la altura de cada hijo visible dentro de detalles
			for(int i=0; i < detalles->ControlCount; i++) {
				TControl *child = detalles->Controls[i];
				if(child->Visible && child->Align == alTop) {
					nuevaAltura += child->Height;
					nuevaAltura += child->Margins->Top;
					nuevaAltura += child->Margins->Bottom;
				}
			}
		}

		// Asignamos la altura calculada + un pequeño margen de seguridad
		fila->Height = nuevaAltura + 10;
		// ---------------------------------------------------------

	}
	else
	{
		// CERRAR
		if(detalles) detalles->Visible = false;
		fila->Height = 40;
		btn->Caption = "v";
	}

	// Mostrar botones de cabecera
	for(int i=0; i<header->ControlCount; i++) {
		int t = header->Controls[i]->Tag;
		if(t == 10 || t == 20 || t == 30 || t == 40) {
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

	// 1. Mostrar el botón "Guardar"
	for(int i=0; i<header->ControlCount; i++) {
		if(header->Controls[i]->Tag == 20) header->Controls[i]->Visible = true;
	}

	// 2. Buscar el panel de detalles
	TPanel *pnlDetalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) pnlDetalles = (TPanel*)fila->Controls[i];
	}

	// 3. Convertir los TEdit VISIBLES en campos editables
	if(pnlDetalles) {
		bool primerFoco = true;

		for(int i=0; i<pnlDetalles->ControlCount; i++) {
			TEdit *ed = dynamic_cast<TEdit*>(pnlDetalles->Controls[i]);

			// AQUÍ ESTÁ LA CLAVE: Si el email es visible, se vuelve editable
			if(ed && ed->Visible) {
				ed->ReadOnly = false;       // Permite escribir
				ed->BorderStyle = bsSingle; // Borde visual
				ed->Color = clWindow;       // Fondo blanco

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

	TPanel *pnlDetalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) pnlDetalles = (TPanel*)fila->Controls[i];
	}

	if (pnlDetalles)
	{
		String id, nombre, apellido, dni, email, contrasena;

		// 2. Leer datos
		for (int i = 0; i < pnlDetalles->ControlCount; i++)
		{
			TEdit *ed = dynamic_cast<TEdit*>(pnlDetalles->Controls[i]);
			if (ed)
			{
				switch (ed->Tag) {
					case 1: nombre = ed->Text; break;
					case 2: apellido = ed->Text; break;
					case 3: dni = ed->Text; break;
					case 4: email = ed->Text; break; // RECUPERA EL EMAIL EDITADO
					case 5: id = ed->Text; break;
					case 6: contrasena = ed->Text; break;
				}

				if(ed->Visible) {
					ed->ReadOnly = true;
					ed->BorderStyle = bsNone;
					ed->Color = clWhite;
				}
			}
		}

		if (id.IsEmpty()) return;

		// 3. Enviar al servidor
		String body = "id=" + id +
					  "&nombre=" + nombre +
					  "&apellido=" + apellido +
					  "&dni=" + dni +
					  "&email=" + email +
					  "&contrasena=" + contrasena;

		String respuesta = HttpPost(L"/gimnasio_api/Admin/guardar_cambios_profesor.php", body);
		ShowMessage(respuesta);

		// 4. ACTUALIZAR TÍTULO (SIN EMAIL, SOLO NOMBRE Y APELLIDO)
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

		String mensaje = "¿Seguro que quieres eliminar al profesor: " + nombre + " " + apellido + "?";

		if(MessageDlg(mensaje, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
		{
			String body = "id=" + id;
			String respuesta = HttpPost(L"/gimnasio_api/Admin/eliminar_profesor.php", body);

			if (respuesta.Pos("success") > 0)
			{
				ShowMessage("Profesor eliminado correctamente.");
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




    void __fastcall TProfesores::ClickAgregarAlumnoAProfesor(TObject *Sender)
{
	// --- PASO 1: OBTENER EL ID DEL PROFESOR ACTUAL ---
	TButton *btn = (TButton*)Sender;
	TPanel *header = (TPanel*)btn->Parent;
	TPanel *fila = (TPanel*)header->Parent;

	String idProfesor = "";
	String nombreProfesor = "";

	// Buscamos el panel de detalles para sacar el ID y luego buscar la Lista
	TPanel *pnlDetalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) pnlDetalles = (TPanel*)fila->Controls[i];
	}

	if(pnlDetalles) {
		for(int i=0; i<pnlDetalles->ControlCount; i++) {
			TEdit *ed = dynamic_cast<TEdit*>(pnlDetalles->Controls[i]);
			if(ed) {
				if (ed->Tag == 5) idProfesor = ed->Text;     // ID
				if (ed->Tag == 1) nombreProfesor = ed->Text; // Nombre
			}
		}
	}

	if (idProfesor.IsEmpty()) {
		ShowMessage("Error: No se pudo identificar al profesor.");
		return;
	}

	// --- PASO 2: CREAR VENTANA DE ALUMNOS (SELECCION) ---
	TForm *ventana = new TForm(this);
	ventana->Caption = "Asignar alumno a: " + nombreProfesor;
	ventana->Width = 400;
	ventana->Height = 500;
	ventana->Position = poMainFormCenter;
	ventana->BorderStyle = bsDialog;

	TLabel *lblInfo = new TLabel(ventana);
	lblInfo->Parent = ventana;
	lblInfo->Left = 20; lblInfo->Top = 10;
	lblInfo->Caption = "Seleccione un alumno de la lista:";
	lblInfo->Font->Style = TFontStyles() << fsBold;

	TListBox *listaAlumnos = new TListBox(ventana);
	listaAlumnos->Parent = ventana;
	listaAlumnos->SetBounds(20, 35, 345, 350);

	TButton *btnAsignar = new TButton(ventana);
	btnAsignar->Parent = ventana;
	btnAsignar->Caption = "ASIGNAR A ESTE PROFESOR";
	btnAsignar->SetBounds(20, 400, 345, 40);
	btnAsignar->ModalResult = mrOk;

	// --- PASO 3: CARGAR LISTA DE TODOS LOS ALUMNOS ---
	String json = HttpPost(L"/gimnasio_api/Admin/consultar_lista_alumnos.php", "");
	TJSONObject* jsonRoot = (TJSONObject*)TJSONObject::ParseJSONValue(json);

	if (jsonRoot) {
		try {
			TJSONArray* dataArray = (TJSONArray*)jsonRoot->GetValue("data");
			if (dataArray && !dataArray->Null) {
				for (int i = 0; i < dataArray->Count; i++) {
					TJSONObject* alum = (TJSONObject*)dataArray->Items[i];
					String aId = alum->GetValue("id")->Value();
					String aNom = alum->GetValue("nombre")->Value();
					String aApe = alum->GetValue("apellido")->Value();
					listaAlumnos->Items->Add(aId + " - " + aNom + " " + aApe);
				}
			}
		}
		__finally {
			delete jsonRoot;
		}
	}

	// --- PASO 4: MOSTRAR VENTANA Y PROCESAR ---
	if (ventana->ShowModal() == mrOk)
	{
		if (listaAlumnos->ItemIndex != -1)
		{
			String seleccion = listaAlumnos->Items->Strings[listaAlumnos->ItemIndex];
			int posGuion = seleccion.Pos("-");
			if (posGuion > 0)
			{
				String idAlumno = seleccion.SubString(1, posGuion - 1).Trim();
				String body = "profesor_id=" + idProfesor + "&alumno_id=" + idAlumno;
				String respuesta = HttpPost(L"/gimnasio_api/Admin/asignar_alumno.php", body);

				if (respuesta.Pos("success") > 0)
				{
					ShowMessage("¡Alumno asignado correctamente!");

					// =========================================================
					// NUEVO: ACTUALIZAR LA LISTA VISUAL AL INSTANTE
					// =========================================================

					// 1. Buscar el ListBox (Tag 500) dentro de pnlDetalles
					TListBox *lbProfesor = NULL;
					if (pnlDetalles) {
						for(int k=0; k < pnlDetalles->ControlCount; k++) {
							if (pnlDetalles->Controls[k]->Tag == 500) {
								lbProfesor = dynamic_cast<TListBox*>(pnlDetalles->Controls[k]);
							}
						}
					}

					// 2. Si encontramos la lista, la recargamos
					if (lbProfesor)
					{
						lbProfesor->Items->Clear(); // Limpiar lista vieja

						// Llamada al servidor para traer los alumnos actualizados de ESTE profesor
						String bodyRefresh = "profesor_id=" + idProfesor;
						String jsonRef = HttpPost(L"/gimnasio_api/Admin/consultar_alumnos_de_profesor.php", bodyRefresh);

						TJSONObject* jsonRootRef = (TJSONObject*)TJSONObject::ParseJSONValue(jsonRef);
						if(jsonRootRef) {
							try {
								TJSONArray* arr = (TJSONArray*)jsonRootRef->GetValue("data");
								if(arr && !arr->Null) {
									for(int i=0; i < arr->Count; i++) {
										TJSONObject* obj = (TJSONObject*)arr->Items[i];
										String nom = obj->GetValue("nombre")->Value();
										String ape = obj->GetValue("apellido")->Value();
										lbProfesor->Items->Add(" - " + nom + " " + ape);
									}
								}

								if(lbProfesor->Items->Count == 0) lbProfesor->Items->Add("(Sin alumnos asignados)");
							}
							__finally {
								delete jsonRootRef;
							}
						}
					}
					// =========================================================
				}
				else {
					ShowMessage("Error al asignar: " + respuesta);
				}
			}
		}
		else {
			ShowMessage("No seleccionaste ningún alumno.");
		}
	}
	delete ventana;
}





void __fastcall TProfesores::AgregarProfesorClick(TObject *Sender)
{

	TForm *ventana = new TForm(this);
	ventana->Caption = "Nuevo Profesor";
	ventana->Width = 350;
	ventana->Height = 450;
	ventana->Position = poMainFormCenter;
	ventana->BorderStyle = bsDialog;


	TEdit *eNombre = new TEdit(ventana);
	eNombre->Parent = ventana;
	eNombre->Left = 40; eNombre->Top = 30; eNombre->Width = 250;
	eNombre->TextHint = "Nombre";


	TEdit *eApellido = new TEdit(ventana);
	eApellido->Parent = ventana;
	eApellido->Left = 40; eApellido->Top = 80; eApellido->Width = 250;
	eApellido->TextHint = "Apellido";


	TEdit *eDNI = new TEdit(ventana);
	eDNI->Parent = ventana;
	eDNI->Left = 40; eDNI->Top = 130; eDNI->Width = 250;
	eDNI->TextHint = "DNI";


	TEdit *eEmail = new TEdit(ventana);
	eEmail->Parent = ventana;
	eEmail->Left = 40; eEmail->Top = 180; eEmail->Width = 250;
	eEmail->TextHint = "Email";


	TEdit *ePass = new TEdit(ventana);
	ePass->Parent = ventana;
	ePass->Left = 40; ePass->Top = 230; ePass->Width = 250;
	ePass->TextHint = "Contraseña";

	//  boton guardar----------
	TButton *btnGuardar = new TButton(ventana);
	btnGuardar->Parent = ventana;
	btnGuardar->Left = 40; btnGuardar->Top = 300;
	btnGuardar->Width = 250; btnGuardar->Height = 45;
	btnGuardar->Caption = "GUARDAR PROFESOR";
	btnGuardar->ModalResult = mrOk;

	//
	if (ventana->ShowModal() == mrOk)
	{

		if(eNombre->Text.IsEmpty() || eApellido->Text.IsEmpty() || eDNI->Text.IsEmpty()) {
			ShowMessage("Faltan datos obligatorios (Nombre, Apellido, DNI).");
		}
		else
		{

			String body = "nombre=" + eNombre->Text +
						  "&apellido=" + eApellido->Text +
						  "&dni=" + eDNI->Text +
						  "&email=" + eEmail->Text +
						  "&contrasena=" + ePass->Text;


			String resp = HttpPost(L"/gimnasio_api/Admin/crear_profesor.php", body);

			if (resp.Pos("Duplicate") > 0 && resp.Pos("dni") > 0)
			{
				ShowMessage("Error: Ya existe un profesor con ese DNI.");
			}
			else if (resp.Pos("success") > 0)
			{
				ShowMessage("Profesor creado correctamente.");
				FormShow(this);
			}
			else
			{
				ShowMessage("Hubo un error: " + resp);
			}
		}
	}
	delete ventana;
}

//---------------------------------------------------------------------------


