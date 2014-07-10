object Form1: TForm1
  Left = 192
  Top = 124
  Width = 907
  Height = 213
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 24
    Top = 16
    Width = 89
    Height = 49
    Caption = 'Dat 1'
    TabOrder = 0
    object Edit1: TEdit
      Left = 8
      Top = 16
      Width = 73
      Height = 21
      TabOrder = 0
      Text = '0'
    end
  end
  object GroupBox2: TGroupBox
    Left = 120
    Top = 16
    Width = 89
    Height = 49
    Caption = 'Dat 2'
    TabOrder = 1
    object Edit2: TEdit
      Left = 8
      Top = 16
      Width = 73
      Height = 21
      TabOrder = 0
      Text = '1000'
    end
  end
  object GroupBox3: TGroupBox
    Left = 216
    Top = 16
    Width = 89
    Height = 49
    Caption = 'Dat 3'
    TabOrder = 2
    object Edit3: TEdit
      Left = 8
      Top = 16
      Width = 73
      Height = 21
      TabOrder = 0
      Text = '2000'
    end
  end
  object GroupBox4: TGroupBox
    Left = 312
    Top = 16
    Width = 89
    Height = 49
    Caption = 'Dat 4'
    TabOrder = 3
    object Edit4: TEdit
      Left = 8
      Top = 16
      Width = 73
      Height = 21
      TabOrder = 0
      Text = '3000'
    end
  end
  object GroupBox5: TGroupBox
    Left = 408
    Top = 16
    Width = 89
    Height = 49
    Caption = 'Dat 5'
    TabOrder = 4
    object Edit5: TEdit
      Left = 8
      Top = 16
      Width = 73
      Height = 21
      TabOrder = 0
      Text = '4000'
    end
  end
  object GroupBox6: TGroupBox
    Left = 504
    Top = 16
    Width = 89
    Height = 49
    Caption = 'Dat 6'
    TabOrder = 5
    object Edit6: TEdit
      Left = 8
      Top = 16
      Width = 73
      Height = 21
      TabOrder = 0
      Text = '5000'
    end
  end
  object GroupBox7: TGroupBox
    Left = 600
    Top = 16
    Width = 89
    Height = 49
    Caption = 'Dat 7'
    TabOrder = 6
    object Edit7: TEdit
      Left = 8
      Top = 16
      Width = 73
      Height = 21
      TabOrder = 0
      Text = '30000'
    end
  end
  object GroupBox8: TGroupBox
    Left = 696
    Top = 16
    Width = 89
    Height = 49
    Caption = 'Dat 8'
    TabOrder = 7
    object Edit8: TEdit
      Left = 8
      Top = 16
      Width = 73
      Height = 21
      TabOrder = 0
      Text = '30000'
    end
  end
  object GroupBox9: TGroupBox
    Left = 24
    Top = 72
    Width = 185
    Height = 73
    Caption = 'Speed'
    TabOrder = 8
    object Edit9: TEdit
      Left = 16
      Top = 24
      Width = 121
      Height = 21
      TabOrder = 0
      Text = '1000'
    end
  end
  object GroupBox10: TGroupBox
    Left = 216
    Top = 72
    Width = 188
    Height = 73
    Caption = 'Len'
    TabOrder = 9
    OnMouseDown = GroupBox10MouseDown
    OnMouseMove = GroupBox10MouseMove
    OnMouseUp = GroupBox10MouseUp
    object Label1: TLabel
      Left = 2
      Top = 15
      Width = 184
      Height = 56
      Align = alClient
      Alignment = taCenter
      BiDiMode = bdLeftToRight
      Caption = '4000'
      Enabled = False
      ParentBiDiMode = False
    end
  end
  object Button1: TButton
    Left = 792
    Top = 40
    Width = 75
    Height = 25
    Caption = 'Send'
    TabOrder = 10
    OnClick = Button1Click
  end
  object Edit10: TEdit
    Left = 792
    Top = 16
    Width = 73
    Height = 21
    TabOrder = 11
    Text = 'COM2'
  end
end
