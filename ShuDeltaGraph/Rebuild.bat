rem cd
rem dir
SubWCRev.exe E:\SONATA-PIP\개발소스\ShuDeltaGraph\ShuDeltaGraph revision.cpp.template revision.cpp



echo POSTBUILDSTEP for $(ProjectName)
 
rem xcopy "$(TargetPath)" "$(SolutionDir)$(OutDir)" /i /d /y
if errorlevel 1 goto BuildEventFailed
xcopy "$(TargetDir)$(TargetName).pdb" "$(SolutionDir)$(OutDir)" /i /d /y
if errorlevel 1 goto BuildEventFailed
 
echo GAC of: "$(SolutionDir)$(OutDir)$(TargetFileName)"
gacutil.exe /nologo /i "$(SolutionDir)$(OutDir)$(TargetFileName)" /f
if errorlevel 1 goto BuildEventFailed
 
REM Exit properly because the build will not fail 
REM unless the final step exits with an error code
goto BuildEventOK
:BuildEventFailed
echo POSTBUILDSTEP for $(ProjectName) FAILED
exit 1
:BuildEventOK
echo POSTBUILDSTEP for $(ProjectName) COMPLETED OK
