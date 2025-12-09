//---------------------------------------------------------------------------

#ifndef ProfesoresH
#define ProfesoresH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TProfesores : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TButton *BtnVentanaProfesores;
	TButton *BtnVentanaCLientes;
	TButton *BtnVentanaVentas;
	TButton *BtnVentanaProductos;
	TScrollBox *ScrollBoxProfesores;
	TButton *AgregarProfesor;
	void __fastcall BtnVentanaProfesoresClick(TObject *Sender);
	void __fastcall BtnVentanaCLientesClick(TObject *Sender);
	void __fastcall BtnVentanaVentasClick(TObject *Sender);
	void __fastcall BtnVentanaProductosClick(TObject *Sender);

	void __fastcall FormShow(TObject *Sender);

private:


	void __fastcall ClickMostrarMas(TObject *Sender);
	void __fastcall ClickEditar(TObject *Sender);
	void __fastcall ClickGuardar(TObject *Sender);
	void __fastcall ClickEliminar(TObject *Sender);
public:		// User declarations
	__fastcall TProfesores(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TProfesores *Profesores;
//---------------------------------------------------------------------------
#endif
