/////////////////////////////////////////////////////////////////////////////
// Name:        Commander.cpp
// Purpose:     Commander for JSBSim
// Author:      Matthew Gong
// Created:     05/03/2005
// Copyright:   (c) Matthew Gong
// Licence:     GPL licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
// #pragma implementation
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/tooltip.h>
#include <wx/splash.h>
#include <wx/cmdline.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/xrc/xmlres.h>          // XRC XML resouces
#include <wx/image.h>               // wxImage
#include <wx/wfstream.h>            // wxFileOutputStream

#include "MyApp.h"
#include "AircraftDialog.h"
#include "Property_Diag.h"
#include "EngineMgr_Diag.h"

#include "initprops.h"

//-----------------------------------------------------------------------------
// Regular resources (the non-XRC kind).
//-----------------------------------------------------------------------------

// The application icon
// All non-MSW platforms use an xpm. MSW uses an .ico file
#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__)
    #include "ogl.xpm"
#endif

/*
 * A macro needed for some compilers (AIX) that need 'main' to be defined
 * in the application itself.
 */

IMPLEMENT_APP (MyApp)

/**
* MyApp ========================================================================
*/

MyApp::MyApp (void)
{
}

/*
 * =============================================================================
 * The `main program' equivalent, creating the windows and returning the
 * main frame
 */

bool MyApp::OnInit (void)
{
  m_locale.AddCatalogLookupPathPrefix(wxString(argv[0]).BeforeLast(wxFileName::GetPathSeparator()));
  m_locale.Init(wxLANGUAGE_DEFAULT);
  // Initialize the catalogs we'll be using
  m_locale.AddCatalog(wxT("jsbsimcommander"));

  Delimited = wxFileName::GetPathSeparator();
  if (!wxApp::OnInit ())
     return false;

  wxBitmap about;
  if ( wxFileExists(about_img_file) && about.LoadFile(about_img_file, wxBITMAP_TYPE_BMP))
  {
    wxSplashScreen* splash = new wxSplashScreen(about,
          wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
          3000, NULL, -1, wxDefaultPosition, wxDefaultSize,
          wxSIMPLE_BORDER|wxSTAY_ON_TOP);
  }

  // If there is any of a certain format of image in the xrcs, then first
  // load a handler for that image type. This example uses XPMs, but if
  // you want PNGs, then add a PNG handler, etc. See wxImage::AddHandler()
  // documentation for the types of image handlers available.
  //wxImage::AddHandler(new wxXPMHandler);
  ::wxInitAllImageHandlers();

  // Initialize all the XRC handlers. Always required (unless you feel like
  // going through and initializing a handler of each control type you will
  // be using (ie initialize the spinctrl handler, initialize the textctrl
  // handler). However, if you are only using a few control types, it will
  // save some space to only initialize the ones you will be using. See
  // wxXRC docs for details.
  wxXmlResource::Get()->InitAllHandlers();

  // Load all of the XRC files that will be used. You can put everything
  // into one giant XRC file if you wanted, but then they become more
  // diffcult to manage, and harder to reuse in later projects.
  // The menubar
  wxString filename;
  bool cont = rc_dir.GetFirst(&filename, wxT("*.xrc"), wxDIR_FILES);
  if (!cont)
  {
    wxMessageBox(_("Fail to load XRC files. Please use --resource=<str> or -r <str> to specify the resource directory."), _("Error"), wxOK | wxICON_ERROR);
    return false;
  }
  while ( cont )
  {
	wxXmlResource::Get()->Load(
	  rc_dir.GetName()+wxFileName::GetPathSeparator()+filename);
      cont = rc_dir.GetNext(&filename);
  }

  InitDir();
  InitCfg();

  //// Create the main frame window
  dlg = new AircraftDialog (NULL, -1);

  //// Give it an icon
  dlg->SetIcon (wxICON (ogl));

  property_dialog = new PropertyDialog(dlg);
  if (! property_dialog->Load(prop_file) )
  {
    wxSafeShowMessage(_("Fail to load property table "), _("Fail to load property table ")+prop_file+_(". ")+_("Use option -p  to specify the file to load."));
    return false;
  }

  wxToolTip::Enable(true);
  wxToolTip::SetDelay(300);

  if (!inputfile.IsEmpty() && ! dlg->Load(inputfile))
  {
      ::wxMessageBox(wxString(_("Something is wrong! I can not load proper file(")) + inputfile + wxT(")."), _("Warning"), wxOK | wxICON_INFORMATION, NULL);
  }
  else
  {
    //dlg->Maximize(true);
    if (dlg->ShowModal () == wxID_OK)
    {
      dlg->Save();
    }
    property_dialog->Destroy();
    dlg->Destroy();
  }

  return true;
}

/**
* OnExit =======================================================================
*/

int MyApp::OnExit (void)
{
  return 0;
}

/**
* OnInitCmdLine ================================================================
*/

void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
  CurrentWorkDirectory = wxGetCwd();
  AppDirectory = argv[0];
  AppDirectory = AppDirectory.BeforeLast(wxFileName::GetPathSeparator());  
  // Do not change current working directory to where the app is.
  //wxSetWorkingDirectory(AppDirectory);
    static const wxCmdLineEntryDesc cmdLineDesc[] =
    {
        { wxCMD_LINE_SWITCH, _T("h"), _T("help"), _T("show this help message"),
            wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
        { wxCMD_LINE_SWITCH, _T("v"), _T("verbose"), _T("be verbose") },
        { wxCMD_LINE_SWITCH, _T("q"), _T("quiet"),   _T("be quiet") },
        { wxCMD_LINE_OPTION, _T("r"), _T("resource"), _T("resource dir") },
	    { wxCMD_LINE_OPTION, _T("a"), _T("about"), _T("about image file") },
        { wxCMD_LINE_OPTION, _T("p"), _T("properties"),   _T("property file") },

        /*
        { wxCMD_LINE_OPTION, _T("o"), _T("output"),  _T("output file") },
        { wxCMD_LINE_OPTION, _T("i"), _T("input"),   _T("input dir") },
        { wxCMD_LINE_OPTION, _T("s"), _T("size"),    _T("output block size"),
            wxCMD_LINE_VAL_NUMBER },
        { wxCMD_LINE_OPTION, _T("d"), _T("date"),    _T("output file date"),
            wxCMD_LINE_VAL_DATE },
        */

        { wxCMD_LINE_PARAM,  NULL, NULL, _T("input file"),
            wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },

        { wxCMD_LINE_NONE }
    };
    parser.SetDesc(cmdLineDesc);
    /*
    parser.AddOption(_T("project_name"),_T(""),_T("full path to project file"),
                     wxCMD_LINE_VAL_STRING,
                     wxCMD_LINE_NEEDS_SEPARATOR);
    */
}

/**
* OnCmdLineParsed ==============================================================
*/

bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
  if (wxApp::OnCmdLineParsed(parser))
  {
    wxString dir;
    if (parser.Found(wxT("resource"), &dir))
    {
      rc_dir.Open(dir);
    }
    else
    {
      rc_dir.Open(AppDirectory+wxFileName::GetPathSeparator()+wxT("rc"));
    }
    if (parser.Found(wxT("about"), &dir))
    {
      about_img_file = dir;
    }
    else
    {
      about_img_file = AppDirectory+wxFileName::GetPathSeparator()+wxT("about.bmp");
    }
    if (parser.Found(wxT("properties"), &dir))
    {
      prop_file = dir;
    }
    else
    {
      prop_file = CurrentWorkDirectory+wxFileName::GetPathSeparator()+wxT("prop.csv");
    }
    if (!::wxFileExists(prop_file))
    {
      wxArrayString source_property;
      wxArrayString source_desc;
      wxArrayString source_unit;
      wxArrayString source_access;
      wxArrayString source_comment;
  
      wxArrayString destination_property;
      wxArrayString destination_desc;
      wxArrayString destination_unit;
      wxArrayString destination_access;
      wxArrayString destination_comment;

      wxArrayString all_property;
      wxArrayString all_desc;
      wxArrayString all_unit;
      wxArrayString all_access;
      wxArrayString all_comment;

      InitProps(source_property, source_desc, source_unit, source_access, source_comment, destination_property, destination_desc, destination_unit, destination_access, destination_comment, all_property, all_desc, all_unit, all_access, all_comment);
      wxFileOutputStream os(prop_file);
      if (!os.Ok())
        return false;
      wxTextOutputStream out(os);
      out << _("\"property name\"") << wxT(",") 
        << _("\"description\"") << wxT(",")
        << _("\"unit\"") << wxT(",")
        << _("\"access\"") << wxT(",")
        << _("\"comment\"") << wxT("\n");
      for (int i=0; i < all_property.GetCount(); ++i)
      {
        out << wxT("\"") << all_property[i] << wxT("\"") << wxT(",") 
          << wxT("\"") << all_desc[i] << wxT("\"") << wxT(",")
          << wxT("\"") << all_unit[i] << wxT("\"") << wxT(",")
          << wxT("\"") << all_access[i] << wxT("\"") << wxT(",")
          << wxT("\"") << all_comment[i] << wxT("\"") << wxT("\n");
      }
    }
    size_t count = parser.GetParamCount();
    if (count > 0u)
    {
      inputfile = parser.GetParam(0u);
    }
    return true;
  }
  else
  {
    return false;
  }
}

/**
* InitDir ======================================================================
*/

void MyApp::InitDir (void)
{
  engine_dir = wxT("engine");
  if (! ::wxDirExists(engine_dir))
  {
    ::wxMkdir(engine_dir);
  }
}

/**
* InitCfg ======================================================================
*/

void MyApp::InitCfg (void)
{
  wxString ini = wxT("EngineMgr.ini");
  EngineMgrDialog::InitEngineCfg(); 
}

/**
* GetMainDlg ===================================================================
*/

AircraftDialog *GetMainDlg (void)
{
  return wxGetApp().dlg;
}

/**
* GetPropertyDialog ============================================================
*/

PropertyDialog *GetPropertyDialog (void)
{
  return wxGetApp().property_dialog;
}

