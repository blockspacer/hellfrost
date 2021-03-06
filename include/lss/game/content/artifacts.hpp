#ifndef __ARTEFACTS_H_
#define __ARTEFACTS_H_

#include "lss/game/content/spells.hpp"
#include "lss/game/effect.hpp"
#include "lss/game/item.hpp"

namespace Prototype {
const auto BAND_OF_DARKVISION =
    std::make_shared<Item>("thin band", "band of darkvision", "HELMET",
                           Effects{std::make_shared<VisibilityModifier>(5)});

const auto ROBE_OF_INTELLIGENCE = std::make_shared<Item>(
    "blue robe", "robe of intelligence", "CLOAK",
    Effects{std::make_shared<IntelligenceModifier>(0.1)});

const auto AMULET_OF_BERSERK = std::make_shared<Item>(
    "ruby neckless", "amulet of berserk", "AMULET",
    Effects{std::make_shared<TraitEffect>(Traits::BLOOD_THIRST)});

const auto LEATHER_CUIRASS_OF_HP = std::make_shared<Item>(
    "damaged leather cuirass", "leather cuirass of health", "CUIRASS",
    Effects{std::make_shared<ArmorValue>(R::I(1, 2)),
            std::make_shared<HPModifier>(25)});

const auto LEATHER_LEFT_GAUNTLET_OF_STRENGTH =
    std::make_shared<Item>("worn leather left gauntlet",
                           "left gauntlet of strength", "LEFT_GAUNTLET",
                           Effects{std::make_shared<ArmorValue>(R::I(1, 2)),
                                   std::make_shared<StrengthModifier>(0.1)});

const auto LEFT_GAUNTLET_OF_DUEL = std::make_shared<Item>(
    "black left gauntlet", "duelist's left gauntlet", "LEFT_GAUNTLET",
    Effects{
        std::make_shared<TraitEffect>(Traits::DUAL_WIELD),
        std::make_shared<CritModifier>(0.2),
    });

const auto BOOTS_OF_FLY =
    std::make_shared<Item>("flying boots", "BOOTS",
                           Effects{std::make_shared<TraitEffect>(Traits::FLY)});

const auto SPEED_RING =
    std::make_shared<Item>("gold ring", "ring of lightning", "RING",
                           Effects{std::make_shared<SpeedModifier>(0.1)});

const auto SWORD_OF_DUALWIELD = std::make_shared<Item>(
    "sword of dualwield", "ONE_HAND",
    Effects{std::make_shared<MeleeDamage>(DamageSpec(
                R::I(-2, 2), R::I(2, 4), R::I(2, 4), DamageType::WEAPON)),
            std::make_shared<TraitEffect>(Traits::DUAL_WIELD)});

const auto POISON_DAGGER = std::make_shared<Item>(
    "LIGHT_WEAPON",
    Effects{
        std::make_shared<MeleeDamage>(DamageSpec(2, 3, 3, DamageType::WEAPON)),
        std::make_shared<CritModifier>(0.2),
        std::make_shared<OnHitEffect>(
            std::make_shared<LastingEffect>(OverTimeEffects::POISON, 5000),
            0.3)});

/* Immunes */
const auto AMULET_OF_FIRE_IMMUNE = std::make_shared<Item>(
    "gold neckless", "amulet of fire immune", "AMULET",
    Effects{std::make_shared<TraitEffect>(Traits::FIRE_IMMUNE)});

const auto AMULET_OF_ACID_IMMUNE = std::make_shared<Item>(
    "old copper neckless", "amulet of acid immune", "AMULET",
    Effects{std::make_shared<TraitEffect>(Traits::ACID_IMMUNE)});

const auto AMULET_OF_FROST_IMMUNE = std::make_shared<Item>(
    "shining silver neckless", "amulet of frost immune", "AMULET",
    Effects{std::make_shared<TraitEffect>(Traits::FROST_IMMUNE)});

/* Resists */

const auto RING_OF_ACID_RESIST = std::make_shared<Item>(
    "old copper ring", "ring of acid resist", "RING",
    Effects{std::make_shared<TraitEffect>(Traits::ACID_RESIST)});

const auto RING_OF_FIRE_RESIST = std::make_shared<Item>(
    "red copper ring", "ring of fire resist", "RING",
    Effects{std::make_shared<TraitEffect>(Traits::FIRE_RESIST)});

const auto RING_OF_FROST_RESIST = std::make_shared<Item>(
    "silver ring", "ring of frost resist", "RING",
    Effects{std::make_shared<TraitEffect>(Traits::FROST_RESIST)});

/* EoT */

const auto RING_OF_REGEN =
    std::make_shared<Item>("emerald ring", "ring of regeneration",
                           "RING", Effects{OverTimeEffects::REGEN});

const auto RING_OF_MANA_RESTORE = std::make_shared<Item>(
    "sapfire ring", "ring of mana restore", "RING",
    Effects{OverTimeEffects::MANA_RESTORE_LESSER});
} // namespace Prototype

#endif // __ARTEFACTS_H_
