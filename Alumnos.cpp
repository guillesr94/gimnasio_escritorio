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

					// --- EXTRAER DATOS (Limpiamos el email porque viene con basura en tu JSON) ---
					String nombre = alumno->GetValue("nombre")->Value();
					String apellido = alumno->GetValue("apellido")->Value();
					String dni = alumno->GetValue("dni")->Value();

					// El Trim() elimina los espacios y saltos de linea extra del JSON
					String email = alumno->GetValue("email")->Value().Trim();
					// Truco extra: si el email tiene saltos de linea internos, cortamos antes
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

					// --- CABECERA (Nombre visible y botones) ---
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

					TButton *btnEditar = new TButton(header);
					btnEditar->Parent = header; btnEditar->Caption = "Editar"; btnEditar->Width = 60;
					btnEditar->Align = alRight; btnEditar->AlignWithMargins = true;
					btnEditar->Visible = false; btnEditar->Tag = 10;
					btnEditar->OnClick = ClickEditar;

					// Título Principal
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

					// FUNCION CREAR CAMPO (Label + Edit)
					auto CrearCampo = [&](String titulo, String valor) {
						// 1. EDIT (El valor)
						TEdit *e = new TEdit(pnlDetalles);
						e->Parent = pnlDetalles;
						e->Align = alTop;          // Al usar alTop, este se pega arriba del anterior
						e->Text = valor;
						e->ReadOnly = true;
						e->BorderStyle = bsNone;
						e->Color = clWhite;
						e->Margins->Bottom = 15;   // Espacio debajo
						e->AlignWithMargins = true;

						// 2. LABEL (El título)
						// Lo creamos DESPUES del edit, para que con alTop quede ARRIBA del edit
						TLabel *l = new TLabel(pnlDetalles);
						l->Parent = pnlDetalles;
						l->Align = alTop;
						l->Caption = titulo;
						l->Font->Size = 8;
						l->Font->Color = clGray;
						l->Margins->Bottom = 2;
						l->AlignWithMargins = true;
					};

					// ----------------------------------------------------------------
					// IMPORTANTE: ORDEN INVERSO DE CREACIÓN
					// Para que visualmente quede: Nombre -> Apellido -> DNI -> Email
					// Los creamos al revés porque alTop empuja los anteriores hacia abajo.
					// ----------------------------------------------------------------

					// 4. El de más abajo visualmente (EMAIL)
					CrearCampo("Email", email);

					// 3. (DNI)
					CrearCampo("DNI", dni);

					// 2. (APELLIDO)
					CrearCampo("Apellido", apellido);

					// 1. El de más arriba visualmente (NOMBRE) - Se crea EL ULTIMO
					CrearCampo("Nombre", nombre);
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

	btn->Visible = false;
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
			if(ed) {
				ed->ReadOnly = false;
				ed->BorderStyle = bsSingle;
				ed->Color = clWindow;

				// Poner foco en el primer Edit que encontremos (Nombre)
				if(primerFoco) {
					ed->SetFocus();
					primerFoco = false;
				}
			}
		}
	}
}






//---------------------------------------------------------------------------
// VENTANA AGREGAR
//---------------------------------------------------------------------------
void __fastcall TAlumnos::AgregarClick(TObject *Sender)
{
	TForm *v = new TForm(this);
	v->Caption = "Ingresa los datos";
	v->Width = 300; v->Height = 350;
	v->Position = poMainFormCenter; v->BorderStyle = bsDialog;

	TEdit *eN = new TEdit(v); eN->Parent=v; eN->Align=alTop; eN->TextHint="Nombre"; eN->Margins->Bottom=5; eN->AlignWithMargins=true;
	TEdit *eA = new TEdit(v); eA->Parent=v; eA->Align=alTop; eA->TextHint="Apellido"; eA->Margins->Bottom=5; eA->AlignWithMargins=true;
	TEdit *eD = new TEdit(v); eD->Parent=v; eD->Align=alTop; eD->TextHint="DNI"; eD->Margins->Bottom=5; eD->AlignWithMargins=true;
	TEdit *eM = new TEdit(v); eM->Parent=v; eM->Align=alTop; eM->TextHint="Email"; eM->Margins->Bottom=5; eM->AlignWithMargins=true;
	TEdit *eP = new TEdit(v); eP->Parent=v; eP->Align=alTop; eP->TextHint="Contraseña"; eP->Margins->Bottom=5; eP->AlignWithMargins=true;

	TButton *bG = new TButton(v); bG->Parent = v; bG->Caption = "Guardar"; bG->Align = alBottom; bG->ModalResult = mrOk;

	if (v->ShowModal() == mrOk)
	{
		String body = "nombre=" + eN->Text + "&apellido=" + eA->Text + "&dni=" + eD->Text + "&email=" + eM->Text + "&contrasena=" + eP->Text;
		String resp = HttpPost(L"/gimnasio_api/Admin/guardar_nuevo_alumno.php", body);
		ShowMessage(resp);
		FormShow(this);
	}
	delete v;
}
