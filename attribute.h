//
// Created by Clay on 7/12/2022.
//

#ifndef ADVERSARIES_ATTRIBUTE_H
#define ADVERSARIES_ATTRIBUTE_H

enum attribute_t {
    attr_brawn,
    attr_agility,
    attr_intellect,
    attr_cunning,
    attr_willpower,
    attr_presence,
    attr_combat,
    attr_social,
    attr_general,
    attr_wound,
    attr_soak,
    attr_strain,
    attr_meleedefense,
    attr_rangeddefence,
    attr_MAX
};

const char* attribute_name(enum attribute_t attr);

#endif //ADVERSARIES_ATTRIBUTE_H
