#include "MyHalfMove.hpp"

MyHalfMove::MyHalfMove(std::string move){SetSAN(move); }

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

  m2 = new MyHalfMove("O-O");
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
  m->SetComment("Test for a very long comment, to see how line breaks are handle by the framework.");
  m->SetComment(m->GetComment()+"Test for a very long comment, to see how line breaks are handle by the framework.");
  m = m2;

  m2 = new MyHalfMove("Bb3");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Ba7");
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("Re1");
  m2->SetNAG(3);
  m->SetMainline(m2);
  m = m2;

  m2 = new MyHalfMove("O-O");
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