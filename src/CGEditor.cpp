#include "CGEditor.hpp"
#include <iostream>
namespace cgeditor {

CGEditor::CGEditor() {
  SBV = new Scrollbar(&status, false);
  SBH = new Scrollbar(&status, true);
  MT = new MoveTable(&status);
  MA = new Margin(&status);
  ME = new Menu(&status);
  // Default Standard NagTable: https://en.wikipedia.org/wiki/Numeric_Annotation_Glyphs
  status.NagTable[0]="";
  status.NagTable[1]="!";
  status.NagTable[2]="?";
  status.NagTable[3]="!!";
  status.NagTable[4]="??";
  status.NagTable[5]="!?";
  status.NagTable[6]="?!";
  status.NagTable[7]="□";
  status.NagTable[10]="=";
  status.NagTable[13]="∞";
  status.NagTable[14]="⩲";
  status.NagTable[15]="⩱";
  status.NagTable[16]="±";
  status.NagTable[17]="∓";
  status.NagTable[18]="+-";
  status.NagTable[19]="-+";
  status.NagTable[22]="⨀";
  status.NagTable[23]="⨀";
  status.NagTable[26]="○";
  status.NagTable[27]="○";
  status.NagTable[32]="⟳";
  status.NagTable[33]="⟳";
  status.NagTable[36]="↑";
  status.NagTable[37]="↑";
  status.NagTable[40]="→";
  status.NagTable[41]="→";
  status.NagTable[44]="⯹";
  status.NagTable[45]="⯹";
  status.NagTable[132]="⇆";
  status.NagTable[133]="⇆";
  status.NagTable[138]="⨁";
  status.NagTable[139]="⨁";
  // Default Non-Standard NagTable: https://en.wikipedia.org/wiki/Numeric_Annotation_Glyphs
  status.NagTable[140]="∆";
  status.NagTable[141]="∇";
  status.NagTable[142]="⌓";
  status.NagTable[143]="<=";
  status.NagTable[144]="==";
  status.NagTable[145]="RR";
  status.NagTable[146]="N";
  status.NagTable[220]="⬒";
  status.NagTable[221]="⬓";
  status.NagTable[238]="○";
  status.NagTable[239]="⇔";
  status.NagTable[240]="⇗";
  status.NagTable[241]="⊞";
  status.NagTable[242]="⟫";
  status.NagTable[243]="⟪";
  status.NagTable[244]="✕";
  status.NagTable[245]="⊥";
  status.NagTable[254]="∟";
}

CGEditor::~CGEditor() {
  delete SBV;
  delete SBH;
  delete MT;
  delete MA;
  delete ME;
}

void CGEditor::Draw() {
  bool ShoudUpdateMouse = false;
  if (status.LeftClick || status.RightClick) {
    ShoudUpdateMouse = true;
  }

  // Should be refreshed before Scrollbar!
  // To update status.MoveTableMaxX and status.MoveTableMaxY
  MA->Refresh();
  MT->Refresh();
  MA->DrawMargin(MT->GetVariationsMarging());
  SBV->Refresh();
  SBH->Refresh();
  ME->Refresh();

  // Order matter
  DrawComponent(MA);
  DrawComponent(MT);
  DrawComponent(SBV);
  DrawComponent(SBH);
  DrawComponent(ME);

  // Update mouse events
  status.LeftClick = false;
  status.RightClick = false;
  status.IsDrag = false;
  if (ShoudUpdateMouse) {
    status.LastMouseClicX = status.MouseX;
    status.LastMouseClicY = status.MouseY;
  }
}

void CGEditor::CallDrawElement(Element e) {
  // For element that want to expands up to the edge
  if (e.width < 0) {
    if (e.ShouldApplyScroll) {
      e.width =
          status.CanvasWidth - status.ScrollbarWidth - (e.x + status.ScrollX);
    } else {
      e.width = status.CanvasWidth - status.ScrollbarWidth - e.x;
    }
    if (e.width < 0) {
      e.width *= -1;
    }
  }

  // Apply scroll
  if (e.ShouldApplyScroll) {
    e.x += status.ScrollX;
    if (!e.IgnoreScrollY) {
      e.y += status.ScrollY;
    }
  }

  // Check if element visible if not just leave
  if((e.x+e.width)<0 || status.CanvasWidth < e.x)
    return;
  if((e.y+e.height)<0 || status.CanvasHeight < e.y)
    return;
  
  // Check if mouse over
  if (e.IsOver(status.MouseX, status.MouseY)) {
    e.prop |= Property::Mouseover;
  }

  DrawElement(e);
}

void CGEditor::SyncCache(){
  MA->SyncCache();
  MT->SyncCache();
  MA->SyncCache();
  SBV->SyncCache();
  SBH->SyncCache();
  ME->SyncCache();
}

void CGEditor::DrawComponent(Component *c) {
  for (Element &e : c->GetElements()) {
    CallDrawElement(e);
  }
}

bool CGEditor::ProcessEvents(){
  bool processed=false;
  for(const cgeditor::Event &e: status.Events){
    HandleEvent(e);
    processed=true;
  }
  if(processed)
    status.Events.clear();
  return processed;
}

std::string CGEditor::GetNAGSymbol(const std::uint8_t id) const{
  for(auto const& pair: status.NagTable){
    if(pair.first == id)
      return pair.second;
  }
  return "";
}

std::uint8_t CGEditor::GetNAGId(const std::string& symbol) const{
  for(auto const& pair: status.NagTable){
    if(pair.second == symbol)
      return pair.first;
  }
  return 0;
}

} // namespace cgeditor
