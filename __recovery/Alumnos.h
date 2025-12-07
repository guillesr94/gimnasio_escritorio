//---------------------------------------------------------------------------

#ifndef AlumnosH
#define AlumnosH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TAlumnos : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TButton *BtnVentanaProfesores;
	TButton *BtnVentanaCLientes;
	TButton *BtnVentanaVentas;
	TButton *BtnVentanaProductos;
	TScrollBox *ScrollBoxAlumnos;
	void __fastcall BtnVentanaProfesoresClick(TObject *Sender);
	void __fastcall BtnVentanaCLientesClick(TObject *Sender);
	void __fastcall BtnVentanaVentasClick(TObject *Sender);
	void __fastcall BtnVentanaProductosClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TAlumnos(TComponent* Owner);
	void __fastcall ClickMostrarMas(TObject *Sender);

	void __fastcall AgregarBotones(TPanel *header, String id);
	void __fastcall ClickEditar(TObject *Sender);
	void __fastcall ClickEliminar(TObject *Sender);
    void __fastcall CrearCampo(TWinControl *parent, String titulo, String valor);
};
//---------------------------------------------------------------------------
extern PACKAGE TAlumnos *Alumnos;
//---------------------------------------------------------------------------
#endif
