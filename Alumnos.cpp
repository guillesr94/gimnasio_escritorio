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
	// 0. TRUCO PARA EVITAR EL ERROR "CANNOT FOCUS":
	// Pasamos el foco al formulario principal antes de borrar los botones.
	if (this->CanFocus()) this->SetFocus();

	// 1. Limpiar lista anterior
	for (int i = ScrollBoxAlumnos->ControlCount - 1; i >= 0; i--) {
		delete ScrollBoxAlumnos->Controls[i];
	}

	// 2. Pedir datos
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

					// --- EXTRAER DATOS ---
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

					// --- FILA PRINCIPAL ---
					TPanel *fila = new TPanel(ScrollBoxAlumnos);
					fila->Parent = ScrollBoxAlumnos;
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

					// Botones
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

					TLabel *lblTitulo = new TLabel(header);
					lblTitulo->Parent = header;
					lblTitulo->Caption = "  " + nombre + " " + apellido;
					lblTitulo->Align = alClient;
					lblTitulo->Layout = tlCenter;
					lblTitulo->Font->Size = 10;
					lblTitulo->Font->Style = TFontStyles() << fsBold;

					// --- PANEL DE DETALLES ---
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

					// FUNCION CREAR CAMPO
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

						// --- CORRECCION CRÍTICA AQUÍ ---
						e->Visible = visible;
						if (!visible) {
							e->TabStop = false; // IMPORTANTE: Si es invisible, que no agarre foco nunca
						}
						// ------------------------------

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

					// --- CREACION (Orden Inverso) ---
					CrearCampo("Rutina", id_rutina, 8, false);
					CrearCampo("Fecha", fecha_venc, 7, false);
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
// 2. LOGICA DE DESPLEGAR (AUMENTADA LA ALTURA PARA QUE QUEPAN LOS CAMPOS)
//---------------------------------------------------------------------------
void __fastcall TAlumnos::ClickMostrarMas(TObject *Sender)
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
	fila->Height = 320; // <--- ALTURA SUFICIENTE PARA LOS 4 CAMPOS
	btn->Caption = "^";
	if(detalles) detalles->Visible = true;
}
	else
	{
		if(detalles) detalles->Visible = false;
		fila->Height = 40;
		btn->Caption = "v";
	}

	// Gestionar visibilidad de botones Editar/Guardar
	for(int i=0; i<header->ControlCount; i++) {
		int t = header->Controls[i]->Tag;
		if(t == 10 || t == 20) {
			header->Controls[i]->Visible = abrir;
		}
	}
}
//---------------------------------------------------------------------------
// 3. LOGICA BOTON EDITAR (SIN CAMBIOS, SOLO PARA REFERENCIA)
//---------------------------------------------------------------------------
void __fastcall TAlumnos::ClickEditar(TObject *Sender)
{
	TButton *btn = (TButton*)Sender;
	TPanel *header = (TPanel*)btn->Parent;
	TPanel *fila = (TPanel*)header->Parent;

	// 1. Mostrar botón Guardar (pero SIN ocultar el botón Editar)
	for(int i=0; i<header->ControlCount; i++) {
		if(header->Controls[i]->Tag == 20) header->Controls[i]->Visible = true;
	}

	// 2. Buscar el panel de detalles
	TPanel *pnlDetalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) pnlDetalles = (TPanel*)fila->Controls[i];
	}

	// 3. Convertir campos en TextFields editables
	if(pnlDetalles) {
		bool primerFoco = true;
		for(int i=0; i<pnlDetalles->ControlCount; i++) {
			TEdit *ed = dynamic_cast<TEdit*>(pnlDetalles->Controls[i]);

			// Solo modificamos si es un Edit VISIBLE
			if(ed && ed->Visible) {
				ed->ReadOnly = false;       // Permite escribir
				ed->BorderStyle = bsSingle; // Pone borde de caja
				ed->Color = clWindow;       // Pone fondo blanco

				// Poner el foco en el primer campo (Nombre)
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

	// 1. Buscar el panel de detalles donde están los TextField
	TPanel *pnlDetalles = NULL;
	for(int i=0; i<fila->ControlCount; i++) {
		if(fila->Controls[i]->Tag == 99) pnlDetalles = (TPanel*)fila->Controls[i];
	}

	if (pnlDetalles)
	{
		// Variables para guardar lo que leemos de las cajas de texto
		String id, nombre, apellido, dni, email, pass, fecha, rutina;

		// 2. Recorremos los controles para leer los datos según su TAG
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
					case 5: id = ed->Text; break;      // ID oculto
					case 6: pass = ed->Text; break;    // Pass oculta
					case 7: fecha = ed->Text; break;   // Fecha oculta
					case 8: rutina = ed->Text; break;  // Rutina oculta
				}
			}
		}

		// Seguridad: Si no hay ID, no podemos actualizar
		if (id.IsEmpty()) return;

		// 3. Armamos el paquete de datos para PHP (formato URL)
		// IMPORTANTE: Los nombres (nombre=, apellido=) deben coincidir con tu PHP $_POST
		String body = "id=" + id +
					  "&nombre=" + nombre +
					  "&apellido=" + apellido +
					  "&dni=" + dni +
					  "&email=" + email +
					  "&contrasena=" + pass +
					  "&fecha_vencimiento=" + fecha +
					  "&id_rutina=" + rutina;

		// 4. Enviamos los datos
		// Asegúrate de que el nombre del archivo PHP sea correcto
		String respuesta = HttpPost(L"/gimnasio_api/Admin/guardar_cambios_alumno.php", body);

		// 5. Mostrar resultado y refrescar la lista
		ShowMessage(respuesta);
		FormShow(this);
	}
}


void __fastcall TAlumnos::AgregarAlumnoClick(TObject *Sender)
{
	// 1. Crear la ventana
	TForm *ventana = new TForm(this);
	ventana->Caption = "Nuevo Alumno";
	ventana->Width = 350;
	ventana->Height = 450;
	ventana->Position = poMainFormCenter;
	ventana->BorderStyle = bsDialog;

	// -----------------------------------------------------------
	// AQUI ESTA EL ORDEN LOGICO (DE ARRIBA HACIA ABAJO)
	// Usamos 'Top' para decir exactamente a qué altura va cada uno
	// -----------------------------------------------------------

	// 1. NOMBRE (Arriba)
	TEdit *eNombre = new TEdit(ventana);
	eNombre->Parent = ventana;
	eNombre->Left = 40;        // Margen izquierdo
	eNombre->Top = 30;         // Altura (Arriba)
	eNombre->Width = 250;      // Ancho
	eNombre->TextHint = "Nombre";

	// 2. APELLIDO (Más abajo)
	TEdit *eApellido = new TEdit(ventana);
	eApellido->Parent = ventana;
	eApellido->Left = 40;
	eApellido->Top = 80;       // 50 pixeles más abajo
	eApellido->Width = 250;
	eApellido->TextHint = "Apellido";

	// 3. DNI
	TEdit *eDNI = new TEdit(ventana);
	eDNI->Parent = ventana;
	eDNI->Left = 40;
	eDNI->Top = 130;
	eDNI->Width = 250;
	eDNI->TextHint = "DNI";

	// 4. EMAIL
	TEdit *eEmail = new TEdit(ventana);
	eEmail->Parent = ventana;
	eEmail->Left = 40;
	eEmail->Top = 180;
	eEmail->Width = 250;
	eEmail->TextHint = "Email";

	// 5. CONTRASEÑA
	TEdit *ePass = new TEdit(ventana);
	ePass->Parent = ventana;
	ePass->Left = 40;
	ePass->Top = 230;
	ePass->Width = 250;
	ePass->TextHint = "Contraseña";

	// 6. BOTON GUARDAR
	TButton *btnGuardar = new TButton(ventana);
	btnGuardar->Parent = ventana;
	btnGuardar->Left = 40;
	btnGuardar->Top = 300;     // Abajo del todo
	btnGuardar->Width = 250;
	btnGuardar->Height = 45;
	btnGuardar->Caption = "GUARDAR ALUMNO";
	btnGuardar->ModalResult = mrOk;


if (ventana->ShowModal() == mrOk)
	{
		// Validamos datos obligatorios
		if(eNombre->Text.IsEmpty() || eApellido->Text.IsEmpty() || eDNI->Text.IsEmpty()) {
			ShowMessage("Faltan datos obligatorios (Nombre, Apellido, DNI).");
		}
		else
		{
			// Preparamos los datos
			String body = "nombre=" + eNombre->Text +
						  "&apellido=" + eApellido->Text +
						  "&dni=" + eDNI->Text +
						  "&email=" + eEmail->Text +
						  "&contrasena=" + ePass->Text;

			// *** CAMBIO CLAVE: Usamos el archivo correcto "crear_alumno.php" ***
			String resp = HttpPost(L"/gimnasio_api/Admin/crear_alumno.php", body);

		if (resp.Pos("Duplicate") > 0 && resp.Pos("dni") > 0)
			{
				ShowMessage("Error: Pusiste el mismo DNI que otro cliente.");
			}
			// 2. Si salió todo bien ("success")
			else if (resp.Pos("success") > 0)
			{
				ShowMessage("Alumno creado correctamente.");
				FormShow(this); // Recargar lista solo si se creó bien
			}
			// 3. Cualquier otro error raro
			else
			{
				ShowMessage("Hubo un error: " + resp);
			}
		}
	}
	delete ventana;
}
