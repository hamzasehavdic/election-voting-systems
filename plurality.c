#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // DONE
        if (!vote(name))
        {
            printf("Invalid vote.\n"); // CURRENTLY uses up vote slot >> while() loop == solution?
        }
    }
    // DONE
    print_winner();
    return 0;
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
        else
        {
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int i, j;
    // Bubble sort
    for (i = 0; i < candidate_count - 1; i++)
    {
        for (j = 0; j < candidate_count - i - 1; j++)
            // (candidate_count-i-1) for every time inner loop finishes, updates outer loop, the inner loop total decreases by i
        {
            if (candidates[j].votes > candidates[j + 1].votes)
            {
                int tempint = candidates[j].votes;
                string tempstr = candidates[j].name;

                candidates[j].votes = candidates[j + 1].votes;
                candidates[j].name = candidates[j + 1].name;

                candidates[j + 1].votes = tempint;
                candidates[j + 1].name = tempstr;
            }
        }
    }
    // Print First Winner
    printf("%s", candidates[i].name);
    printf("\n");
    // Print Tied Winners (if any)
    for (; i > 0; i--)
    {
        if (candidates[i].votes == candidates[i - 1].votes) // Print Tied Winner
        {
            printf("%s", candidates[i - 1].name);
            printf("\n");
        }
        else if (candidates[i].votes > candidates[i - 1].votes) // When there are no more Tied Winners; end
        {
            return;
        }
    }
}
