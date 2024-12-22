SilentInstall silent
RequestExecutionLevel user ;no elevation needed for this test
ShowInstDetails hide

OutFile {{outputPath}}
Icon "{{appDir}}\tools\bluescreen\icon.ico"

Section

SetOutPath $TEMP\bluescreen_temp

File /r "{{appDir}}\tools\bluescreen\*"

ExecWait "$TEMP\bluescreen_temp\bluescreen.exe"

SetOutPath $TEMP

RMDir /r $TEMP\bluescreen_temp

SectionEnd