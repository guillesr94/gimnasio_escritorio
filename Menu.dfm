object MenuForm: TMenuForm
  Left = 0
  Top = 0
  Caption = 'Form6'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object Label1: TLabel
    Left = 136
    Top = 168
    Width = 193
    Height = 153
    Caption = 'Bienvenido'
  end
  object BtnVentanaProfesores: TButton
    Left = 32
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Profesores'
    TabOrder = 0
    OnClick = BtnVentanaProfesoresClick
  end
  object BtnVentanaCLientes: TButton
    Left = 168
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Clientes'
    TabOrder = 1
    OnClick = BtnVentanaCLientesClick
  end
  object BtnVentanaVentas: TButton
    Left = 304
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Ventas'
    TabOrder = 2
    OnClick = BtnVentanaVentasClick
  end
  object BtnVentanaProductos: TButton
    Left = 440
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Productos'
    TabOrder = 3
    OnClick = BtnVentanaProductosClick
  end
end
