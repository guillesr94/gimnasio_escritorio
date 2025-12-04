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
  object email: TEdit
    Left = 256
    Top = 96
    Width = 121
    Height = 23
    TabOrder = 0
    Text = 'email'
  end
  object password: TEdit
    Left = 256
    Top = 168
    Width = 121
    Height = 23
    TabOrder = 1
    Text = 'password'
  end
  object Iniciar: TButton
    Left = 280
    Top = 240
    Width = 75
    Height = 25
    Caption = 'Iniciar'
    TabOrder = 2
    OnClick = IniciarClick
  end
end
