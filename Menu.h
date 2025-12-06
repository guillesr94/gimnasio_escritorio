//---------------------------------------------------------------------------

#ifndef MenuH
#define MenuH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TMenuForm : public TForm
{
__published:	// IDE-managed Components
	TButton *BtnVentanaProfesores;
	TButton *BtnVentanaCLientes;
	TButton *BtnVentanaVentas;
	TButton *BtnVentanaProductos;
	TLabel *Label1;
	void __fastcall BtnVentanaProfesoresClick(TObject *Sender);
	void __fastcall BtnVentanaCLientesClick(TObject *Sender);
	void __fastcall BtnVentanaVentasClick(TObject *Sender);
	void __fastcall BtnVentanaProductosClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMenuForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMenuForm *MenuForm;
//---------------------------------------------------------------------------
#endif
