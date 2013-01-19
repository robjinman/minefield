#ifndef __C_COUNTER_HPP__
#define __C_COUNTER_HPP__


#include "CTextEntity.hpp"


class Counter : public CTextEntity {
   public:
      Counter(const Dodge::XmlNode data);
      Counter(const Counter& copy);
      Counter(const Counter& copy, long name);

      void setValue(int value);
      inline int getValue() const;
      inline void add(int i);
      inline void subtract(int i);
      inline void increment();
      inline void decrement();

      virtual void addToWorld();

      virtual size_t getSize() const;
      virtual Counter* clone() const;

      virtual void assignData(const Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~Counter() {}

   private:
      int m_value;
};

typedef boost::shared_ptr<Counter> pCounter_t;

//===========================================
// Counter::getValue
//===========================================
inline int Counter::getValue() const {
   return m_value;
}

//===========================================
// Counter::increment
//===========================================
inline void Counter::increment() {
   add(1);
}

//===========================================
// Counter::decrement
//===========================================
inline void Counter::decrement() {
   subtract(1);
}

//===========================================
// Counter::add
//===========================================
inline void Counter::add(int i) {
   setValue(getValue() + i);
}

//===========================================
// Counter::subtract
//===========================================
inline void Counter::subtract(int i) {
   setValue(getValue() - i);
}


#endif
