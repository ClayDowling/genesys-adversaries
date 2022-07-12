//
// Created by Clay on 7/12/2022.
//
#include "attribute.h"

const char *attribute_name(enum attribute_t attr) {
    switch (attr) {
        case attr_brawn:
            return "brawn";
        case attr_agility:
            return "agility";
        case attr_intellect:
            return "intellect";
        case attr_cunning:
            return "cunning";
        case attr_willpower:
            return "willpower";
        case attr_presence:
            return "presence";
        case attr_combat:
            return "combat";
        case attr_social:
            return "social";
        case attr_general:
            return "general";
        case attr_wound:
            return "wound";
        case attr_soak:
            return "soak";
        case attr_strain:
            return "strain";
        case attr_meleedefense:
            return "meleedefense";
        case attr_rangeddefence:
            return "rangeddefence";
        case attr_MAX:
            return "MAX";
    }
    return "UNKNOWN";
}
