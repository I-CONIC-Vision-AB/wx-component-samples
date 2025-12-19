/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWidgets sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "help_defines.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "../sample.xpm"
#endif

#include	"SensorPanel.h"
#include	<wx/aboutdlg.h>

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit() wxOVERRIDE;
};



// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame, public Helper
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnHelpContents(wxCommandEvent& WXUNUSED(event));
	void OnCopyToClipboard(wxCommandEvent& event);

private:
	SensorPanel * cpPanel;
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
	EVT_MENU(wxID_HELP_CONTENTS, MyFrame::OnHelpContents)
	EVT_MENU(SensorPanel::CLIPBOARD, MyFrame::OnCopyToClipboard)
wxEND_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
wxIMPLEMENT_APP(MyApp);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

	// Create a simple help provider to make SetHelpText() do something.
	// Note that this must be set before any SetHelpText() calls are made.
#if USE_SIMPLE_HELP_PROVIDER
	wxSimpleHelpProvider* provider = new wxSimpleHelpProvider;
#else
	wxHelpControllerHelpProvider* provider = new wxHelpControllerHelpProvider;
#endif
	wxHelpProvider::Set(provider);

#if USE_HTML_HELP
#if wxUSE_GIF
	// Required for images in the online documentation
	wxImage::AddHandler(new wxGIFHandler);
#endif // wxUSE_GIF

	// Required for advanced HTML help
#if wxUSE_STREAMS && wxUSE_ZIPSTREAM && wxUSE_ZLIB
	wxFileSystem::AddHandler(new wxZipFSHandler);
#endif
#endif // wxUSE_HTML



    // create the main application window
    MyFrame *frame = new MyFrame("I-CONIC SensorDesign v0.3");

#if !USE_SIMPLE_HELP_PROVIDER
#if wxUSE_MS_HTML_HELP && !defined(__WXUNIVERSAL__)
	provider->SetHelpController(&frame->GetMSHtmlHelpController());
#else
	provider->SetHelpController(&frame->GetHelpController());
#endif
#endif // !USE_SIMPLE_HELP_PROVIDER


	frame->Maximize();

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

	// initialise the help system: this means that we'll use doc.hlp file under
	// Windows and that the HTML docs are in the subdirectory doc for platforms
	// using HTML help
	wxString helpFile(wxGetCwd() + "/ssd");

#if USE_HTML_HELP
	// initialise the advanced HTML help system: this means that the HTML docs are in .htb
	// (zipped) form
	if (!frame->GetAdvancedHtmlHelpController().Initialize(helpFile))
	{
		wxLogError("Cannot initialize the advanced HTML help system, aborting.");

		//return false;
	}
#endif

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
    // set the frame icon
    SetIcon(wxICON(iconic_icon));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
	
	helpMenu->Append(wxID_HELP_CONTENTS, "&Help\tF1", "Show documentation");
//	helpMenu->Append(wxID_HELP_CONTENTS, "&Help in web browser\tF1", "Open default web browser and show documentation");
	helpMenu->Append(Minimal_About, "&About\tF2", "Show about dialog");

	fileMenu->Append(SensorPanel::CLIPBOARD, _("Copy to clipboard\tCtrl+C"), _("Copy current data to clipboard"));
    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#else // !wxUSE_MENUS
    // If menus are not available add a button to access the about box
    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* aboutBtn = new wxButton(this, wxID_ANY, "About...");
    aboutBtn->Bind(wxEVT_BUTTON, &MyFrame::OnAbout, this);
    sizer->Add(aboutBtn, wxSizerFlags().Center());
#endif // wxUSE_MENUS/!wxUSE_MENUS

	cpPanel = new SensorPanel(this);

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Ready");
#endif // wxUSE_STATUSBAR
}


// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxAboutDialogInfo info;
	info.SetName(GetTitle());
//	info.SetVersion(wxT("0.2"));
	info.SetWebSite(wxT("http://www.i-conic.eu/"));
	//info.SetLicense(wxT("WiMap licenses are issued by ReVisitor AB, SWEDEN. Visit our web site to aquire pricing information."));
	info.SetCopyright(wxT("(C) I-CONIC Vision AB "));
	info.AddDeveloper(wxT("Håkan Wiman, PhD"));
	wxString description = wxString::Format(_("A software that computes parameters for a perspective frame camera.\n\nRunning on: %s"), wxGetOsDescription());
	info.SetDescription(description);
	info.SetIcon(wxIcon(wxString("iconic_icon"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	wxAboutBox(info, this);
}

void MyFrame::OnHelpContents(wxCommandEvent& WXUNUSED(event))
{
	GetAdvancedHtmlHelpController().DisplayContents();
}

void MyFrame::OnCopyToClipboard(wxCommandEvent& event)
{
	cpPanel->OnCopyToClipboardVertical(event);
}
