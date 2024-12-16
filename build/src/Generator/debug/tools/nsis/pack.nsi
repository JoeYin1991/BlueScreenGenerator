SilentInstall silent
RequestExecutionLevel user ;no elevation needed for this test
ShowInstDetails hide

OutFile {{outputPath}}
Icon "..\bluescreen\icon.ico"

Section

SetOutPath $TEMP\bluescreen_temp

File /r "..\bluescreen\*"

ExecWait "$TEMP\bluescreen_temp\bluescreen.exe"

SetOutPath $TEMP

RMDir /r $TEMP\bluescreen_temp

SectionEnd