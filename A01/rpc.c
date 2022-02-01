#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROCK 1
#define PAPER 2
#define SCISSORS 3

int match(int *player_move_o, int *ai_move_o, int *player_score, int *ai_score)
{
    int player_move = *player_move_o;
    int ai_move = *ai_move_o;
    int player_score_l = *player_score;
    int ai_score_l = *ai_score;

    if (player_move != 1 && player_move != 2 && player_move != 3)
    {
        printf("You entered an invalid choice, please try again.\n");
        printf("AI score: %d, player score: %d\n", ai_score_l, player_score_l);
            *player_score = player_score_l;
            *ai_score = ai_score_l;
    }
    else
    {
        if (player_move == ROCK && ai_move == SCISSORS)
        {
            player_score_l++;
            printf("Rock bashes scissors. You win!\n");
            printf("AI score: %d, player score: %d\n", ai_score_l, player_score_l);
            *player_score = player_score_l;
            *ai_score = ai_score_l;
        }
        else if (player_move == ROCK && ai_move == PAPER)
        {
            ai_score_l++;
            printf("Paper covers rock. You lose!\n");
            printf("AI score: %d, player score: %d\n", ai_score_l, player_score_l);
            *player_score = player_score_l;
            *ai_score = ai_score_l;
        }
        else if (player_move == SCISSORS && ai_move == PAPER)
        {
            player_score_l++;
            printf("Scissors cuts paper. You win!\n");
            printf("AI score: %d, player score: %d\n", ai_score_l, player_score_l);
            *player_score = player_score_l;
            *ai_score = ai_score_l;
        }
        else if (player_move == SCISSORS && ai_move == ROCK)
        {
            ai_score_l++;
            printf("Rock bashes scissors. You lose!\n");
            printf("AI score: %d, player score: %d\n", ai_score_l, player_score_l);
            *player_score = player_score_l;
            *ai_score = ai_score_l;
        }
        else if (player_move == PAPER && ai_move == ROCK)
        {
            player_score_l++;
            printf("Paper covers rock. You win!\n");
            printf("AI score: %d, player score: %d\n", ai_score_l, player_score_l);
            *player_score = player_score_l;
            *ai_score = ai_score_l;
        }
        else if (player_move == PAPER && ai_move == SCISSORS)
        {
            ai_score_l++;
            printf("Scissors cuts paper. You lose!\n");
            printf("AI score: %d, player score: %d\n", ai_score_l, player_score_l);
            *player_score = player_score_l;
            *ai_score = ai_score_l;
        }
        else
        {
            printf("Draw. You and AI made the same move.\n");
            printf("AI score: %d, player score: %d\n", ai_score_l, player_score_l);
            *player_score = player_score_l;
            *ai_score = ai_score_l;
        }
    }
    return 0;
}

int main()
{
    srand(time(0));

    int player_move_o = 0;
    int ai_move_o = 0;
    int player_score = 0;
    int ai_score = 0;
    int rounds;

    printf("Welcome to Rock, Paper, Scissors!\nHow many rounds do you want to play?\n");
    scanf("%d", &rounds);

    for (int i = 0; i < rounds; i++)
    {
        printf("Which do you choose? Enter (1) for rock, (2) for paper, or (3) for scissors?\n");
        scanf("%d", &player_move_o);

        // generate random number for ai move
        ai_move_o = (rand() % 3) + 1;

        if (ai_move_o == ROCK)
        {
            printf("AI choses rock.\n");
        }
        else if (ai_move_o == PAPER)
        {
            printf("AI choses paper.\n");
        }
        else if (ai_move_o == SCISSORS)
        {
            printf("AI choses scissors.\n");
        }

        // game rule
        match(&player_move_o, &ai_move_o, &player_score, &ai_score);
    }
    return 0;
}
