#ifndef __WEAPONS_H_
#define __WEAPONS_H_

#include "lss/game/content/spells.hpp"
#include "lss/game/item.hpp"

namespace Prototype {
const auto SWORD = std::make_shared<Item>(
    "iron sword", "ONE_HAND",
    Effects{std::make_shared<MeleeDamage>(
        DamageSpec(R::I(-2, 2), R::I(2, 4), R::I(2, 4), DamageType::WEAPON))});

const auto AXE = std::make_shared<Item>(
    "iron axe", "ONE_HAND",
    Effects{std::make_shared<MeleeDamage>(
        DamageSpec(R::I(-2, 0), R::I(2, 4), R::I(2, 6), DamageType::WEAPON))});

const auto STEEL_SWORD = std::make_shared<Item>(
    "steel sword", "ONE_HAND",
    Effects{std::make_shared<MeleeDamage>(
        DamageSpec(R::I(-2, 2), R::I(3, 6), R::I(3, 6), DamageType::WEAPON))});

const auto STEEL_AXE = std::make_shared<Item>(
    "steel axe", "ONE_HAND",
    Effects{std::make_shared<MeleeDamage>(
        DamageSpec(R::I(-2, 0), R::I(3, 5), R::I(2, 8), DamageType::WEAPON))});

const auto RAPIER = std::make_shared<Item>(
    "rapier", "ONE_HAND",
    Effects{std::make_shared<CritModifier>(0.2),
            std::make_shared<MeleeDamage>(DamageSpec(
                R::I(1, 3), R::I(1, 2), R::I(2, 3), DamageType::WEAPON))});

const auto GREAT_AXE = std::make_shared<Item>(
    "great axe", "TWO_HAND",
    Effects{std::make_shared<MeleeDamage>(
                DamageSpec(R::I(-2, 0), 2, R::I(5, 7), DamageType::WEAPON)),
            std::make_shared<SpeedModifier>(-0.4f)});

const auto GREAT_SWORD =
    std::make_shared<Item>("great sword", "TWO_HAND",
                           Effects{std::make_shared<MeleeDamage>(DamageSpec(
                                       0, 2, R::I(4, 6), DamageType::WEAPON)),
                                   std::make_shared<SpeedModifier>(-0.2f)});

const auto DAGGER = std::make_shared<Item>(
    "dagger", "LIGHT_WEAPON",
    Effects{std::make_shared<MeleeDamage>(
        DamageSpec(R::I(-2, 2), 1, R::I(1, 3), DamageType::WEAPON))});

const auto THROWING_KNIVES =
    std::make_shared<Item>("THROWING_KNIVES", 1,
                           Effects{std::make_shared<MeleeDamage>(
                               DamageSpec(0, 1, 6, DamageType::WEAPON))});

const auto THROWING_AXES =
    std::make_shared<Item>("throwing axes", "THROWING_KNIVES", 1,
                           Effects{std::make_shared<MeleeDamage>(
                               DamageSpec(0, 2, 6, DamageType::WEAPON))});

const auto ORK_SWORD = std::make_shared<Item>(
    "ONE_HAND", Effects{std::make_shared<MeleeDamage>(
                                    DamageSpec(5, 4, 6, DamageType::WEAPON)),
                                std::make_shared<CritModifier>(0.4)});
const auto GOBLIN_DAGGER =
    std::make_shared<Item>("LIGHT_WEAPON",
                           Effects{std::make_shared<MeleeDamage>(
                                       DamageSpec(2, 3, 3, DamageType::WEAPON)),
                                   std::make_shared<CritModifier>(0.2)});
const auto WRAITH_TOUCH = std::make_shared<Item>(
    "ENEMY",
    Effects{
        std::make_shared<MeleeDamage>(DamageSpec(2, 3, 6, DamageType::MAGIC)),
        std::make_shared<CritModifier>(0.2),
        std::make_shared<OnHitEffect>(
            std::make_shared<LastingEffect>(
                std::make_shared<TraitEffect>(Traits::CONFUSED), 50000),
            0.5)});

const auto POISON_FANG = std::make_shared<Item>(
    "ENEMY",
    Effects{
        std::make_shared<MeleeDamage>(DamageSpec(1, 2, 3, DamageType::WEAPON)),
        std::make_shared<CritModifier>(0.2),
        std::make_shared<OnHitEffect>(
            std::make_shared<LastingEffect>(OverTimeEffects::POISON, 5000),
            0.3)});
} // namespace Prototype

#endif // __WEAPONS_H_
