object LoginForm: TLoginForm
  Left = 0
  Top = 0
  Caption = 'LoginForm'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object Login: TLabel
    Left = 288
    Top = 24
    Width = 30
    Height = 15
    Caption = 'Login'
  end
  object usuario: TEdit
    Left = 256
    Top = 96
    Width = 121
    Height = 23
    TabOrder = 0
    Text = 'usuario'
  end
  object Contraseña: TEdit
    Left = 256
    Top = 168
    Width = 121
    Height = 23
    TabOrder = 1
    Text = 'Contrase'#241'a'
  end
  object Iniciar: TButton
    Left = 280
    Top = 240
    Width = 75
    Height = 25
    Caption = 'Iniciar'
    TabOrder = 2
  end
end
