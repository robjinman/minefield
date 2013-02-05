#include "ProgressBar.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// ProgressBar::ProgressBar
//===========================================
ProgressBar::ProgressBar(const Colour& col, const Vec2f& pos, const Vec2f& size) {
   m_gap = 0.005f;
   m_value = 1.f;

   m_size = size;

   m_shell = unique_ptr<Quad>(new Quad(Vec2f(0.f, 0.f), Vec2f(m_size.x, 0.f), Vec2f(m_size.x, m_size.y), Vec2f(0.f, m_size.y)));

   m_shell->setLineWidth(2);
   m_shell->setLineColour(col);
   m_shell->setFillColour(Colour(0.f, 0.f, 0.f, 0.f));
   m_shell->setRenderTransform(pos.x, pos.y, 8.f);

   m_bar = unique_ptr<Quad>(new Quad(Vec2f(m_gap, m_gap), Vec2f(m_size.x - m_gap, m_gap), Vec2f(m_size.x - m_gap, m_size.y - m_gap), Vec2f(m_gap, m_size.y - m_gap)));

   m_bar->setLineWidth(0);
   m_bar->setFillColour(col);
   m_bar->setRenderTransform(pos.x, pos.y, 9.f);
}

//===========================================
// ProgressBar::setAlpha
//===========================================
void ProgressBar::setAlpha(Dodge::float32_t a) {
   // TODO
}

//===========================================
// ProgressBar::increaseValue
//===========================================
void ProgressBar::increaseValue(Dodge::float32_t val) {
   setValue(m_value + val);
}

//===========================================
// ProgressBar::setValue
//===========================================
void ProgressBar::setValue(Dodge::float32_t val) {
   if (val < 0.f) val = 0.f;
   if (val > 1.f) val = 1.f;

   const Vec2f& A = m_bar->getVertex(0);
   const Vec2f& B = m_bar->getVertex(1);

   Vec2f C = B;
   C.y += val * (m_size.y - 2.f * m_gap);
   m_bar->setVertex(2, C);

   Vec2f D = A;
   D.y += val * (m_size.y - 2.f * m_gap);
   m_bar->setVertex(3, D);

   m_value = val;
}

//===========================================
// ProgressBar::draw
//===========================================
void ProgressBar::draw() const {
   m_shell->draw();
   m_bar->draw();
}
