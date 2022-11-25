#include "timedwordleround.h"
#include "timedroundwindow.h"

TimedWordleRound::TimedWordleRound(QWidget *parent, const int len, bool hardMode):
    WordleRound(parent, len, hardMode)
{
    initGuesses(parent);

    /**
     * TODO: BONUS TASK 3
     *
     * Disconnect the invalidWord, invalidHardWord, roundWin, roundLose signals
     * from the RoundWindow popup slots, since we don't want popups during game.
     * Connect them to TimedRoundWindow's msg slots instead.
     * Also connect scoreUpdated() signal with scoreUpdate() slot.
     */

}

/**
 * TODO: BONUS TASK 3
 *
 * Same as base class endRound(), except:
 * - Don't change numTries and curGuess, since we are only ending the round for one word
 * - If round was won, add 1 to score and emit scoreUpdated
 * - Finally, call resetRound() to go to next word
 */
void TimedWordleRound::endRound(const bool win) {

}

/**
 * TODO: BONUS TASK 3
 *
 * Reset score to 0 (remember to emit signal) and resetRound() to go to a new word
 */
void TimedWordleRound::resetTimedRound() {

}

void TimedWordleRound::endTimedRound() {
    // Do this to stop user input
    curGuess = numGuesses;
}

/**
 * TODO: BONUS TASK 3
 *
 * resetRound() to go to a new word, unless the timed game is over
 */
void TimedWordleRound::skipCurrentWord() {

}
