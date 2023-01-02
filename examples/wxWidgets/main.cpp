#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "CGEditor.hpp"
#include "MyHalfMove.hpp"

/**
 * @brief CGEditor Window
 *
 */
class MyFrame : public wxFrame, public cgeditor::CGEditor {
  wxPaintDC *dc;

public:
  MyFrame()
      : wxFrame(NULL, wxID_ANY, "Hello World CGEditor"), CGEditor(), dc(NULL) {
    CreateStatusBar();
    SetStatusText("CGEditor");
    // Create a game
    CGEditor::status.Moves = BuildExampleGame();
  }

private:
  void OnExit(wxCommandEvent &event) { Close(true); }

  void OnPaint(wxPaintEvent &event) {
    wxPaintDC current_dc(this);
    dc = &current_dc;

    // Just in case you want to fetch font sizes to configure comment text:
    //wxSize fontsize=dc->GetTextExtent("a");
    //wxLogDebug("width=%d, height=%d",fontsize.x,fontsize.y);

    // Refresh canvas size
    wxSize sz = GetClientSize();
    CGEditor::status.CanvasWidth = sz.GetWidth();
    CGEditor::status.CanvasHeight = sz.GetHeight();
    CGEditor::status.UseMoveIcons =
        true; // Piece image should be drawn before the move ?

    const wxPoint pt = wxGetMousePosition();
    CGEditor::status.MouseX = pt.x - this->GetScreenPosition().x;
    CGEditor::status.MouseY = pt.y - this->GetScreenPosition().y;
    CGEditor::Draw();
  }

  /**
   * @brief We refresh CGEditor status according to events
   *
   * @param event
   */
  void MouseEvent(wxMouseEvent &event) {
    if (event.Dragging()) {
      CGEditor::status.LeftClick = false;
      CGEditor::status.IsDrag = true;
      Refresh();
    } else if (event.LeftDown()) {
      CGEditor::status.LeftClick = true;
      Refresh();
    } else if (event.RightDown()) {
      CGEditor::status.RightClick = true;
      Refresh();
    } else if (event.GetWheelRotation() != 0) {
      if (event.GetWheelRotation() < 0) {
        CGEditor::status.EventVScroll = 50;
      } else {
        CGEditor::status.EventVScroll = -50;
      }
      Refresh();
    }

    // Now handle generated events from last user inputs
    Update();
    if(ProcessEvents()){
      Refresh();
    }
  }

  /**
   * @brief Convenient fonction to center text
   *
   * @param e Element to center
   * @return wxPoint The centered version of e according to wdWidget API
   */
  wxPoint Middle(cgeditor::Element e) {
    wxSize sz = dc->GetTextExtent(e.text);
    return (wxPoint(e.x + (e.width - sz.GetWidth()) / 2,
                    e.y + (e.height - sz.GetHeight()) / 2));
  }

  /**
   * @brief CGEditor is going to call this method with the elements to draw on
   * the canvas
   *
   * @param e Element to draw
   */
  void DrawElement(const cgeditor::Element &e) {
    dc->SetPen(wxNullPen);
    dc->SetBrush(*wxRED_BRUSH);
    if (e.prop & cgeditor::Property::Rectangle) {
      if (e.prop & cgeditor::Property::Scrollbarbg) {
        dc->SetBrush(*wxCYAN_BRUSH);
      } else if (e.prop & cgeditor::Property::Scrollbar) {
        dc->SetBrush(*wxBLUE_BRUSH);
      } else if (e.prop & cgeditor::Property::Margin) {
        dc->SetBrush(*wxLIGHT_GREY_BRUSH);
      } else if (e.prop & cgeditor::Property::Button) {
        dc->SetBrush(*wxBLACK_BRUSH);
      } else if (e.prop & cgeditor::Property::Comment) {
        dc->SetBrush(*wxYELLOW_BRUSH);
      }
      wxRect recToDraw(e.x, e.y, e.width, e.height);
      dc->DrawRectangle(recToDraw);
    } else if (e.prop & cgeditor::Property::Text ||
               e.prop & cgeditor::Property::Image) {
      if (e.prop & cgeditor::Property::Image) {
        // Draw your pieces images instead
        dc->SetBrush(*wxRED_BRUSH);
        wxRect recToDraw(e.x, e.y, e.width, e.height);
        dc->DrawRectangle(recToDraw);
        dc->DrawText(wxString(e.text), Middle(e));
      } else if (e.prop & cgeditor::Property::Comment) {
        wxRect recToDraw(e.x, e.y, e.width, e.height);
        dc->SetBrush(*wxBLUE_BRUSH);
        dc->DrawRectangle(recToDraw);
        dc->DrawText(wxString(e.text), wxPoint(e.x, e.y));
      } else if (e.prop & cgeditor::Property::Menuitem) {
        wxRect recToDraw(e.x, e.y, e.width, e.height);
        dc->SetBrush(*wxLIGHT_GREY_BRUSH);
        dc->DrawRectangle(recToDraw);
        dc->DrawText(wxString(e.text), wxPoint(e.x, Middle(e).y));
      } else {
        if (e.prop & cgeditor::Property::Move) {
          if (e.prop & cgeditor::Property::Current) {
            wxRect recToDraw(e.x, e.y, e.width, e.height);
            dc->SetBrush(*wxLIGHT_GREY_BRUSH);
            dc->DrawRectangle(recToDraw);
          }
          if (CGEditor::status.UseMoveIcons) {
            dc->DrawText(wxString(e.text), wxPoint(e.x, Middle(e).y));
          } else {
            dc->DrawText(wxString(e.text), Middle(e));
          }
        } else {
          dc->DrawText(wxString(e.text), Middle(e));
        }
      }
    }
  }

  /**
   * @brief CGEditor events that occurs during last draw
   *
   * @param e event to handle
   */
  void HandleEvent(const cgeditor::Event &e) {
    std::string str;
    if (e.type == cgeditor::Event::Type::CommentSelected)
      str = "Comment Selected";
    else if (e.type == cgeditor::Event::Type::Promote) {
      str = "Promote";
      static_cast<MyHalfMove *>(e.move)->MyHalfMove::Promote();
    } else if (e.type == cgeditor::Event::Type::Delete) {
      str = "Delete";
      if (e.move->Parent != NULL) {
        static_cast<MyHalfMove *>(e.move)->GetParent()->MyHalfMove::RemoveChild(
            (MyHalfMove *)e.move);
      } else {
        CGEditor::status.Moves = NULL;
      }
    } else if (e.type == cgeditor::Event::Type::SetAsMainline) {
      str = "Set as main line";
      static_cast<MyHalfMove *>(e.move)->MyHalfMove::SetAsMainline();
    } else if (e.type == cgeditor::Event::Type::Goto) {
      str = "Goto move";
    }
    std::cout << "Event received: " << str << std::endl << std::flush;
  }

  // wxWidgets specific
  DECLARE_EVENT_TABLE()
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_PAINT(MyFrame::OnPaint)
EVT_MOUSE_EVENTS(MyFrame::MouseEvent)
wxEND_EVENT_TABLE()

class MyApp : public wxApp {
public:
  virtual bool OnInit() {
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
  }
};
wxIMPLEMENT_APP(MyApp);
