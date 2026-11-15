/*
 * Machine dependent defines
 *
 * @(#)mach_dep.h	X.XX (Berkeley) XX/XX/XX
 */
/*
 * where scorefile should live
 */
#ifndef SCOREFILE
#define SCOREFILE	"/usr/games/lib/rogue_roll"
#endif

/*
 * Variables for checking to make sure the system isn't too loaded
 * for people to play.  MAXUSERS can be set to 0 to leave out the check.
 */

#ifndef MAXUSERS
#define	MAXUSERS	6	/* max number of users for this game */
#endif
#undef	MAXLOAD			/* 10 * max 15 minute load average */

#if MAXUSERS|MAXLOAD
#define	CHECKTIME	1	/* number of minutes between load checks */
				/* if not defined checks are only on startup */
#endif

#if MAXUSERS
#ifdef UCOUNT			/* defined if rogue should provide ucount() */
#define UTMP	"/etc/utmp"	/* where utmp file lives */
#endif
#endif
