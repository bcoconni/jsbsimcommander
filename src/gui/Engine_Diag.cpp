/////////////////////////////////////////////////////////////////////////////
// Name:        Engine_Diag.h
// Purpose:     Dialog for General Engine
// Author:      Matthew Gong
// Created:     07/26/2005
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

#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/filename.h>

#include "Engine_Diag.h"

// -*- C++ -*- generated by wxGlade 0.4cvs on Tue Jun 28 16:57:11 2005

bool newDirect(const wxString & filename)
{ 
  if (!::wxFileExists(filename))
  {
    wxFileOutputStream os(filename);
    if (!os.Ok())
      return false;
    wxTextOutputStream out(os,wxEOL_UNIX);
    wxString name = filename.AfterLast(wxFileName::GetPathSeparator());
    name = name.BeforeLast('.');

    out << wxT("<?xml version=\"1.0\"?>\n");

    out << wxT("<direct name=\"") << name << wxT("\">\n");
    out << wxT("</direct>\n");
    return true;
  }

  return false;
}

EngineDialog::EngineDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
}

