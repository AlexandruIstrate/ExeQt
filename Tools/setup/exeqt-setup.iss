#define ApplicationName "ExeQt"
#define ApplicationVersion "1.2.2"

#define SupportURL "https://github.com/AlexandruIstrate/ExeQt"

#define SourceDirectory "..\..\ExeQt\bin"

[Setup]
AppName={#ApplicationName}
AppVersion={#ApplicationVersion}
DefaultDirName={commonpf}
AppId={{A0D8ADF9-F339-4EE1-AC80-77C6F18DFCB0}}
AppSupportURL={#SupportURL}

[Files]
Source: "{#SourceDirectory}\*"; DestDir: "{app}"; Flags: ignoreversion