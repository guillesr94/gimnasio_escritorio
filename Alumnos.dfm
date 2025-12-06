object Alumnos: TAlumnos
  Left = 0
  Top = 0
  Caption = 'Form3'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  OnShow = FormShow
  TextHeight = 15
  object Label1: TLabel
    Left = 32
    Top = 55
    Width = 48
    Height = 15
    Caption = 'Alumnos'
  end
  object BtnVentanaProfesores: TButton
    Left = 32
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Profesores'
    TabOrder = 0
    OnClick = BtnVentanaProfesoresClick
  end
  object BtnVentanaCLientes: TButton
    Left = 168
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Clientes'
    TabOrder = 1
    OnClick = BtnVentanaCLientesClick
  end
  object BtnVentanaVentas: TButton
    Left = 304
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Ventas'
    TabOrder = 2
    OnClick = BtnVentanaVentasClick
  end
  object BtnVentanaProductos: TButton
    Left = 440
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Productos'
    TabOrder = 3
    OnClick = BtnVentanaProductosClick
  end
  object Memo1: TMemo
    Left = 32
    Top = 112
    Width = 483
    Height = 305
    Lines.Strings = (
      'Memo1')
    TabOrder = 4
  end
end
