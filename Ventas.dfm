object Ventas: TVentas
  Left = 0
  Top = 0
  Caption = 'Form5'
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
    Left = 120
    Top = 72
    Width = 34
    Height = 15
    Caption = 'Ventas'
  end
  object Total: TLabel
    Left = 56
    Top = 376
    Width = 65
    Height = 15
    Caption = 'Total'
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
  object GridProductos: TStringGrid
    Left = 8
    Top = 93
    Width = 545
    Height = 260
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goFixedRowDefAlign]
    TabOrder = 4
  end
end
