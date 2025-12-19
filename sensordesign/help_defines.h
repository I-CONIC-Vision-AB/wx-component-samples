#pragma once

#   include "wx/image.h"
#   include "wx/help.h"
#   include "wx/cshelp.h"

// define this to 1 to use HTML help even under Windows (by default, Windows
// version will use WinHelp).
// Please also see samples/html/helpview.
#define USE_HTML_HELP 1

// define this to 1 to use external help controller (not used by default)
#define USE_EXT_HELP 0

// Define this to 0 to use the help controller as the help
// provider, or to 1 to use the 'simple help provider'
// (the one implemented with wxTipWindow).
#define USE_SIMPLE_HELP_PROVIDER 0

#if !wxUSE_HTML
#undef USE_HTML_HELP
#define USE_HTML_HELP 0
#endif

#if USE_HTML_HELP
#include "wx/filesys.h"
#include "wx/fs_zip.h"

#include "wx/html/helpctrl.h"
#endif

#if wxUSE_MS_HTML_HELP && !defined(__WXUNIVERSAL__)
#include "wx/msw/helpchm.h"
#endif

#if wxUSE_MS_HTML_HELP && wxUSE_WXHTML_HELP && !defined(__WXUNIVERSAL__)
#include "wx/msw/helpbest.h"
#endif

#if USE_EXT_HELP
#include "wx/generic/helpext.h"
#endif

class Helper
{
public:
	wxHelpControllerBase& GetHelpController() { return m_help; }

#if USE_HTML_HELP
	wxHtmlHelpController& GetAdvancedHtmlHelpController() { return m_advancedHtmlHelp; }
#endif
#if wxUSE_MS_HTML_HELP && !defined(__WXUNIVERSAL__)
	wxCHMHelpController& GetMSHtmlHelpController() { return m_msHtmlHelp; }
#endif
#if wxUSE_MS_HTML_HELP && wxUSE_WXHTML_HELP && !defined(__WXUNIVERSAL__)
	wxBestHelpController& GetBestHelpController() { return m_bestHelp; }
#endif

private:
#if USE_EXT_HELP
	wxExtHelpController      m_help;
#else
	wxHelpController         m_help;
#endif

#if USE_HTML_HELP
	wxHtmlHelpController     m_advancedHtmlHelp;
	wxHtmlHelpController     m_embeddedHtmlHelp;
	wxHtmlHelpWindow*        m_embeddedHelpWindow;
#endif

#if wxUSE_MS_HTML_HELP && !defined(__WXUNIVERSAL__)
	wxCHMHelpController     m_msHtmlHelp;
#endif

#if wxUSE_MS_HTML_HELP && wxUSE_WXHTML_HELP && !defined(__WXUNIVERSAL__)
	wxBestHelpController    m_bestHelp;
#endif

};
