#ifndef __PROGRESS_BAR_HPP__
#define __PROGRESS_BAR_HPP__


#include <memory>
#include <dodge/math/math.hpp>


class ProgressBar {
   public:
      ProgressBar(const Dodge::Colour& col, const Dodge::Vec2f& pos, const Dodge::Vec2f& size);
      void setAlpha(Dodge::float32_t a);
      void setValue(Dodge::float32_t val);
      void increaseValue(Dodge::float32_t val);
      void draw() const;

   private:
      Dodge::float32_t m_gap;
      Dodge::float32_t m_value;
      Dodge::Vec2f m_pos;
      Dodge::Vec2f m_size;
      std::unique_ptr<Dodge::Quad> m_shell;
      std::unique_ptr<Dodge::Quad> m_bar;
};


#endif
