#include "tree_inlines.h"

/*
 * This file is included in multiple places in our new fast hand evaluator.
 * Each time it is included, certain hand identification predicate macros
 * are defined from outside this file, and then others are defined from
 * within this file, and then, using all the predicates, a decision tree is
 * walked until we get to a terminal node, at which point we return a complete
 * hand identifying value (i.e. a 32 bit number whose most significant bits
 * can be used to identify which type of hand you have (e.g. full-house), and
 * whose remaining bits can tell you the composition of the hand (e.g. sevens
 * and fives).
 *
 * In the interests of speed, we actually have three different types of
 * predicates.  The simplest either returns 0 or non-zero based on whether
 * or not the current hand is of a given type, given all the information
 * that we know by the time the predicate is called.  NOTE:  it is generally
 * NOT good computer science to have predicates that are postion dependent
 * (e.g. you can't use the TWO_PAIR_P predicate just anywhere in the below
 * code -- it can only be used after we've determined that we don't have
 * three of a kind).  However, it is faster to do things this way, and once
 * you fully understand what we're doing below, you can see that our code is
 * correct.  Simple predicates have names of the form XXX_P, where XXX is
 * the hand class in capital letters.  Simple predicates are ALWAYS defined
 * outside of this file (i.e. all the information that they need is available
 * to us at compile time).
 *
 * This file contains four types of things:
 *
 *	Complete predicate macros definitions that define macros which detect
 *	particular classes of hands and either return 0 if such a hand isn't
 *	detected, or a complete hand identifying value.  Complete predicate
 *	macros have names of the form XXX_complete_P, where XXX is the hand
 *	class in capital letters.
 *
 *	Helper predicate macro definitions that define macros which detect
 *	particular classes of hands and either return 0 if such a hand isn't
 *	detected, or a useful number which will be used later as the
 *	evaluation proceeds.  Helper predicate macros have names of the form
 *	XXX_helper_P, where XXX is the hand class in capital letters.
 *
 *	Helper macro definitions that aren't predicates, but that provide
 *	information useful to the tree walk.  Helper macros have names of
 *	the form YYY_helper, where YYY helps describe the data that the
 *	particular helper macro returns.
 *
 *	The decision tree code that uses all three classes of predicates to
 *	walk to the identifying node and then return the proper hand
 *	identifying value.  At different places, where this file is included
 *	some of the predicates that we #define below will be overridden with
 *	an outside definition of 0, because at compile time we can tell that
 *	certain hands do not need to be checked for particular hand types.
 *
 * The decision tree walking code frequently makes use of inline-functions
 * which map some intermediate information into coplete hand identifying
 * values.  These functions are defined elsewhere and have names of the
 * form lower_case_hand_class_value.
 *	
 */

/*
 * FLUSH_helper_P returns 0 if no flush, or the top 5 ranks in the flush suit
 *	if a flush is found.  We don't yet or in FLUSH_VALUE, since that is
 *	premature.  We return what we do because it is all the information
 *	that is needed to continue checking for other things (like a straight
 *	flush).
 */

#if	!defined(FLUSH_helper_P)

#define	FLUSH_helper_P()						\
    (top_five_cards[c] | top_five_cards[d] top_five_cards[h] |		\
						top_five_cards[s])

#endif

/*
 * STRAIGHT_FLUSH_helper_P returns 0 or a complete hand rank, but with the
 *	VALUE set as a straight value.  A straight flush is sufficiently rare
 *	that when we actually find one, we can clean up the VALUE by xoring in
 *	xor of the *	correct VALUE (i.e. STRAIGHT_FLUSH_VALUE) with the
 *	incorrect value (i.e. STRAIGHT_VALUE).  Since a ^ (a^b) is b, this
 *	does the right thing and we don't need a separate table, which might
 *	slow us down by interfering with the data cache.
 */

#if	!defined(STRAIGHT_FLUSH_helper_P)

#define	STRAIGHT_FLUSH_helper_P(suit)					\
    straight_value[suit]

#endif

#if	!defined(FOUR_OF_A_KIND_complete_P)

#define FOUR_OF_A_KIND_complete_P()					\
({									\
    uint32 retval;							\
									\
    retval = c & d & h & s;						\
    if (retval) {							\
	retval = FOUR_OF_A_KIND_VALUE | (retval << N_RANK) |		\
					top_card_table[ranks ^ retval];	\
    }									\
    retval;								\
})

#endif

/*
 * THREE_OF_A_KIND returns all the ranks that have at least three distinct
 *	members.  It is not sufficient to just return the top one, because
 *	of the splenderiferous implementation of FULL_HOUSE below.
 */

#if	!defined(THREE_OF_A_KIND_helper_P)

#define THREE_OF_A_KIND_helper_P() 					\
     ((( c&d )|( h&s )) & (( c&h )|( d&s )))

#endif

/*
 * Watch closely:  FULL_HOUSE_complete_P will only be examined after we know
 *	that we do not have four of a kind.  So if we xor all four
 *	suits together we are left with ones every place where we
 *	either have one or three members of a particular rank.  If
 *	we invert this and then and it with ranks, we now only have
 *	ones where we have exactly two members of a particular
 *	rank.  However, this is not enough, because it is possible
 *	for a full-house to consist of two three-of-a-kinds, so we
 *	have to or in three_info, which contains all of our
 *	three-of-a-kinds.  Then we need to mask off the top rank
 *	to see if we still have a pair or three-of-a-kind left
 *	over.
 */

#if	!defined(FULL_HOUSE_complete_P)

#define FULL_HOUSE_complete_P(three_info)				\
({									\
    uint32 retval;							\
									\
    retval = (~(c^d^h^s) & ranks)|three_info;				\
    top_bit = top_rank[three_info];					\
    retval ^= top_bit;							\
    if (retval)								\
	retval = FULL_HOUSE_VALUE | (top_bit << N_RANK) |		\
						top_rank[retval];	\
    return retval;							\
})

#endif

#define	STRAIGHT_FLUSH_XOR_CORRECTION_VALUE	(STRAIGHT_VALUE ^	\
						STRAIGHT_FLUSH_VALUE)

    if (STRAIGHT_P()) {
	if ((flush_suit = FLUSH_helper_P())) {
	    if ((retval = STRAIGHT_FLUSH_helper_P(flush_suit))) {
		/* straight flush */
		return retval ^ STRAIGHT_FLUSH_XOR_CORRECTION_VALUE;
	    } else {
		if ((retval = FOUR_OF_A_KIND_complete_P())) {
		    return retval;
		    /* four of a kind */
		} else {
		    if ((three_info = THREE_OF_A_KIND_helper_P()) &&
			       (retval = FULL_HOUSE_complete_P(three_info))) {
			/* full house */
			return retval;
		    } else {
			/* flush */
			return flush_value(flush_suit);
		    }
		}
	    }
	} else {
	    if ((retval = FOUR_OF_A_KIND_complete_P())) {
		return retval;
		/* four of a kind */
	    } else {
		if ((three_info = THREE_OF_A_KIND_helper_P()) &&
			       (retval = FULL_HOUSE_complete_P(three_info))) {
		    /* full house */
		    return retval;
		} else {
		    /* straight */
		    return straight_value(ranks);
		}
	    }
	}
    } else {
	if (PAIR()) {
	    if ((three_info = THREE_OF_A_KIND_helper_P())) {
		if ((retval = FOUR_OF_A_KIND_complete_P())) {
		    return retval;
		    /* four of a kind */
		} else {
		    if ((retval = FULL_HOUSE_complete_P(three_info))) {
			/* full house */
			return retval;
		    } else {
			/* three of a kind */
			return three_of_a_kind_value(ranks, three_info);
		    }
		}
	    } else {
		all_pairs = ALL_PAIRS_helper();
		if (TWO_PAIR_P()) {
		    /* two pair */
		    return two_pair_value(ranks, all_pairs);
		} else {
		    return pair_value(ranks, all_pairs);
		    /* pair */
		}
	    }
	} else {
	    /* high hand */
	    return high_hand_value(ranks);
	}
    }
