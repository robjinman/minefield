#ifndef __C_TEXT_ENTITY_HPP__
#define __C_TEXT_ENTITY_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/TextEntity.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class CTextEntity : public Item, public Dodge::TextEntity {
   public:
      CTextEntity(const Dodge::XmlNode data);
      CTextEntity(const CTextEntity& copy);
      CTextEntity(const CTextEntity& copy, long name);

      virtual void draw() const;

      virtual void onEvent(const Dodge::EEvent* event) { TextEntity::onEvent(event); }
      virtual void update();

      virtual size_t getSize() const;
      virtual CTextEntity* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(const Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~CTextEntity() {}
};

typedef boost::shared_ptr<CTextEntity> pCTextEntity_t;


#endif
