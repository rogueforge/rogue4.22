/*
 * This file contains misc functions for dealing with armor
 *
 * @(#)armor.c	X.XX (Berkeley) XX/XX/XX
 */

#include <curses.h>
#include "rogue.h"

/*
 * wear:
 *	The player wants to wear something, so let him/her put it on.
 */
wear()
{
    register THING *obj;

    if (cur_armor != NULL)
    {
	addmsg("you are already wearing some");
	if (!terse)
	    addmsg(".  You'll have to take it off first");
	endmsg();
	after = FALSE;
	return;
    }
    if ((obj = get_item("wear", ARMOR)) == NULL)
	return;
    if (obj->o_type != ARMOR)
    {
	msg("you can't wear that");
	return;
    }
    waste_time();
    obj->o_flags |= ISKNOW;
    if (!terse)
	addmsg("you are now ");
    msg("wearing %s", inv_name(obj, TRUE));
    cur_armor = obj;
}

/*
 * take_off:
 *	Get the armor off of the players back
 */
take_off()
{
    register THING *obj;

    if ((obj = cur_armor) == NULL)
    {
	if (terse)
		msg("not wearing armor");
	else
		msg("you aren't wearing any armor");
	after = FALSE;
	return;
    }
    if (!dropcheck(cur_armor))
	return;
    cur_armor = NULL;
    if (terse)
	addmsg("was");
    else
	addmsg("you used to be ");
    msg(" wearing %c) %s", pack_char(obj), inv_name(obj, TRUE));
}

/*
 * waste_time:
 *	Do nothing but let other things happen
 */
waste_time()
{
    do_daemons(BEFORE);
    do_fuses(BEFORE);
    do_daemons(AFTER);
    do_fuses(AFTER);
}
