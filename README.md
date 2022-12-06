# Genesys RPG Adversary Builder

Create, maintain, and update adversaries for the [Genesys RPG](https://www.fantasyflightgames.com/en/products/genesys/).  Adversaries are stored in a plain text file that might look like this:

    MINION "Gang Heavy" : "Tough Person", "Dodgy", "Criminal Tough"

The result would be a combination of the packages *Tough Person,* *Dodgy,* and *Criminal Tough* as defined in the [Genesys Expanded Player's Guide](https://www.drivethrurpg.com/product/298208/Genesys-Expanded-Players-Guide)

## The Formal Language

    world ::=.
    world ::= world skill.
    world ::= world talent.
    world ::= world package.
    world ::= world character.
    world ::= world weapon.
    skill ::= SKILL name LPAREN ATTRIBUTE RPAREN.
    talent ::= TALENT name.
    package ::= PACKAGE namedlist.
    character ::= MINION namedlist.
    character ::= RIVAL namedlist.
    character ::= NEMESIS namedlist.
    namedlist ::= name COLON.
    namedlist ::= namedlist namedlistitem.
    namedlist ::= namedlist COMMA namedlistitem.
    namedlistitem ::= leveleditem.
    namedlistitem ::= attributebonus.
    leveleditem ::= name.
    leveleditem ::= name NUMBER.
    attributebonus ::= ATTRIBUTE NUMBER.
    weapon ::= WEAPON name LPAREN name SEMICOLON DAMAGE NUMBER SEMICOLON CRITICAL NUMBER specials RPAREN.
    specials ::= specials COMMA leveleditem.
    specials ::= SEMICOLON leveleditem.
    specials ::=.
    name ::= QUOTEDSTRING.
    name ::= WORD.

## How To Use It

If you're using the Quick Adversary Generation System from the Genesys Advanced Player's Guide, adversaries are very quick to generate.  A single line using the shorthand of that system might look like this:

    use "quick.adv"
    use "modern.adv"

    minion "Jets Gang Member" : "Tough Person", "Hardy", "Brawler", "Brass Knuckles"

    rival "Jets Gang Lt" : "Tough Person", "Hardy", "Warrior Leader", Knife

I then process it with:

    adveraries westside-story.adv

I'll get output similar to:

    Jets Gang Member

    Br Ag Int Cun Will Pres    Combat: 0
    -- -- --- --- ---- ----    Social: -1
     3  2   2   2    2    1   General: 0

    Athletics PPP, Brawl PPa, Resilience PPP
    Brass Knuckles (Brawl; Dmg +1; Crit 3; Disorient 3)

    Jets Gang Lt

    Br Ag Int Cun Will Pres    Combat: 1
    -- -- --- --- ---- ----    Social: 1
     3  2   2   2    2    1   General: 2

    Athletics Paa, Discipline PP, Driving PPa, Leadership Paa, Melee PPPa, Riding PPa
    Knife (Melee; Dmg +1; Crit 3)

## Building Your Own NPCs

First, determine the setting(s) for your campaign.  The included setting files are:

* Fantasy (fantasy.adv)
* Modern (modern.adv)
* Science Fiction (sciencefiction.adv)
* Space Opera (spaceopera.adv)
* Steampunk (steampunk.adv)
* Weird War (weirdwar.adv)

There are also two other files:

* Core Rule (core.adv)
* Quick Adversary Creation (quick.adv)

All of the settings reference the core rules internally, so you won't need to use it directly.  The Quick Adversary Creation system from the Enhanced Player's Guide doesn't include any setting specific information, so if you use the Quick Adversary Creation system you'll want to use that plus an appropriate setting file for your game.

All of these files live in the `data` directory under the installation folder.

Use a setting by placing this at the top of your file:

    use "spaceopera.adv"

Create an NPC starting with `minion`, `rival`, or `nemesis`. Follow that with the NPC's name, in quotes, and a colon.  Follow that with a comma-separated list of skills, talents, attribute bonuses, and weapons.

#### On the Naming of Names

If a name is a single word, you can just write the word, and it will be recognized as a name.  Hyphenated words are considered a single word.  If the name contains more than one word, or punctuation marks, it must be enclosed in double quotes.

All name recognition is case insensitive, with the exception of file names for the settings files.  The case sensitivity is dependent on the underlying operating system.  For most windows users, this means the names are case insensitive.  Linux users will mostly be case sensitive.  OSX users will depend on how your system is configured.

### Talents

Talents in the setting files start with the keyword "talent" followed by the talent name in quotes.  Give an NPC a talent by writing the talent name in the list of NPC attributes.

### Attribute Bonuses

Attribute bonuses are identified by writing the name of the attribute, followed by a number.  If no sign is presented for the number, it is assumed to be added to the attribute.  If you preceed a number with a minus sign, it will be subtracted from the attribute.

### Skill

Skills appear in settings files with the word "skill" followed by a quoted name, and in parenthesis the attribute normally used with this skill.

Add a skill to your NPC by writing it's name in the list of attributes, followed by a number for the skill level.  If you do not provide a skill level this is effectively level 0, and it will not appear in the output.

#### Skill Specializations

Some skills, principally Melee and Ranged, can have "Heavy" and "Light" specializations in some settings.  The settings files follow the convention of the character sheets in writing this, so you get Melee-Light or Ranged-Heavy.  If you really preferred the convention of the rule books, you can create your own custom settings with the specialization in parenthesis, but it is ambiguous with the attribute specialization for a skill, and will look funny printed.

### Packages

The Quick Adversary System uses skill, attribute, and talent packages to assign a large number of properties to an NPC with a single name.  You can create your own packages for your campaign in the same way.

Give an NPC a package by writing it's name in the attribute list.

### Weapons

A weapon appears in a settings file with the word "weapon" followed by its properties.  Add a weapon to your NPC by adding its name to the attribute list.  You can also write the weapon definition inline, if for instance a rival or nemesis has a customized weapon that doesn't appear otherwise in your setting.