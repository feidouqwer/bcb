object Form1: TForm1
  Left = 488
  Top = 282
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'DM36X'#25209#37327#20462#25913'MAC'#21644'HWID--'#20339#20449#25463
  ClientHeight = 340
  ClientWidth = 455
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object lblInfo: TLabel
    Left = 8
    Top = 272
    Width = 337
    Height = 25
    AutoSize = False
    Font.Charset = GB2312_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object lblClientNum: TLabel
    Left = 8
    Top = 304
    Width = 337
    Height = 25
    AutoSize = False
    Font.Charset = GB2312_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
  end
  object btnUpdate: TButton
    Left = 352
    Top = 272
    Width = 97
    Height = 57
    BiDiMode = bdLeftToRight
    Caption = #25209#37327#35774#32622
    Default = True
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = #23435#20307
    Font.Style = []
    ModalResult = 1
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 0
    OnClick = btnUpdateClick
  end
  object GroupBox1: TGroupBox
    Left = 296
    Top = 8
    Width = 153
    Height = 65
    Caption = #35774#32622#35774#22791#25968#37327
    TabOrder = 1
    object Label1: TLabel
      Left = 16
      Top = 24
      Width = 73
      Height = 17
      AutoSize = False
      Caption = #35774#22791#25968#37327
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object ComboBox1: TComboBox
      Left = 80
      Top = 16
      Width = 57
      Height = 28
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ItemHeight = 20
      ItemIndex = 3
      ParentFont = False
      TabOrder = 0
      Text = '4'
      Items.Strings = (
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        '8'
        '9'
        '10'
        '11'
        '12'
        '13'
        '14'
        '15')
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 129
    Height = 65
    Caption = #35774#32622
    TabOrder = 2
    object ckMAC: TCheckBox
      Left = 16
      Top = 16
      Width = 97
      Height = 17
      Caption = #35774#32622'MAC'
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object ckHWID: TCheckBox
      Left = 16
      Top = 40
      Width = 97
      Height = 17
      Caption = #35774#32622#30828#20214'ID'
      Enabled = False
      TabOrder = 1
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 80
    Width = 441
    Height = 177
    TabOrder = 3
    object ProgressBar1: TProgressBar
      Left = 8
      Top = 144
      Width = 425
      Height = 25
      Min = 0
      Max = 100
      Smooth = True
      TabOrder = 0
    end
    object RichEdit1: TRichEdit
      Left = 8
      Top = 16
      Width = 425
      Height = 121
      Font.Charset = GB2312_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      ScrollBars = ssBoth
      TabOrder = 1
      WordWrap = False
    end
  end
  object GroupBox4: TGroupBox
    Left = 144
    Top = 8
    Width = 145
    Height = 65
    Caption = #20018#21475#35774#32622
    TabOrder = 4
    object cbComm: TComboBox
      Left = 8
      Top = 24
      Width = 73
      Height = 28
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ItemHeight = 20
      ParentFont = False
      TabOrder = 0
    end
    object btnOpenComm: TButton
      Left = 88
      Top = 16
      Width = 49
      Height = 41
      Caption = #25171#24320
      TabOrder = 1
      OnClick = btnOpenCommClick
    end
  end
  object mscm1: TYbCommDevice
    PortNo = 1
    Baud = br115200
    ByteSize = 8
    InBufSize = 8192
    OutBufSize = 8192
    HwInSize = 1200
    HwOutSize = 1200
    QueueSize = 16
    PackageSize = 4096
    PackageInterval = 5000
    Top = 312
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 10000
    OnTimer = Timer1Timer
    Left = 32
    Top = 312
  end
end
