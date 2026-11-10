#include "UnitFactory.h"
#include "MeleeUnits.h"
#include "RangedUnits.h"
#include "GlobalUnits.h"

std::unique_ptr<Unit> UnitFactory::create_unit(UnitType type, COORD spawn_pos)
{
    switch (type)
    {
    case UnitType::bruiser:   return std::make_unique<Bruiser>(spawn_pos);
    case UnitType::samurai:   return std::make_unique<Samurai>(spawn_pos);
    case UnitType::soldier:   return std::make_unique<Soldier>(spawn_pos);
    case UnitType::mage:      return std::make_unique<Mage>(spawn_pos);
    case UnitType::healer:    return std::make_unique<Healer>(spawn_pos);
    case UnitType::cannoneer: return std::make_unique<Cannoneer>(spawn_pos);
    default:                  return nullptr;
    }
}
