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
	TButton *CalcularTotal;
	TLabel *Label2;
	void __fastcall BtnVentanaProfesoresClick(TObject *Sender);
	void __fastcall BtnVentanaCLientesClick(TObject *Sender);
	void __fastcall BtnVentanaVentasClick(TObject *Sender);
	void __fastcall BtnVentanaProductosClick(TObject *Sender);
	void __fastcall GridProductosSelectCell(TObject *Sender, System::LongInt ACol, System::LongInt ARow,
          bool &CanSelect);
	void __fastcall CalcularTotalClick(TObject *Sender);



private:

	void CargarDatosEnLaGrilla();
public:
	__fastcall TVentas(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TVentas *Ventas;
//---------------------------------------------------------------------------
#endif
