# Genesys RPG Adversary Builder

Create, maintain, and update adversaries for the [Genesys RPG](https://www.fantasyflightgames.com/en/products/genesys/).  Adversaries are stored in a plain text file that might look like this:

    MINION Gang Heavy : Tough Person, Dodgy, Criminal Tough

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
