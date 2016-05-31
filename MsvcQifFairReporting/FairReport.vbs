
 
Dim fso,objFile
Dim strFile

Function GetFileName(mydir)
	Dim objDialog
	' Create a dialog object
	Set objDialog = CreateObject( "MSComDlg.CommonDialog" )
	objDialog.InitialDir = mydir
	objDialog.Filter = "All files|*.*"
	' Open the dialog and return the selected file name
	If objDialog.ShowOpen Then
 		 GetFileName = objDialog.FileName
	Else
		GetFileName = ""
	End If
End Function

set fso=CreateObject("Scripting.FileSystemObject")
Set wShell=CreateObject("WScript.Shell")

Set oExec=wShell.Exec("mshta.exe ""about:<input type=file id=FILE><script>FILE.click();new ActiveXObject('Scripting.FileSystemObject').GetStandardStream(1).WriteLine(FILE.value);close();resizeTo(0,0);</script>""")
strFile =oExec.StdOut.ReadAll 

If Right(strFile, 2) = vbCrLf Then
  strFile = Left(strFile, Len(strFile) - 2)
End If 

Dim oShell
Set oShell = WScript.CreateObject ("WScript.Shell")
oShell.run "MsvcQifFairReporting\Win32\Debug\MsvcQifFairReporting.exe -v -q " &  strFile



