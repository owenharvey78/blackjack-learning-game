#ifndef RULESET_H
#define RULESET_H

struct Ruleset {
    // Dealer Rules
    bool dealerHitsSoft17 = true;     // true = dealer hits on soft 17.
    bool dealerPeeks = true;          // true = dealer peeks when first card is ace.
    bool pushOnDealer22 = false;      // true = Free bet style.

    // Payouts/Decks
    double blackjackPayout = 1.5;     // determines payout when player hits a blackjack.
    int numDecks = 6;                 // 6 or 8 is standard. define valid range 1 - 10.

    // Restrictions
    bool doubleAfterSplit = true;     // can you double on a hand that was just split?
    bool resplit = true;              // Can you split non-aces after splitting?
    bool hitSplitAces = false;        // Can you hit after splitting Aces? Default is player gets one more card
    bool resplitAces = false;         // can you split if you split aces and get another ace? (Only applicable if hitSplitAces is true)
    bool surrenderAllowed = true;     // Can you give up half your bet after dealer checks for blackjack? Lose the hand and half your bet

    // Add section for player assistances and hints in practice mode
};

#endif // RULESET_H
