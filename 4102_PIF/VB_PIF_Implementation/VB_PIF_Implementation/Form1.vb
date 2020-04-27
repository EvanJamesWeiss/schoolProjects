Imports System
Imports System.IO

Public Class frmMain

    Private strFileName As String = "No File Selected"

    Private Sub btnChoose_Click(sender As Object, e As EventArgs) Handles btnChoose.Click
        Dim fd As OpenFileDialog = New OpenFileDialog()
        strFileName = "No File Selected"

        fd.Title = "Open a Text File"
        fd.InitialDirectory = "C:/"
        fd.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*"
        fd.FilterIndex = 2
        fd.RestoreDirectory = True

        If fd.ShowDialog() = DialogResult.OK Then
            strFileName = fd.FileName
            btnEvaluate.Enabled = True
        Else
            btnEvaluate.Enabled = False
        End If

        lblFileName.Text = strFileName
    End Sub

    Private Sub btnEvaluate_Click(sender As Object, e As EventArgs) Handles btnEvaluate.Click
        ' This is where the PIF happens
        txtOutput.Clear()
        Dim fileText() As String
        Dim words As Dictionary(Of String, Tuple(Of String, Integer)) = New Dictionary(Of String, Tuple(Of String, Integer))

        If File.Exists(strFileName) Then
            fileText = File.ReadAllLines(strFileName)
        Else
            MessageBox.Show("No File Found")
            Return
        End If

        Dim i As Integer = 1

        For Each line In fileText

            line = LCase(line.Replace(".", "").Replace("?", "").Replace("!", "").Replace(",", "").Replace("(", "").Replace(")", ""))

            For Each word In line.Split(" ")

                word = word.Trim(" ")

                If words.ContainsKey(word) Then
                    Dim val As Tuple(Of String, Integer) = words.Item(word)
                    Dim appearedIn As String = val.Item1 & "-" & i.ToString()
                    Dim count As Integer = val.Item2 + 1
                    words(word) = New Tuple(Of String, Integer)(appearedIn, count)
                Else
                    words.Add(word, New Tuple(Of String, Integer)(i.ToString(), 1))
                End If

            Next

            i += 1

        Next

        Dim msg As String = ""

        For Each pair In words

            msg &= pair.Key
            msg &= " in lines: "
            msg &= pair.Value.Item1
            msg &= " wc="
            msg &= pair.Value.Item2.ToString()
            msg &= vbCrLf

        Next

        txtOutput.Text = msg

    End Sub
End Class
