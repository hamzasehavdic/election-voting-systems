#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];
int losers[MAX];

int pair_count;
int candidate_count;
int voter_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
bool cycle_check(int winner, int loser);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++) // LOOP thru (ith candidates)
    {
        if (strcmp(name, candidates[i]) == 0) // IF (name) i/p MATCHES (ith candidate)
        {
            ranks[rank] = i; // UPDATE (ranks array [rank index]) to equal (ith candidate number)
            return true; // IF found >> return true
        }
    }
    return false; // ELSE not-found & invalid >> re turn false
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // DONE
    for (int i = 0; i < candidate_count - 1; i++) // LOOP thru every candidate for [ith or preferred pref]
    {
        for (int j = i + 1; j < candidate_count; j++) // LOOP thru the latter candidate prefs [jth pref]
            preferences[ranks[i]][ranks[j]]++; // INCREMENT +1 to preferred matchup
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Dont need to count pair index on final ith row, priior i rows wouldve added the pair
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // Add 1+i to avoid the same (but inverted) matchup or already added pairs
        for (int j = 1 + i; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++; // Update global pair count
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++; // Update global pair
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    while (1)
    {
        int tally = 0;
        for (int c = 0; c < pair_count; c++)
        {
            // If c pair strength < c+1 pair strength >> bubble c pair over c+1 pair; creating decreasing order
            if (preferences[pairs[c].winner][pairs[c].loser] - preferences[pairs[c].loser][pairs[c].winner] <
                preferences[pairs[c + 1].winner][pairs[c + 1].loser] - preferences[pairs[c + 1].loser][pairs[c + 1].winner])
            {
                int tempwinner = pairs[c].winner;
                int temploser = pairs[c].loser;

                pairs[c].winner = pairs[c + 1].winner;
                pairs[c].loser = pairs[c + 1].loser;

                pairs[c + 1].winner = tempwinner;
                pairs[c + 1].loser = temploser;

                tally = 1;
            }
        }
        if (tally == 0) // Break loop when no changes made >> when loop is finished
            break;
    }
}

bool cycle_check(int winner, int loser)
{
    // base case
    if (locked[loser][winner] == true)
        return true;

    // recursive case
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner] == true)
            return cycle_check(i, loser);
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!cycle_check(pairs[i].winner, pairs[i].loser))
            locked[pairs[i].winner][pairs[i].loser] = true;
    }
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true) // if candidate i loses then count
                count++;
        }
        if (count == 0) // &if candidate i never lost, theyre a||the winner
            printf("%s\n", candidates[i]);
    }
}
