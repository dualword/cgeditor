#include "MyHalfMove.hpp"

MyHalfMove::MyHalfMove(std::string move) { this->move = move; }
MyHalfMove::~MyHalfMove() {}

void MyHalfMove::AddVariation(MyHalfMove *m) {
  m->IsBlack = this->IsBlack;
  m->Number = this->Number;
  MyHalfMove::variations.push_back(m);
  cgeditor::CGEHalfMove::variations.push_back(m);
  m->SetParent(this);
}

void MyHalfMove::SetMainline(MyHalfMove *m) {
  if (!this->IsBlack) {
    m->IsBlack = true;
    m->Number = this->Number;
  } else {
    m->IsBlack = false;
    m->Number = this->Number + 1;
  }
  MyHalfMove::mainline = m;
  cgeditor::CGEHalfMove::MainLine = m;
  if (m != NULL) {
    m->SetParent(this);
  }
}
void MyHalfMove::SetParent(MyHalfMove *m) {
  MyHalfMove::parent = m;
  CGEHalfMove::Parent = m;
}
void MyHalfMove::RemoveChild(MyHalfMove *m) {
  std::uint32_t i = 0;
  bool found = false;
  for (i; i < MyHalfMove::variations.size(); i++) {
    if (MyHalfMove::variations[i] == m) {
      found = true;
      break;
    }
  }
  if (found) {
    MyHalfMove::variations.erase(MyHalfMove::variations.begin() + i);
  }
  if (MyHalfMove::MainLine == m) {
    MyHalfMove::MainLine = NULL;
  }
  cgeditor::CGEHalfMove::RemoveChild((CGEHalfMove *)m);
}

MyHalfMove *MyHalfMove::GetParent() { return (parent); }

MyHalfMove *MyHalfMove::GetRoot() {
  MyHalfMove *m = this;
  MyHalfMove *p = MyHalfMove::parent;
  while (p != NULL) {
    if (p->mainline != m) {
      return (m);
    }
    m = p;
    p = m->MyHalfMove::parent;
  }
  return (m);
}

void MyHalfMove::SetAsMainline() {
  MyHalfMove *root = GetRoot();
  MyHalfMove *lastRoot;
  do {
    lastRoot = root;
    root->MyHalfMove::Promote();
    root = GetRoot();
  } while (root != lastRoot);

  // std::cout << IsVariation() << std::endl << std::flush;
}

void MyHalfMove::Promote() {
  if (MyHalfMove::parent != NULL) {
    MyHalfMove *p = MyHalfMove::parent;
    if (p->MyHalfMove::mainline != this) {
      if (MyHalfMove::parent->MyHalfMove::parent != NULL) {
        MyHalfMove *pp = MyHalfMove::parent->MyHalfMove::parent;
        if (pp->MyHalfMove::mainline == p) {
          pp->MyHalfMove::SetMainline(this);
        } else {
          pp->AddVariation(this);
          pp->MyHalfMove::RemoveChild(p);
        }
      }
      if (p->MyHalfMove::mainline == this) {
        p->MyHalfMove::SetMainline(NULL);
      } else {
        p->MyHalfMove::RemoveChild(this);
      }
      this->AddVariation(p);
    }
  }
}

bool MyHalfMove::IsVariation() {
  MyHalfMove *m = this;
  MyHalfMove *p = MyHalfMove::parent;
  while (p != NULL) {
    if (p->mainline != m) {
      return (true);
    }
    m = p;
    p = m->MyHalfMove::parent;
  }
  return (false);
}

MyHalfMove *BuildExampleGame() {
  MyHalfMove *m = new MyHalfMove("e4");
  MyHalfMove *toDisplay = m;

  MyHalfMove *m2 = new MyHalfMove("e5");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Nf3");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Nc6");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Bc4");
  m->SetMainline(m2);
  m->SetComment("Italian Opening");
  m = m2;

  m2 = new MyHalfMove("Bc5");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("c3");
  m2->SetComment("Giuoco Pianissimo");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Nf6");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("o-o");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("d6");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Nd2");
  m->SetMainline(m2);
  m = m2;

  {
    MyHalfMove *var = new MyHalfMove("Re1");
    var->SetComment("Also possible");
    m->AddVariation(var);

    MyHalfMove *var2 = new MyHalfMove("a6");
    var->SetMainline(var2);
    var = var2;

    {
      MyHalfMove *subvar = new MyHalfMove("Bg4");
      var->AddVariation(subvar);

      MyHalfMove *subvar2 = new MyHalfMove("Bb3");
      subvar->SetMainline(subvar2);
      subvar = subvar2;
    }

    var2 = new MyHalfMove("Bb3");
    var->SetMainline(var2);
    var = var2;

    var2 = new MyHalfMove("Ba7");
    var->SetMainline(var2);
    var = var2;
  }

  m2 = new MyHalfMove("a6");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Bb3");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Ba7");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Re1");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("o-o");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Nf1");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("h6");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Ng3");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Nd7");
  m->SetMainline(m2);
  m = m2;

  return (toDisplay);
}