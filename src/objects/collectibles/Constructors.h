#ifndef UI_FRAMEWORK_CONSTRUCTORS_H
#define UI_FRAMEWORK_CONSTRUCTORS_H

#include "objects/collectibles/EnergyBoost.h"
#include "objects/collectibles/Portalgun.h"
#include "objects/collectibles/StrengthModifier.h"
#include "objects/collectibles/Trowel.h"

static const CollectibleConstructor s_item_constructors[] = {
        Trowel::construct,
        Portalgun::construct,
        StrengthModifier::construct_upgrade,
        StrengthModifier::construct_downgrade,
        EnergyBoost::construct
};
static const size_t s_item_count = sizeof(s_item_constructors) / sizeof(CollectibleConstructor);

#endif //UI_FRAMEWORK_CONSTRUCTORS_H
