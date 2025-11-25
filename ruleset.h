#ifndef RULESET_H
#define RULESET_H

#endif // RULESET_H

struct Ruleset {
    // Dealer Rules
    bool dealerHitsSoft17 = true;     // true = dealer hits on soft 17.
    bool dealerPeeks = true;          // true = dealer peeks when first card is ace.
    bool pushOnDealer22 = false;      // true = Free bet style.
    bool fiveCardCharlie = false;     // true = 5 cards is instant win for player.

    // Payouts/Decks
    double blackjackPayout = 1.5;     // determines payout when player hits a blackjack.
    int numDecks = 1;                 // default to 1 for simplicity.

    // Restrictions
    bool doubleAfterSplit = true;     // can you double on a hand that was just split?
    bool resplitAces = false;         // can you split if you split aces and get another ace?
    bool hitSplitAces = false;        // Can you hit after splitting Aces? Default is player gets one more card
    bool surrenderAllowed = true;     // Can you give up half your bet after dealer checks for blackjack? Lose the hand and half your bet

    // Add section for player assistances and hints in practice mode
};
