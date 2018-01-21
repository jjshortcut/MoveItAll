﻿'Serial Port Interfacing with VB.net 2010 Express Edition
'Copyright (C) 2010  Richard Myrick T. Arellaga
'
'This program is free software: you can redistribute it and/or modify
'it under the terms of the GNU General Public License as published by
'the Free Software Foundation, either version 3 of the License, or
'(at your option) any later version.
'
'This program is distributed in the hope that it will be useful,
'but WITHOUT ANY WARRANTY; without even the implied warranty of
'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'GNU General Public License for more details.
'
' You should have received a copy of the GNU General Public License
' along with this program.  If not, see <http://www.gnu.org/licenses/>.


Imports System
Imports System.ComponentModel
Imports System.Threading
Imports System.IO.Ports
Public Class frmMain
    Dim myPort As Array  'COM Ports detected on the system will be stored here
    Delegate Sub SetTextCallback(ByVal [text] As String) 'Added to prevent threading errors during receiveing of data

    Private Sub frmMain_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        'When our form loads, auto detect all serial ports in the system and populate the cmbPort Combo box.
        myPort = IO.Ports.SerialPort.GetPortNames() 'Get all com ports available
        cmbBaud.Items.Add(9600)     'Populate the cmbBaud Combo box to common baud rates used   
        cmbBaud.Items.Add(19200)
        cmbBaud.Items.Add(38400)
        cmbBaud.Items.Add(57600)
        cmbBaud.Items.Add(115200)

        For i = 0 To UBound(myPort)
            cmbPort.Items.Add(myPort(i))
        Next
        Try
            cmbPort.Text = cmbPort.Items.Item(0)    'Set cmbPort text to the first COM port detected
        Catch ex As Exception
            MessageBox.Show("No COM ports found!", "Curtain controller",
            MessageBoxButtons.OKCancel, MessageBoxIcon.Asterisk)
        End Try

        cmbBaud.Text = cmbBaud.Items.Item(0)    'Set cmbBaud text to the first Baud rate on the list

        btnDisconnect.Enabled = False           'Initially Disconnect Button is Disabled

    End Sub

    Private Sub btnConnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnConnect.Click
        SerialPort1.PortName = cmbPort.Text         'Set SerialPort1 to the selected COM port at startup
        SerialPort1.BaudRate = cmbBaud.Text         'Set Baud rate to the selected value on 

        'Other Serial Port Property
        SerialPort1.Parity = IO.Ports.Parity.None
        SerialPort1.StopBits = IO.Ports.StopBits.One
        SerialPort1.DataBits = 8            'Open our serial port
        SerialPort1.Open()

        btnConnect.Enabled = False          'Disable Connect button
        btnDisconnect.Enabled = True        'and Enable Disconnect button

    End Sub

    Private Sub btnDisconnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnDisconnect.Click
        SerialPort1.Close()             'Close our Serial Port

        btnConnect.Enabled = True
        btnDisconnect.Enabled = False
    End Sub

    Private Sub btnSend_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnSend.Click
        SerialPort1.Write(txtTransmit.Text & vbCr) 'The text contained in the txtText will be sent to the serial port as ascii
        'plus the carriage return (Enter Key) the carriage return can be ommitted if the other end does not need it
    End Sub

    Private Sub SerialPort1_DataReceived(ByVal sender As Object, ByVal e As System.IO.Ports.SerialDataReceivedEventArgs) Handles SerialPort1.DataReceived
        ReceivedText(SerialPort1.ReadExisting())    'Automatically called every time a data is received at the serialPort
    End Sub
    Private Sub ReceivedText(ByVal [text] As String)
        'compares the ID of the creating Thread to the ID of the calling Thread
        If Me.rtbReceived.InvokeRequired Then
            Dim x As New SetTextCallback(AddressOf ReceivedText)
            Me.Invoke(x, New Object() {(text)})
        Else
            Me.rtbReceived.Text &= [text]
        End If

        'rtbReceived.SelectionStart = rtbReceived.Text.Length()
        'rtbReceived.ScrollToCaret()
    End Sub

    Private Sub cmbPort_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmbPort.SelectedIndexChanged
        If SerialPort1.IsOpen = False Then
            SerialPort1.PortName = cmbPort.Text         'pop a message box to user if he is changing ports
        Else                                            'without disconnecting first.
            MsgBox("Valid only if port is Closed", vbCritical)
        End If
    End Sub

    Private Sub cmbBaud_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmbBaud.SelectedIndexChanged
        If SerialPort1.IsOpen = False Then
            SerialPort1.BaudRate = cmbBaud.Text         'pop a message box to user if he is changing baud rate
        Else                                            'without disconnecting first.
            MsgBox("Valid only if port is Closed", vbCritical)
        End If
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles up_button.Click
        SerialPort1.Write(",") 'The text contained in the txtText will be sent to the serial port as ascii
    End Sub

    Private Sub down_button_Click(sender As Object, e As EventArgs) Handles down_button.Click
        SerialPort1.Write(".") 'The text contained in the txtText will be sent to the serial port as ascii
    End Sub

    Private Sub home_button_Click(sender As Object, e As EventArgs) Handles home_button.Click
        SerialPort1.Write("h") 'The text contained in the txtText will be sent to the serial port as ascii
    End Sub

    Private Sub move_button_Click(sender As Object, e As EventArgs) Handles move_button.Click
        SerialPort1.Write("m" + moveText.Text) 'The text contained in the txtText will be sent to the serial port as ascii
    End Sub

    Private Sub rtbReceived_TextChanged(sender As Object, e As EventArgs) Handles rtbReceived.TextChanged

    End Sub

    Private Sub save_home_button_Click(sender As Object, e As EventArgs) Handles save_home_button.Click
        SerialPort1.Write("s") 'The text contained in the txtText will be sent to the serial port as ascii
    End Sub

    Private Sub txtTransmit_TextChanged(sender As Object, e As EventArgs) Handles txtTransmit.TextChanged

    End Sub

    Private Sub GroupBox1_Enter(sender As Object, e As EventArgs) Handles GroupBox1.Enter

    End Sub
End Class
