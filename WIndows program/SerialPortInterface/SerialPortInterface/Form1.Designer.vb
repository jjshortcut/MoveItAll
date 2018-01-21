<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmMain
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.cmbPort = New System.Windows.Forms.ComboBox()
        Me.cmbBaud = New System.Windows.Forms.ComboBox()
        Me.btnConnect = New System.Windows.Forms.Button()
        Me.btnDisconnect = New System.Windows.Forms.Button()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.btnSend = New System.Windows.Forms.Button()
        Me.txtTransmit = New System.Windows.Forms.TextBox()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.rtbReceived = New System.Windows.Forms.RichTextBox()
        Me.SerialPort1 = New System.IO.Ports.SerialPort(Me.components)
        Me.up_button = New System.Windows.Forms.Button()
        Me.down_button = New System.Windows.Forms.Button()
        Me.home_button = New System.Windows.Forms.Button()
        Me.move_button = New System.Windows.Forms.Button()
        Me.moveText = New System.Windows.Forms.TextBox()
        Me.save_home_button = New System.Windows.Forms.Button()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.GroupBox1.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(29, 15)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(53, 13)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Com Port:"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(29, 44)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(61, 13)
        Me.Label2.TabIndex = 1
        Me.Label2.Text = "Baud Rate:"
        '
        'cmbPort
        '
        Me.cmbPort.FormattingEnabled = True
        Me.cmbPort.Location = New System.Drawing.Point(114, 12)
        Me.cmbPort.Name = "cmbPort"
        Me.cmbPort.Size = New System.Drawing.Size(151, 21)
        Me.cmbPort.TabIndex = 2
        '
        'cmbBaud
        '
        Me.cmbBaud.FormattingEnabled = True
        Me.cmbBaud.Location = New System.Drawing.Point(114, 41)
        Me.cmbBaud.Name = "cmbBaud"
        Me.cmbBaud.Size = New System.Drawing.Size(151, 21)
        Me.cmbBaud.TabIndex = 3
        '
        'btnConnect
        '
        Me.btnConnect.Location = New System.Drawing.Point(271, 12)
        Me.btnConnect.Name = "btnConnect"
        Me.btnConnect.Size = New System.Drawing.Size(85, 23)
        Me.btnConnect.TabIndex = 4
        Me.btnConnect.Text = "Connect"
        Me.btnConnect.UseVisualStyleBackColor = True
        '
        'btnDisconnect
        '
        Me.btnDisconnect.Location = New System.Drawing.Point(271, 41)
        Me.btnDisconnect.Name = "btnDisconnect"
        Me.btnDisconnect.Size = New System.Drawing.Size(85, 23)
        Me.btnDisconnect.TabIndex = 5
        Me.btnDisconnect.Text = "Disconnect"
        Me.btnDisconnect.UseVisualStyleBackColor = True
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.btnSend)
        Me.GroupBox1.Controls.Add(Me.txtTransmit)
        Me.GroupBox1.Location = New System.Drawing.Point(19, 185)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(359, 44)
        Me.GroupBox1.TabIndex = 6
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Transmit Data"
        '
        'btnSend
        '
        Me.btnSend.Location = New System.Drawing.Point(252, 15)
        Me.btnSend.Name = "btnSend"
        Me.btnSend.Size = New System.Drawing.Size(85, 23)
        Me.btnSend.TabIndex = 1
        Me.btnSend.Text = "Send"
        Me.btnSend.UseVisualStyleBackColor = True
        '
        'txtTransmit
        '
        Me.txtTransmit.Location = New System.Drawing.Point(19, 18)
        Me.txtTransmit.Name = "txtTransmit"
        Me.txtTransmit.Size = New System.Drawing.Size(206, 20)
        Me.txtTransmit.TabIndex = 0
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.rtbReceived)
        Me.GroupBox2.Location = New System.Drawing.Point(19, 235)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(359, 204)
        Me.GroupBox2.TabIndex = 7
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Received Data"
        '
        'rtbReceived
        '
        Me.rtbReceived.Location = New System.Drawing.Point(19, 19)
        Me.rtbReceived.Name = "rtbReceived"
        Me.rtbReceived.Size = New System.Drawing.Size(320, 164)
        Me.rtbReceived.TabIndex = 0
        Me.rtbReceived.Text = ""
        '
        'SerialPort1
        '
        '
        'up_button
        '
        Me.up_button.Location = New System.Drawing.Point(271, 116)
        Me.up_button.Name = "up_button"
        Me.up_button.Size = New System.Drawing.Size(87, 23)
        Me.up_button.TabIndex = 2
        Me.up_button.Text = "UP/LEFT"
        Me.up_button.UseVisualStyleBackColor = True
        '
        'down_button
        '
        Me.down_button.Location = New System.Drawing.Point(271, 146)
        Me.down_button.Name = "down_button"
        Me.down_button.Size = New System.Drawing.Size(87, 23)
        Me.down_button.TabIndex = 8
        Me.down_button.Text = "DOWN/RIGHT"
        Me.down_button.UseVisualStyleBackColor = True
        '
        'home_button
        '
        Me.home_button.Location = New System.Drawing.Point(32, 116)
        Me.home_button.Name = "home_button"
        Me.home_button.Size = New System.Drawing.Size(76, 23)
        Me.home_button.TabIndex = 2
        Me.home_button.Text = "Home"
        Me.home_button.UseVisualStyleBackColor = True
        '
        'move_button
        '
        Me.move_button.Location = New System.Drawing.Point(32, 145)
        Me.move_button.Name = "move_button"
        Me.move_button.Size = New System.Drawing.Size(76, 23)
        Me.move_button.TabIndex = 9
        Me.move_button.Text = "Go to pos:"
        Me.move_button.UseVisualStyleBackColor = True
        '
        'moveText
        '
        Me.moveText.Location = New System.Drawing.Point(114, 147)
        Me.moveText.Name = "moveText"
        Me.moveText.Size = New System.Drawing.Size(151, 20)
        Me.moveText.TabIndex = 2
        '
        'save_home_button
        '
        Me.save_home_button.Location = New System.Drawing.Point(114, 116)
        Me.save_home_button.Name = "save_home_button"
        Me.save_home_button.Size = New System.Drawing.Size(151, 23)
        Me.save_home_button.TabIndex = 10
        Me.save_home_button.Text = "Save as max (EEPROM)"
        Me.save_home_button.UseVisualStyleBackColor = True
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(29, 100)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(40, 13)
        Me.Label3.TabIndex = 11
        Me.Label3.Text = "Control"
        '
        'frmMain
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(396, 451)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.save_home_button)
        Me.Controls.Add(Me.moveText)
        Me.Controls.Add(Me.move_button)
        Me.Controls.Add(Me.home_button)
        Me.Controls.Add(Me.down_button)
        Me.Controls.Add(Me.up_button)
        Me.Controls.Add(Me.GroupBox2)
        Me.Controls.Add(Me.GroupBox1)
        Me.Controls.Add(Me.btnDisconnect)
        Me.Controls.Add(Me.btnConnect)
        Me.Controls.Add(Me.cmbBaud)
        Me.Controls.Add(Me.cmbPort)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.MaximizeBox = False
        Me.Name = "frmMain"
        Me.Text = "Curtain and blinds controller V1"
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.GroupBox2.ResumeLayout(False)
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents cmbPort As System.Windows.Forms.ComboBox
    Friend WithEvents cmbBaud As System.Windows.Forms.ComboBox
    Friend WithEvents btnConnect As System.Windows.Forms.Button
    Friend WithEvents btnDisconnect As System.Windows.Forms.Button
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents GroupBox2 As System.Windows.Forms.GroupBox
    Friend WithEvents btnSend As System.Windows.Forms.Button
    Friend WithEvents txtTransmit As System.Windows.Forms.TextBox
    Friend WithEvents rtbReceived As System.Windows.Forms.RichTextBox
    Friend WithEvents SerialPort1 As System.IO.Ports.SerialPort
    Friend WithEvents up_button As System.Windows.Forms.Button
    Friend WithEvents down_button As System.Windows.Forms.Button
    Friend WithEvents home_button As System.Windows.Forms.Button
    Friend WithEvents move_button As System.Windows.Forms.Button
    Friend WithEvents moveText As System.Windows.Forms.TextBox
    Friend WithEvents save_home_button As System.Windows.Forms.Button
    Friend WithEvents Label3 As System.Windows.Forms.Label

End Class
