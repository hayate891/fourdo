#include "FourDOApp.h"
#include "wx/log.h"

IMPLEMENT_APP(FourDOApp)

/////////////////////////////////////////////////////////////////////////
// Application startup
/////////////////////////////////////////////////////////////////////////

#ifndef GENRE_UNIX
int main()
{
	return WinMain(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), SW_SHOWNORMAL);
}
#endif

bool FourDOApp::OnInit()
{
   //
   // Even though we're a windowed app, I'm still redirecting output to the command line
   // (If I don't do this, command line usage summary shows up in a message box... yuck)
   //
   wxMessageOutput::Set( new wxMessageOutputStderr() );

   // 
   // Command line parser.
   // 
   wxString  logo;
   wxCmdLineParser parser( this->argc, this->argv );
   
   parser.SetSwitchChars( _T("-/") );

   logo.Append( _T("============================================================\r\n") );
   logo.Append( _T("FourDO - An open-source HLE 3DO Emulator\r\n") );
   logo.Append( _T("============================================================\r\n") );
   parser.AddSwitch( _T("?"),  _T(""),          _T("Displays usage information"),		wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_OPTION_HELP );
   parser.AddSwitch( _T("h"),  _T("help"),      _T("Displays usage information"),		wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_OPTION_HELP );
   parser.AddSwitch( _T("d"),  _T("debug"),     _T("Enables debug mode"),				wxCMD_LINE_PARAM_OPTIONAL );
   parser.AddOption( _T("li"), _T("loadimage"), _T("Loads a CD image file"),			wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL );
   parser.AddOption( _T("lc"), _T("loadcode"),  _T("Loads a file as ARM60 code"),		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL );
   parser.SetLogo( logo );
   parser.EnableLongOptions();

   // 
   // logger
   // 
   wxLog::SetActiveTarget( new wxLogStderr( fopen( "fourdo.log", "a" ) ) );
   wxLogMessage( _T("=====================================================") );

   if( parser.Parse() == 0 )
   {
      wxFrame* main = new MainFrame( &parser );
      main->Show();
      SetTopWindow( main );
      return true;
   }
   else
   {
      // Cancel out.
      return false;
   }
}
