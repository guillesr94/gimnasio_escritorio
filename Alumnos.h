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
private:	// User declarations
public:		// User declarations
	__fastcall TAlumnos(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAlumnos *Alumnos;
//---------------------------------------------------------------------------
#endif
