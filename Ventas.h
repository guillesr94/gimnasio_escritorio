//---------------------------------------------------------------------------

#ifndef VentasH
#define VentasH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TVentas : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TButton *BtnVentanaProfesores;
	TButton *BtnVentanaCLientes;
	TButton *BtnVentanaVentas;
	TButton *BtnVentanaProductos;
	TStringGrid *GridProductos;
	TLabel *Total;
	void __fastcall BtnVentanaProfesoresClick(TObject *Sender);
	void __fastcall BtnVentanaCLientesClick(TObject *Sender);
	void __fastcall BtnVentanaVentasClick(TObject *Sender);
	void __fastcall BtnVentanaProductosClick(TObject *Sender);
private:

    void CargarDatosEnLaGrilla();
public:		// User declarations
	__fastcall TVentas(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TVentas *Ventas;
//---------------------------------------------------------------------------
#endif
