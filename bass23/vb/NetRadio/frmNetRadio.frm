VERSION 5.00
Begin VB.Form frmNetRadio 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS internet radio tuner"
   ClientHeight    =   4215
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4215
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4215
   ScaleWidth      =   4215
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame frameProxy 
      Caption         =   " Proxy server "
      Height          =   975
      Left            =   120
      TabIndex        =   18
      Top             =   3120
      Width           =   3975
      Begin VB.CheckBox chkDirectConnect 
         Caption         =   "Direct connection"
         Height          =   255
         Left            =   240
         TabIndex        =   20
         Top             =   600
         Width           =   1575
      End
      Begin VB.TextBox txtProxy 
         Height          =   285
         Left            =   120
         MaxLength       =   100
         TabIndex        =   19
         Top             =   240
         Width           =   3735
      End
      Begin VB.Label lblUserPass 
         AutoSize        =   -1  'True
         Caption         =   "[user:pass@]server:port"
         Height          =   195
         Left            =   2160
         TabIndex        =   21
         Top             =   600
         Width           =   1680
      End
   End
   Begin VB.Frame framePresents 
      Caption         =   " Presents "
      Height          =   1455
      Left            =   120
      TabIndex        =   11
      Top             =   0
      Width           =   3975
      Begin VB.CheckBox chkSave 
         Caption         =   "Save local copy"
         Height          =   255
         Left            =   120
         TabIndex        =   10
         Top             =   1080
         Width           =   3735
      End
      Begin VB.CommandButton btnPresents 
         Caption         =   "5"
         Height          =   350
         Index           =   9
         Left            =   3360
         TabIndex        =   9
         Top             =   630
         Width           =   450
      End
      Begin VB.CommandButton btnPresents 
         Caption         =   "4"
         Height          =   350
         Index           =   8
         Left            =   2790
         TabIndex        =   7
         Top             =   630
         Width           =   450
      End
      Begin VB.CommandButton btnPresents 
         Caption         =   "3"
         Height          =   350
         Index           =   7
         Left            =   2220
         TabIndex        =   5
         Top             =   630
         Width           =   450
      End
      Begin VB.CommandButton btnPresents 
         Caption         =   "2"
         Height          =   350
         Index           =   6
         Left            =   1650
         TabIndex        =   3
         Top             =   630
         Width           =   450
      End
      Begin VB.CommandButton btnPresents 
         Caption         =   "1"
         Height          =   350
         Index           =   5
         Left            =   1080
         TabIndex        =   1
         Top             =   630
         Width           =   450
      End
      Begin VB.CommandButton btnPresents 
         Caption         =   "5"
         Height          =   350
         Index           =   4
         Left            =   3360
         TabIndex        =   8
         Top             =   240
         Width           =   450
      End
      Begin VB.CommandButton btnPresents 
         Caption         =   "4"
         Height          =   350
         Index           =   3
         Left            =   2790
         TabIndex        =   6
         Top             =   240
         Width           =   450
      End
      Begin VB.CommandButton btnPresents 
         Caption         =   "3"
         Height          =   350
         Index           =   2
         Left            =   2220
         TabIndex        =   4
         Top             =   240
         Width           =   450
      End
      Begin VB.CommandButton btnPresents 
         Caption         =   "2"
         Height          =   350
         Index           =   1
         Left            =   1650
         TabIndex        =   2
         Top             =   240
         Width           =   450
      End
      Begin VB.CommandButton btnPresents 
         Caption         =   "1"
         Height          =   350
         Index           =   0
         Left            =   1080
         TabIndex        =   0
         Top             =   240
         Width           =   450
      End
      Begin VB.Label lblModem 
         AutoSize        =   -1  'True
         Caption         =   "Modem"
         Height          =   195
         Left            =   120
         TabIndex        =   14
         Top             =   720
         Width           =   525
      End
      Begin VB.Label lblBroadband 
         AutoSize        =   -1  'True
         Caption         =   "Broadband"
         Height          =   195
         Left            =   120
         TabIndex        =   13
         Top             =   240
         Width           =   780
      End
   End
   Begin VB.Frame framePlaying 
      Caption         =   " Currently playing "
      Height          =   1455
      Left            =   120
      TabIndex        =   12
      Top             =   1560
      Width           =   3975
      Begin VB.Label lblBPS 
         Alignment       =   2  'Center
         Height          =   195
         Left            =   90
         TabIndex        =   17
         Top             =   1200
         Width           =   3795
         WordWrap        =   -1  'True
      End
      Begin VB.Label lblName 
         Alignment       =   2  'Center
         Caption         =   "not playing"
         Height          =   375
         Left            =   105
         TabIndex        =   16
         Top             =   720
         Width           =   3765
         WordWrap        =   -1  'True
      End
      Begin VB.Label lblSong 
         Alignment       =   2  'Center
         Height          =   435
         Left            =   105
         TabIndex        =   15
         Top             =   240
         Width           =   3765
         WordWrap        =   -1  'True
      End
   End
End
Attribute VB_Name = "frmNetRadio"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'/////////////////////////////////////////////////////////////////////////////////
' frmNetRadio.frm - Copyright (c) 2002-2006 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                         [http://www.jobnik.org]
'                                                         [  jobnik@jobnik.org  ]
'
' * Save local copy is added by: Peter Hebels @ http://www.phsoft.nl
'                                             e-mail: info@phsoft.nl
'
' Other sources: modNetRadio.bas & clsFileIo.cls
'
' BASS Internet radio example
' Originally translated from - netradio.c - Example of Ian Luck
'/////////////////////////////////////////////////////////////////////////////////

Option Explicit

Private Declare Function GetModuleFileName Lib "kernel32" Alias "GetModuleFileNameA" (ByVal hModule As Long, ByVal lpFileName As String, ByVal nSize As Long) As Long
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal length As Long)

Private Sub chkDirectConnect_Click()
    If (chkDirectConnect.value) Then
        Call BASS_SetConfig(BASS_CONFIG_NET_PROXY, 0) ' disable proxy
    Else
        Call BASS_SetConfig(BASS_CONFIG_NET_PROXY, VarPtr(proxy(0))) ' enable proxy
    End If
End Sub

Private Sub Form_Load()
    ' change and set the current path, to prevent from VB not finding BASS.DLL
    ChDrive App.Path
    ChDir App.Path

    ' check the correct BASS was loaded
    If (HiWord(BASS_GetVersion) <> BASSVERSION) Then
        Call MsgBox("An incorrect version of BASS.DLL was loaded", vbCritical)
        End
    End If

    ' setup output device
    If (BASS_Init(-1, 44100, 0, Me.hwnd, 0) = 0) Then
        Call Error_("Can't initialize device")
        End
    End If

    Call BASS_SetConfig(BASS_CONFIG_NET_PREBUF, 0) ' minimize automatic pre-buffering, so we can do it (and display it) instead
    Call BASS_SetConfig(BASS_CONFIG_NET_PROXY, VarPtr(proxy(0)))  ' setup proxy server location

    ' stream URLs
    url = Array("http://64.236.34.196/stream/1048", "http://205.188.215.226:8000", _
                "http://207.200.96.228/stream/1030", "http://206.98.167.99:8406", _
                "http://64.236.34.97/stream/1040", "http://64.202.98.193:8000", _
                "http://205.188.215.226:8012", "http://205.188.215.226:8008", _
                "http://207.200.96.225:8010", "http://64.202.98.91:8082")

    Set WriteFile = New clsFileIo
    cthread = 0
End Sub

' this function will check if you're running in IDE or EXE modes
' VB will crash if you're closing the app while (cthread<>0) in IDE,
' but won't crash if in EXE mode
Public Function isIDEmode() As Boolean
    Dim sFileName As String, lCount As Long

    sFileName = String(255, 0)
    lCount = GetModuleFileName(App.hInstance, sFileName, 255)
    sFileName = UCase(GetFileName(Mid(sFileName, 1, lCount)))

    isIDEmode = (sFileName = "VB6.EXE")
End Function

Private Sub Form_Unload(Cancel As Integer)
    If (isIDEmode And cthread) Then
        ' IDE Version
        Cancel = True   ' disable closing app to avoid crash
    Else
        ' Compiled Version or (cthread = 0) close app is available
        Call BASS_Free
    End If
End Sub

Private Sub btnPresents_Click(index As Integer)
    If (cthread) Then   ' already connecting
        Call Beep
    Else
        Call CopyMemory(proxy(0), ByVal txtProxy.Text, Len(txtProxy.Text))   ' get proxy server

        ' open URL in a new thread (so that main thread is free)
        Dim threadid As Long
        cthread = CreateThread(ByVal 0&, 0, AddressOf OpenURL, index, 0, threadid)   ' threadid param required on win9x
    End If
End Sub

Private Sub chkSave_Click()
    If chkSave.value = vbChecked Then
        DoDownload = True
    Else
        DoDownload = False
    End If
End Sub

'--------------------
' useful function :)
'--------------------

' get file name from file path
Public Function GetFileName(ByVal fp As String) As String
    GetFileName = Mid(fp, InStrRev(fp, "\") + 1)
End Function
