/*
 * Function(s) for dealing with potions
 *
 * @(#)potions.c	X.XX (Berkeley) XX/XX/XX
 */

#include <curses.h>
#include "rogue.h"

/*
 * quaff:
 *	Quaff a potion from the pack
 */
quaff()
{
    register THING *obj, *th;

    obj = get_item("quaff", POTION);
    /*
     * Make certain that it is somethings that we want to drink
     */
    if (obj == NULL)
	return;
    if (obj->o_type != POTION)
    {
	if (!terse)
	    msg("yuk! Why would you want to drink that?");
	else
	    msg("that's undrinkable");
	return;
    }
    if (obj == cur_weapon)
	cur_weapon = NULL;

    /*
     * Calculate the effect it has on the poor guy.
     */
    switch (obj->o_which)
    {
	when P_CONFUSE:
	    if (!on(player, ISHUH))
	    {
		msg("wait, what's going on here. Huh? What? Who?");
		if (on(player, ISHUH))
		    lengthen(unconfuse, rnd(8)+HUHDURATION);
		else
		    fuse(unconfuse, 0, rnd(8)+HUHDURATION, AFTER);
		player.t_flags |= ISHUH;
	    }
	    p_know[P_CONFUSE] = TRUE;
	when P_POISON:
	    if (!ISWEARING(R_SUSTSTR))
	    {
		chg_str(-(rnd(3)+1));
		msg("you feel very sick now");
	    }
	    else
		msg("you feel momentarily sick");
	    p_know[P_POISON] = TRUE;
	when P_HEALING:
	    if ((pstats.s_hpt += roll(pstats.s_lvl, 4)) > max_hp)
		pstats.s_hpt = ++max_hp;
	    msg("you begin to feel better");
	    sight();
	    p_know[P_HEALING] = TRUE;
	when P_STRENGTH:
	    msg("you feel stronger, now.  What bulging muscles!");
	    chg_str(1);
	    p_know[P_STRENGTH] = TRUE;
	when P_MFIND:
	    player.t_flags |= SEEMONST;
	    fuse(turn_see, TRUE, HUHDURATION, AFTER);
	    if (mlist == NULL)
		msg("you have a strange feeling for a moment");
	    else
		p_know[P_MFIND] |= turn_see(FALSE);
	when P_TFIND:
	    /*
	     * Potion of magic detection.  Show the potions and scrolls
	     */
	    if (lvl_obj != NULL)
	    {
		register THING *tp;
		register bool show;

		show = FALSE;
		wclear(hw);
		for (tp = lvl_obj; tp != NULL; tp = next(tp))
		{
		    if (is_magic(tp))
		    {
			show = TRUE;
			mvwaddch(hw, tp->o_pos.y, tp->o_pos.x, MAGIC);
			p_know[P_TFIND] = TRUE;
		    }
		}
		for (th = mlist; th != NULL; th = next(th))
		{
		    for (tp = th->t_pack; tp != NULL; tp = next(tp))
		    {
			if (is_magic(tp))
			{
			    show = TRUE;
			    mvwaddch(hw, th->t_pos.y, th->t_pos.x, MAGIC);
			    p_know[P_TFIND] = TRUE;
			}
		    }
		}
		if (show)
		{
		    show_win(hw,
			"You sense the presence of magic on this level.--More--");
		    break;
		}
	    }
	    msg("you have a strange feeling for a moment, then it passes");
	when P_PARALYZE:
	    msg("you can't move");
	    no_command = HOLDTIME;
	    player.t_flags &= ~ISRUN;
	    p_know[P_PARALYZE] = TRUE;
	when P_SEEINVIS:
	    msg("this potion tastes like %s juice", fruit);
	    if (!on(player, CANSEE))
	    {
		fuse(unsee, 0, SEEDURATION, AFTER);
		look(FALSE);
		invis_on();
	    }
	    sight();
	when P_RAISE:
	    msg("you suddenly feel much more skillful");
	    p_know[P_RAISE] = TRUE;
	    raise_level();
	when P_XHEAL:
	    if ((pstats.s_hpt += roll(pstats.s_lvl, 8)) > max_hp)
	    {
		if (pstats.s_hpt > max_hp + pstats.s_lvl + 1)
		    ++max_hp;
		pstats.s_hpt = ++max_hp;
	    }
	    msg("you begin to feel much better");
	    p_know[P_XHEAL] = TRUE;
	    sight();
	when P_HASTE:
	    add_haste(TRUE);
	    msg("you feel yourself moving much faster");
	    p_know[P_HASTE] = TRUE;
	when P_RESTORE:
	    msg("hey, this tastes great.  It make you feel warm all over");
	    if (ISRING(LEFT, R_ADDSTR))
		add_str(&pstats.s_str, -cur_ring[LEFT]->o_ac);
	    if (ISRING(RIGHT, R_ADDSTR))
		add_str(&pstats.s_str, -cur_ring[RIGHT]->o_ac);
	    if (pstats.s_str < max_stats.s_str)
		pstats.s_str = max_stats.s_str;
	    if (ISRING(LEFT, R_ADDSTR))
		add_str(&pstats.s_str, cur_ring[LEFT]->o_ac);
	    if (ISRING(RIGHT, R_ADDSTR))
		add_str(&pstats.s_str, cur_ring[RIGHT]->o_ac);
	when P_BLIND:
	    msg("a cloak of darkness falls around you");
	    if (!on(player, ISBLIND))
	    {
		player.t_flags |= ISBLIND;
		fuse(sight, 0, SEEDURATION, AFTER);
		look(FALSE);
	    }
	    p_know[P_BLIND] = TRUE;
	when P_NOP:
	    msg("this potion tastes extremely dull");
	otherwise:
	    msg("what an odd tasting potion!");
	    return;
    }
    status();
    /*
     * Throw the item away
     */
    call_it(p_know[obj->o_which], &p_guess[obj->o_which]);

    inpack--;
    if (obj->o_count > 1)
	obj->o_count--;
    else
    {
	detach(pack, obj);
	discard(obj);
    }
}

/*
 * invis_on:
 *	Turn on the ability to see invisible
 */
invis_on()
{
    register THING *th;

    player.t_flags |= CANSEE;
    for (th = mlist; th != NULL; th = next(th))
	if (on(*th, ISINVIS) && see_monst(th))
	{
	    wmove(cw, th->t_pos.y, th->t_pos.x);
	    waddch(cw, th->t_disguise);
	}
}

/*
 * turn_see:
 *	Put on or off seeing monsters on this level
 */
turn_see(turn_off)
register bool turn_off;
{
    register THING *mp;
    register bool can_see, add_new;

    add_new = FALSE;
    for (mp = mlist; mp != NULL; mp = next(mp))
    {
	wmove(cw, mp->t_pos.y, mp->t_pos.x);
	can_see = see_monst(mp) || winch(cw) == mp->t_type;
	if (turn_off)
	{
	    if (!can_see)
		waddch(cw, mp->t_oldch);
	}
	else
	{
	    if (!can_see)
		wstandout(cw);
	    waddch(cw, mp->t_type);
	    if (!can_see)
	    {
		wstandend(cw);
		add_new++;
	    }
	}
    }
    if (turn_off)
	player.t_flags &= ~SEEMONST;
    else
	player.t_flags |= SEEMONST;
    return add_new;
}
