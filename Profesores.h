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
private:	// User declarations
public:		// User declarations
	__fastcall TProfesores(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TProfesores *Profesores;
//---------------------------------------------------------------------------
#endif
