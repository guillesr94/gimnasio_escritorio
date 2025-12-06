//---------------------------------------------------------------------------

#ifndef VentasH
#define VentasH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TVentas : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
private:	// User declarations
public:		// User declarations
	__fastcall TVentas(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TVentas *Ventas;
//---------------------------------------------------------------------------
#endif
