#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_CANDIDATES 9
#define MAX_VOTERS 100
#define MAX_NAME_LENGTH 30

int preferences[MAX_VOTERS][MAX_CANDIDATES];
typedef struct
{
    char name[MAX_NAME_LENGTH + 1];
    int votes;
    bool eliminated;
}
candidate;

candidate candidates[MAX_CANDIDATES];
int voter_count;
int candidate_count;

bool vote(int voter, int rank, char *name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./runoff candidate1 candidate2 ...\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        strcpy(candidates[i].name, argv[i + 1]);
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    printf("Number of voters: ");
    if (scanf("%d", &voter_count) != 1 || voter_count > MAX_VOTERS)
    {
        return 3;
    }
    while (getchar() != '\n');

    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            char name[MAX_NAME_LENGTH + 1];
            printf("Rank %i: ", j + 1);

            if (fgets(name, sizeof(name), stdin) == NULL) return 4;
            name[strcspn(name, "\n")] = 0;

            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 5;
            }
        }
        printf("\n");
    }

    while (true)
    {
        tabulate();
        
        if (print_winner()) break;

        int min = find_min();
        bool tie = is_tie(min);

        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated) printf("%s\n", candidates[i].name);
            }
            break;
        }

        eliminate(min);
    }
    return 0;
}

bool vote(int voter, int rank, char *name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

void tabulate(void)
{
    for (int i = 0; i < candidate_count; i++) candidates[i].votes = 0;

    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            int preferred = preferences[i][j];
            if (!candidates[preferred].eliminated)
            {
                candidates[preferred].votes++;
                break;
            }
        }
    }
}

bool print_winner(void)
{
    int majority = voter_count / 2 + 1;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes >= majority)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

int find_min(void)
{
    int min_votes = MAX_VOTERS + 1;
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes < min_votes)
        {
            min_votes = candidates[i].votes;
        }
    }
    return min_votes;
}

bool is_tie(int min)
{
    int active_candidates = 0;
    int min_voted_candidates = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            active_candidates++;
            if (candidates[i].votes == min)
            {
                min_voted_candidates++;
            }
        }
    }
    return active_candidates == min_voted_candidates;
}

void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
}
