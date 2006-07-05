/////////////////////////////////////////////////////////////////////////////
// Name:        pid_dlg.h
// Purpose:     dialog for FGPID
// Author:      Jon Berndt
// Created:     07/02/2006
// Copyright:   (c) Jon Berndt
// Licence:     GPL licence
/////////////////////////////////////////////////////////////////////////////

// -*- C++ -*- generated by wxGlade 0.4.1 on Tue Jul 04 14:40:04 2006

#include <wx/wx.h>
#include <wx/image.h>

#ifndef PIDPROPERTYDIALOG_H
#define PIDPROPERTYDIALOG_H

// begin wxGlade: ::dependencies
// end wxGlade


class PID;

class PIDPropertyDialog: public wxDialog {
public:
    // begin wxGlade: PIDPropertyDialog::ids
    // end wxGlade

    PIDPropertyDialog(PID * pid, wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: PIDPropertyDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PIDPropertyDialog::attributes
    wxStaticBox* sizer_inout_staticbox;
    wxStaticBox* sizer_pid_top_staticbox;
    wxStaticBox* sizer_trigger_staticbox;
    wxStaticBox* sizer_clip_staticbox;
    wxStaticBox* sizer_basic_staticbox;
    wxStaticText* label_name;
    wxTextCtrl* text_ctrl_name;
    wxStaticText* label_type;
    wxTextCtrl* text_ctrl_type;
    wxStaticText* label_order;
    wxTextCtrl* text_ctrl_order;
    wxCheckBox* checkbox_clip;
    wxStaticText* label_clip_min;
    wxTextCtrl* text_ctrl_clip_min;
    wxStaticText* label_6;
    wxStaticText* label_clip_max;
    wxTextCtrl* text_ctrl_clip_max;
    wxStaticText* label_kp;
    wxTextCtrl* text_ctrl_kp;
    wxStaticText* label_ki;
    wxTextCtrl* text_ctrl_ki;
    wxStaticText* label_kd;
    wxTextCtrl* text_ctrl_kd;
    wxButton* button_trigger;
    wxTextCtrl* text_ctrl_trigger_prop;
    wxStaticText* label_1;
    wxTextCtrl* text_ctrl_input_prop;
    wxStaticText* label_output_property;
    wxTextCtrl* text_ctrl_output_prop;
    wxPanel* panel_toplevel;
    wxButton* button_OK;
    wxButton* button_Cancel;
    wxButton* button_Help;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnCheckboxClip(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnButtonPressOK(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnButtonPressCancel(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnButtonPressHelp(wxCommandEvent &event); // wxGlade: <event_handler>
    void GetDataIn(PID * g);
    void SetDataOut(PID * g);
    bool PIDPropertyDialog::Show(bool);

private:
  wxString name;
  wxString order;
  bool clipable;
  wxString clip_max;
  wxString clip_min;
//  int input;
  wxString kp, ki, kd;


}; // wxGlade: end class

#endif // PIDPROPERTYDIALOG_H
