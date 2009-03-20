#include "MainFrame.h"
#include "ImageViewer.h"
#include "KernelFaker.h"

#include "wx/settings.h"

#define  REFRESH_DELAY     50
#define  ROM_LOAD_ADDRESS  0x00100000

//Status bar
enum StatusBar
{
	SB_MENU,
	SB_INFO,
	SB_CNT
};

/////////////////////////////////////////////////////////////////////////
// Menu items.
/////////////////////////////////////////////////////////////////////////
enum Menu
{
   ID_MENU_FILE_OPENISO = 1,
   ID_MENU_FILE_OPENCODE,
   ID_MENU_FILE_EXIT,
   ID_MENU_CONSOLE_RESET,
   ID_MENU_TOOLS_BROWSEISO,
   ID_MENU_HELP_ABOUT
};

enum Timer
{
	ID_TIMER_MAIN = 1,
};

BEGIN_EVENT_TABLE( MainFrame, wxFrame )
   EVT_MENU ( ID_MENU_FILE_OPENISO,    MainFrame::OnMenuFileOpenISO )
   EVT_MENU ( ID_MENU_FILE_OPENCODE,   MainFrame::OnMenuFileOpenCode)
   EVT_MENU ( ID_MENU_FILE_EXIT,       MainFrame::OnMenuFileExit )
   EVT_MENU ( ID_MENU_CONSOLE_RESET,   MainFrame::OnMenuConsoleReset )
   EVT_MENU ( ID_MENU_TOOLS_BROWSEISO, MainFrame::OnMenuToolsBrowseISO )
   EVT_MENU ( ID_MENU_HELP_ABOUT,      MainFrame::OnMenuHelpAbout )
   EVT_TIMER( ID_TIMER_MAIN,	       MainFrame::OnMainTimer )
END_EVENT_TABLE()

/////////////////////////////////////////////////////////////////////////
// Frame startup
/////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxCmdLineParser* parser )
		: wxFrame( ( wxFrame * ) NULL, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize )
{
	wxString fileName;

	// Create a console
	m_con = new Console();
	m_con->CPU()->SetPC( ROM_LOAD_ADDRESS );

	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	// KILL ME
	wxFile file;
	
	// Put the stack pointers somewhere
	m_con->CPU()->ARM.USER[13] = 0x00040000;
	m_con->CPU()->ARM.SVC[0]   = 0x00041000;
	m_con->CPU()->ARM.ABT[0]   = 0x00042000;
	m_con->CPU()->ARM.IRQ[0]   = 0x00043000;
	m_con->CPU()->ARM.UND[0]   = 0x00044000;
	m_con->CPU()->ARM.FIQ[5]   = 0x00045000;
	
	// Set up registers
	m_con->CPU()->ARM.USER[0]  = 0x00000000;
	m_con->CPU()->ARM.USER[1]  = 0x00000000;
	m_con->CPU()->ARM.USER[2]  = 0x00000000;
	m_con->CPU()->ARM.USER[3]  = 0x00000000;
	m_con->CPU()->ARM.USER[4]  = 0xFFFFFFFC;
	m_con->CPU()->ARM.USER[5]  = 0x00000000;
	m_con->CPU()->ARM.USER[6]  = 0x00000000;
	m_con->CPU()->ARM.USER[7]  = 0x00021230;
	m_con->CPU()->ARM.USER[8]  = 0x00000000;
	m_con->CPU()->ARM.USER[9]  = 0x400002CC;
	m_con->CPU()->ARM.USER[10] = 0x0007EFE0;
	m_con->CPU()->ARM.USER[11] = 0x00000000;
	m_con->CPU()->ARM.USER[12] = 0x00078508;
	
	sprintf( (char*)m_con->DMA()->GetRAMPointer( m_con->CPU()->ARM.USER[13] ), "$app/Launchme" );
	
	///////////////////////////
	// Fake out the Kernel!
	KernelFaker kernelFaker;
	kernelFaker.InitializeFakeKernel( m_con->DMA() );


	//m_con->DMA()->SetWord(0,  0);
	//m_con->DMA()->SetWord(4,  3925886382);
	//m_con->DMA()->SetWord(8,  3925886486);
	//m_con->DMA()->SetWord(12, 3925886392);
	//m_con->DMA()->SetWord(16, 3925886405);
	//m_con->DMA()->SetWord(20, 0);
	//m_con->DMA()->SetWord(24, 3925886582);
	//m_con->DMA()->SetWord(28, 3925886586);

	//m_con->DMA()->SetWord(0x11854, 0xE92D0100);
	//m_con->DMA()->SetWord(0x11858, 0xE10F8000);
	//m_con->DMA()->SetWord(0x1185C, 0xE3C88080);
	//m_con->DMA()->SetWord(0x11860, 0xE121F008);
	//m_con->DMA()->SetWord(0x11864, 0xEA00295B);

	//m_con->DMA()->SetWord(0x11868, 0xE92D4F00);
	//m_con->DMA()->SetWord(0x1186C, 0xE14F8000);
	//m_con->DMA()->SetWord(0x11870, 0xE2188003);
	//m_con->DMA()->SetWord(0x11874, 0x1AFFFFF6);
	//m_con->DMA()->SetWord(0x11878, 0xE51EE004);
	//m_con->DMA()->SetWord(0x1187C, 0xE3DEE4FF);
	
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////

	/////////////////////
	// GUI Setup.
	this->SetTitle( "4DO" );
	this->SetIcon( wxIcon( fourdo_xpm ) );
	this->SetSize( 648, 546 );
	this->CenterOnScreen();
	
	this->SetBackgroundColour( wxSystemSettings::GetColour ( wxSYS_COLOUR_WINDOW ) );
	this->InitializeMenu ();
	
	wxBoxSizer* mainSizer = new wxBoxSizer( wxHORIZONTAL );
	ctlCanvas = new MainCanvas( this, wxID_ANY, m_con->DMA()->GetRAMPointer( 0x002c0000 ) );
	ctlCanvas->SetBackgroundColour( *wxLIGHT_GREY );
	mainSizer->Add( ctlCanvas, 1, wxEXPAND, 0, NULL );
	this->SetSizer( mainSizer );

	this->CreateStatusBar();
	this->GetStatusBar()->SetFieldsCount( SB_CNT );
	this->SetStatusText( _T( "4DO: Open-Source HLE 3DO Emulator" ), SB_MENU );

	///////////////
	// Set up timer?
	tmrMain = new wxTimer( this, ID_TIMER_MAIN );
	tmrMain->Start( REFRESH_DELAY );

	/////////////////////
	// Handle command-line arguments.
	if( parser->Found( "li" ) )
	{
		parser->Found( "li", &fileName );
		this->ConsoleLoadIso( fileName );
	}
	else if( parser->Found( "lc" ) )
	{
		parser->Found( "lc", &fileName );
		this->ConsoleLoadCode( fileName );
	}
}

MainFrame::~MainFrame()
{
	delete m_con;
}

void MainFrame::RunCycles() 
{
	if( m_consoleRunning )
	{
		uint	    cycles;
		wxStopWatch sw;
		
		//////////////////////////////
		// Start execution timer.
		sw.Start();
		
		//cycles = time_used / 0.00008;
		//cycles = REFRESH_DELAY / 0.00008;
		cycles = REFRESH_DELAY / 0.00008 * 1.2;
		m_con->CPU()->Execute( cycles );

		// End execution timer.
		sw.Pause();
		
		// Display total time metric
		this->SetStatusText( wxString::Format( "Time: %ldms     Cycles:%u", sw.Time(), cycles ), SB_INFO );
	}
	else
	{
		// Display total time metric
		this->SetStatusText( _T( "Not Running" ), SB_INFO );
	}
}

void MainFrame::InitializeMenu ()
{
	wxMenuBar* mnuMain    = new wxMenuBar ();
	wxMenu*    mnuFile    = new wxMenu ();
	wxMenu*    mnuTools   = new wxMenu ();
	wxMenu*    mnuHelp    = new wxMenu ();

	this->SetMenuBar (mnuMain);

	//////////////////////
	// File menu
	mnuMain->Append( mnuFile, _T( "&File" ));
	mnuFile->Append( ID_MENU_FILE_OPENISO,  _T( "&Open ISO...\tCtrl+O" ) );
	mnuFile->Append( ID_MENU_FILE_OPENCODE, _T( "&Open ARM Binary..." ) );
	mnuFile->AppendSeparator();
	mnuFile->Append( ID_MENU_FILE_EXIT, _T( "&Exit\tCtrl+X" ) );

	/*
	//////////////////////
	// Console menu
	mnuMain->Append( mnuConsole, _T( "&Console" ) );
	mnuConsole->Append( ID_MENU_CONSOLE_RESET, _T( "&Reset...\tCtrl+R" ) );
	*/

	//////////////////////
	// Tools menu
	mnuMain->Append( mnuTools, _T( "&Tools" ) );
	mnuTools->Append( ID_MENU_TOOLS_BROWSEISO, _T( "&Browse ISO...\tCtrl+B" ) );

	//////////////////////
	// Help menu
	mnuMain->Append( mnuHelp, _T( "&Help" ) );
	mnuHelp->Append( ID_MENU_HELP_ABOUT, _T( "&About...\tShift+F1" ) );
}

/////////////////////////////////////////////////////////////////////////
// Event handlers
/////////////////////////////////////////////////////////////////////////
void MainFrame::OnMainTimer( wxTimerEvent & )
{
	this->RunCycles();
	ctlCanvas->UpdateImage();
	ctlCanvas->Refresh();
}

void MainFrame::OnMenuFileOpenISO( wxCommandEvent& WXUNUSED( event ) )
{
	wxString fileName = wxFileSelector( _T( "Open 3DO ISO File" ), NULL, NULL, NULL, _T( "ISO Files (*.iso)|*.iso|All files (*.*)|*.*" ), wxFD_OPEN | wxFD_FILE_MUST_EXIST );

	if( !fileName.empty() )
	{
		this->ConsoleLoadIso( fileName );
	}
}

void MainFrame::OnMenuFileOpenCode( wxCommandEvent& WXUNUSED( event ) )
{
	wxString fileName = wxFileSelector( _T( "Open ARM Code File" ), NULL, NULL, NULL, _T( "Code files (*.3bn)|*.3bn|All files (*.*)|*.*" ), wxFD_OPEN | wxFD_FILE_MUST_EXIST );

	if( !fileName.empty() )
	{
		this->ConsoleLoadCode( fileName );
	}
}

void MainFrame::OnMenuFileExit (wxCommandEvent& WXUNUSED( event ) )
{
	this->Close();
}

void MainFrame::OnMenuToolsBrowseISO( wxCommandEvent& WXUNUSED( event ) )
{
	this->BrowseIso();
}

void MainFrame::OnMenuHelpAbout( wxCommandEvent& WXUNUSED( event ) )
{
	wxMessageBox( _T( "FourDO - An Open-Source HLE 3DO Emulator\n\nVersion 0.0.0.1" ), _T( "About 4DO" ), wxOK | wxICON_INFORMATION );
}

void MainFrame::OnMenuConsoleReset( wxCommandEvent& WXUNUSED( event ) )
{
	this->ConsoleReset();
}

void MainFrame::BrowseIso()
{
	wxString fileName = wxFileSelector( _T( "Open 3DO ISO File" ), NULL, NULL, NULL, _T( "ISO Files (*.iso)|*.iso|All files (*.*)|*.*" ), wxFD_OPEN | wxFD_FILE_MUST_EXIST );
	
	if( !fileName.empty() )
	{
		this->BrowseIso( fileName );
	}
}

void MainFrame::BrowseIso( wxString fileName )
{
	ISOBrowser* browser;
	browser = new ISOBrowser( this, fileName );
	browser->Show();
}

void MainFrame::ConsoleReset ()
{
	/////////////////////
	// TODO: Properly reset console
	
	// Initialize the PC
	m_con->CPU()->Reset();
	
	m_loadIso = false;
	m_fileName = wxEmptyString;
	m_consoleRunning = false;
}

void MainFrame::ConsoleLoadIso( wxString fileName )
{
	bool  success;
	uint  fileSize;
	
	/////////////////
	// Load the ISO

	// Open the Launchme of this CD image.
	uint32_t  bytesRead;
	File f( fileName );

	success = f.openFile( "/launchme" );
	if( !success )
	{
		// Error opening
		return;
	}

	// Load it into memory.
	fileSize = f.getFileSize();
	f.read( m_con->DMA ()->GetRAMPointer( ROM_LOAD_ADDRESS ), f.getFileSize (), &bytesRead );
	
	// Initialize the PC
	m_con->CPU()->SetPC( ROM_LOAD_ADDRESS );

	m_loadIso = true;
	m_fileName = fileName;
	m_consoleRunning = true;
}

void MainFrame::ConsoleLoadCode ( wxString fileName )
{
	// Open a code file.
	wxFile file;
	
	// Load the code into memory
	file.Open( fileName );
	file.Read( m_con->DMA()->GetRAMPointer( ROM_LOAD_ADDRESS ), file.Length () );
	file.Close();

	m_con->CPU()->SetPC( ROM_LOAD_ADDRESS );
	
	m_loadIso = false;
	m_fileName = fileName;
	m_consoleRunning = true;
}