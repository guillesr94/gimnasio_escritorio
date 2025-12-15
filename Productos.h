//---------------------------------------------------------------------------

#ifndef ProductosH
#define ProductosH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------

class TProductos : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TButton *BtnVentanaProfesores;
	TButton *BtnVentanaCLientes;
	TButton *BtnVentanaVentas;
	TButton *BtnVentanaProductos;
	TStringGrid *GridInventario;
	TButton *actualizarDatosProductos;
	TButton *agregarProducto;
	TButton *eliminarProducto;
	void __fastcall BtnVentanaProfesoresClick(TObject *Sender);
	void __fastcall BtnVentanaCLientesClick(TObject *Sender);
	void __fastcall BtnVentanaVentasClick(TObject *Sender);
	void __fastcall BtnVentanaProductosClick(TObject *Sender);
	void __fastcall actualizarDatosProductosClick(TObject *Sender);
	void __fastcall agregarProductoClick(TObject *Sender);
	void __fastcall eliminarProductoClick(TObject *Sender);
private:
    void CargarInventario();
public:		// User declarations
	__fastcall TProductos(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TProductos *Productos;
//---------------------------------------------------------------------------
#endif
