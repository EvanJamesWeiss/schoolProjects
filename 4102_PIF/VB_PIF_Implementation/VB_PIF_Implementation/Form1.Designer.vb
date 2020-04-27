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
        Me.btnChoose = New System.Windows.Forms.Button()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.lblFileName = New System.Windows.Forms.Label()
        Me.btnEvaluate = New System.Windows.Forms.Button()
        Me.txtOutput = New System.Windows.Forms.TextBox()
        Me.SuspendLayout()
        '
        'btnChoose
        '
        Me.btnChoose.Location = New System.Drawing.Point(28, 23)
        Me.btnChoose.Name = "btnChoose"
        Me.btnChoose.Size = New System.Drawing.Size(126, 50)
        Me.btnChoose.TabIndex = 0
        Me.btnChoose.Text = "Choose File"
        Me.btnChoose.UseVisualStyleBackColor = True
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(39, 85)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(101, 20)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Selected File"
        '
        'lblFileName
        '
        Me.lblFileName.AutoSize = True
        Me.lblFileName.Location = New System.Drawing.Point(71, 115)
        Me.lblFileName.MaximumSize = New System.Drawing.Size(400, 0)
        Me.lblFileName.Name = "lblFileName"
        Me.lblFileName.Size = New System.Drawing.Size(85, 13)
        Me.lblFileName.TabIndex = 2
        Me.lblFileName.Text = "No File Selected"
        '
        'btnEvaluate
        '
        Me.btnEvaluate.Enabled = False
        Me.btnEvaluate.Location = New System.Drawing.Point(28, 241)
        Me.btnEvaluate.Name = "btnEvaluate"
        Me.btnEvaluate.Size = New System.Drawing.Size(126, 50)
        Me.btnEvaluate.TabIndex = 3
        Me.btnEvaluate.Text = "Evaluate"
        Me.btnEvaluate.UseVisualStyleBackColor = True
        '
        'txtOutput
        '
        Me.txtOutput.Location = New System.Drawing.Point(44, 326)
        Me.txtOutput.Multiline = True
        Me.txtOutput.Name = "txtOutput"
        Me.txtOutput.ScrollBars = System.Windows.Forms.ScrollBars.Both
        Me.txtOutput.Size = New System.Drawing.Size(419, 344)
        Me.txtOutput.TabIndex = 4
        '
        'frmMain
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(501, 695)
        Me.Controls.Add(Me.txtOutput)
        Me.Controls.Add(Me.btnEvaluate)
        Me.Controls.Add(Me.lblFileName)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.btnChoose)
        Me.Name = "frmMain"
        Me.Text = "PIF Implementaion in Visual Basic"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents btnChoose As Button
    Friend WithEvents Label1 As Label
    Friend WithEvents lblFileName As Label
    Friend WithEvents btnEvaluate As Button
    Friend WithEvents txtOutput As TextBox
End Class
