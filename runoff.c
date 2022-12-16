#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max voters and candidates || DEFINING CONSTANTS
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];
// pref[i] rep prefs for voter i
// pref[i][j] store index of the candidate who is jth pref for voter i

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated; // important: use for runoff situations
}
candidate;

// Array of candidates || Tracks candidates in election
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES) // If > 9 cmd line args || too many candidates
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++) // Loop which populates candidates array
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: "); // Get N voters
    if (voter_count > MAX_VOTERS) // If > 100 voters || too many voters
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++) // Loop for every voter to record pref
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++) // Loop to record each voters prefs
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name)) // When (voter n) (pref n) (inputted candidate name) is false: print error
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // DONE
    // Look for a candidate called name (search thru candidate array)
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            // Debugger >> Check if voter pref. is updated // printf("Voter: %d \n Rank: %d\n", voter, rank);
            return true;
        }
    }
    // Else if no candidate found: return false
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // DONE
    const int top_choice = 0;

    for (int i = 0; i < voter_count; i++)
    {
        if (candidates[preferences[i][top_choice]].eliminated == false) //Candidate is not elim >> take top pref
        {
            candidates[preferences[i][top_choice]].votes++;
        }
        else // Candidate is elim >> take next pref
        {
            int new_top_choice = top_choice;

            do // Find non-elim candidate
            {
                new_top_choice++;
            }
            while (candidates[preferences[i][new_top_choice]].eliminated == true);

            candidates[preferences[i][new_top_choice]].votes++; // Give non-elim candidate vote
        }
        // printf("Alice votes: %i\n Bob votes: %i\n Charlie votes: %i\n", candidates[0].votes, candidates[1].votes, candidates[2].votes);
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // DONE
    // IF any candidate has >half of vote >> printer winner name & return true
    float rounded_half_count = roundf((float) voter_count / 2);
    int majority = (int) rounded_half_count;

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == majority && (voter_count % 2) == 0) // IF candidate has exactly 50% of vote
        {
            return false;
        }
        else if (candidates[i].votes >= majority)
        {
            printf("%s", candidates[i].name);
            printf("\n");
            return true;
        }
    }
    // IF noone has won election yet, return false
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // DONE
    int min;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count - i; j++)
        {
            if (candidates[i].eliminated == false && candidates[i + j].eliminated == false
                && candidates[i].votes <= candidates[i + j].votes)
                // Store non-elim candidate with few
            {
                min = candidates[i].votes;
            }
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min && candidates[i].eliminated == false)
        {
            count++;
        }
        else if (candidates[i].eliminated == true) // return true if there are any eliminated candidates
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    if (count == candidate_count) // return true when election is tied
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // DONE
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min && candidates[i].eliminated == false) // eliminate non-elim candidates with min votes
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
