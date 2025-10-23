#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CANDIDATES 9
#define MAX_NAME_LENGTH 30

typedef struct { char name[MAX_NAME_LENGTH + 1]; int votes; } candidate;

candidate candidates[MAX_CANDIDATES];
int candidate_count;

int vote(char *name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return 1;
        }
    }
    return 0;
}

void print_winner(void)
{
    int max_votes = 0;
    for (int i = 0; i < candidate_count; i++)
        if (candidates[i].votes > max_votes) max_votes = candidates[i].votes;

    for (int i = 0; i < candidate_count; i++)
        if (candidates[i].votes == max_votes) printf("%s\n", candidates[i].name);
}

int main(int argc, char *argv[])
{
    if (argc < 2 || (candidate_count = argc - 1) > MAX_CANDIDATES)
    {
        printf(argc < 2 ? "Usage: ./plurality candidate1 candidate2 ...\n" : "Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 1;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        strcpy(candidates[i].name, argv[i + 1]);
        candidates[i].votes = 0;
    }

    int voter_count;
    printf("Number of voters: ");
    if (scanf("%d", &voter_count) != 1) return 2;
    while (getchar() != '\n');

    for (int i = 0; i < voter_count; i++)
    {
        char name[MAX_NAME_LENGTH + 1];
        printf("Vote: ");

        if (fgets(name, sizeof(name), stdin) == NULL) return 3;
        name[strcspn(name, "\n")] = 0;

        if (!vote(name)) printf("Invalid vote.\n");
    }

    print_winner();

    return 0;
}
