#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int compareMoves(int a_move, int p_move);

int main() {
    int p_score = 0;
    int a_score = 0;
    int rounds, aMove, result;
    char pMove[64];

    printf("Welcome to Rock, Paper, Scissors!\n");
    printf("How many rounds do you want to play?\n");
    scanf(" %d", &rounds);

    for (int i = 0; i < rounds; i++) {
        srand(time(0));
        printf("Which do you choose? rock, paper, or scissors? ");
        scanf("%s", pMove);
        if ((strcmp(pMove, "rock") != 0) && (strcmp(pMove, "paper") != 0) && (strcmp(pMove, "scissors") != 0)) {
            printf("You entered an invalid choice: %s\n", pMove);
            printf("AI score: %d Player score: %d\n", a_score, p_score);
        }
        int pRock = strcmp(pMove, "rock");
        int pPaper  = strcmp(pMove, "paper");
        int pScissors = strcmp(pMove, "scissors");

        // AI move
        aMove = rand() % 3; 
        if (aMove == 0) {
            printf("AI chose rock\n");
        }
        else if (aMove == 1) {
            printf("AI chose paper\n");
        }
        else {
            printf("AI chose scissors\n");
        }

        // result print statement
        if ((pRock == 0 && aMove == 1) || (pPaper == 0 && aMove == 0)) {
            printf("Paper covers rock\n");
        }
        else if ((pPaper == 0 && aMove == 2) || (pScissors == 0 && aMove == 1)) {
            printf("Scissors cut paper\n");
        }
        else if ((pScissors == 0 && aMove == 0) || (pPaper == 0 && aMove == 2)) {
            printf("Rock bashes scissors\n");
        }

        // compare moves
        if (pRock == 0) {
            result = compareMoves(aMove, 0);
        } else if (pPaper == 0) {
            result = compareMoves(aMove, 1);
        } else {
            result = compareMoves(aMove, 2);
        }

        // compute AI and Player scores
        if (result == 1) {
            p_score++;
        } 
        else if (result == -1) {
            a_score++;
        }
        printf("AI score: %d Player score: %d\n", a_score, p_score);
    }
    //compute winner
    if (p_score > a_score) {
        printf("Player wins!\n");
    } 
    else if (p_score < a_score) {
        printf("AI wins!\n");
    } 
    else {
        printf("It's a tie!\n");
    }
    return 0;
}

// rock = 0; paper = 1; scissors = 2
// 0 = ties; 1 = player win; -1 = ai win
int compareMoves(int a_move, int p_move) {
    if (p_move == a_move) { // same move
        return 0;
    }
    else if (p_move + a_move == 1) { // rock + paper
        if (p_move == 0) {
            return -1;
        } else {
            return 1;
        }
    }
    else if (p_move + a_move == 2) { // rock + scissors
        if (p_move == 0) {
            return 1;
        } else {
            return -1;
        }
    }
    else {
        if (p_move == 1) { // paper + scissors
            return -1;
        } else {
            return 1;
        }
    }
}
