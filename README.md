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

