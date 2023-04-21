// Implementation of the Agent ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Agent.h"
#include "Map.h"

// This struct stores information about an individual agent and can be
// used to store information that the agent needs to remember.
struct agent {
    char *name;
    int startLocation;
    int location;
    int maxStamina; // max stamina
    int stamina;    // current stamina
    int strategy;
    Map map;
    
    // TODO: Add more fields here as needed
    int* visitedCount; // stores counted or visited cities in array form

    // DFS paths according to agent movement
    int dfsCitiesCount;
    int currentDfsCityTag;
    Road** dfsCities;
};

static Move chooseRandomMove(Agent agent, Map m);
static Move chooseDfsMove(Agent agent, Map m);
static Move chooseLeastVisitedMove(Agent agent, Map m);
static int filterRoads(Agent agent, Road roads[], int numRoads,
                       Road legalRoads[]);

// Function to fetch the DFS traversal from a location using recursion
void dfsTraversal(int currentCity, Road** citiesPath, int* cityId, int* visited, Map m) {
    visited[currentCity] = 1;
    
    Road *roads = malloc(MapNumCities(m) * sizeof(Road));

    int numRoads = MapGetRoadsFrom(m, currentCity, roads); // same method of lisitng as random-stategy
    
    // if roads connecting is present
    if (numRoads > 0) {
        // loop through the connecting cities-nodes
        for (int i = 0; i < numRoads; i++) {
            Road r = roads[i];
            
            int desitination = r.to;
            // visit desitination city if not visited and store as visited
            if(visited[desitination] == 0) {
                // the road is added to current function of DFS
                Road *current = malloc(sizeof(Road));
                current->to = r.to;
                current->from = r.from;
                current->length = r.length;
                citiesPath[*cityId] = current;
                *cityId += 1;
                dfsTraversal(desitination, citiesPath, cityId, visited, m);
            }
        }
    }
    free(roads);

}

// Function to obtain DFS stages 
// idea for function obtained from help session and tutor:
// suggested idea to represent the function as part of Road instead of static
Road** getDfsPath(int initialCity, Map m, int *count) {
    
    int totalCities = MapNumCities(m);

    // from initial city store DFS stages
    Road** citiesPath = (Road**)calloc(totalCities, sizeof(Road*));
    int* visited = (int*)calloc(totalCities, sizeof(int));

    for(int i = 0 ; i < totalCities; i++) {
        citiesPath[i] = NULL;
        visited[i] = 0;
    }

    int indicator = 0;
    dfsTraversal(initialCity, citiesPath, &indicator, visited, m);
    free(visited);

    // involvement with back-tracking
    Road** newCitiesPath = (Road**)calloc(totalCities*5, sizeof(Road*));

    int numIndicator = 0;
    // Loop over each path from simple DFS traversal
    for(int i = 0; i < indicator; i++) {
        
        Road *currRoad = malloc(sizeof(Road));
        Road *road = citiesPath[i];

        newCitiesPath[numIndicator++] = currRoad;
        currRoad->from = road->from;
        currRoad->to = road->to;
        currRoad->length = road->length;

        // if statement in case not last road in DFS 
        if(i != indicator - 1) {
            Road* nextRoad = citiesPath[i + 1];

            if(nextRoad->from == road->to) {
                continue;
            } else {
                // If there is no path from current city to next, backtracing takes place
                int index = i;
                // creation of infinite loop here according to tutor reccomendation
                // halts when backtracking not valid
                while(1) {
                    Road* prevRoad = citiesPath[index];
                    Road * presentRoad = (Road*)malloc(sizeof(Road));
                    
                    presentRoad->from = prevRoad->to;
                    presentRoad->to = prevRoad->from;
                    presentRoad->length = prevRoad->length;
                    newCitiesPath[numIndicator++] = presentRoad;

                    // no backtracking allowed
                    if(nextRoad->from == prevRoad->from) {
                        break;
                    }
                    index--;
                }
            }
        }
    }

    // Free all the memory allocated for DFS path
    for(int i = 0; i < totalCities; i++) {
          free(citiesPath[i]);
    }
    free(citiesPath);
    *count = numIndicator;
    return newCitiesPath;
}

/**
 * Creates a new agent
 */
Agent AgentNew(int start, int stamina, int strategy, Map m, char *name) {
    if (start >= MapNumCities(m)) {
        fprintf(stderr, "error: starting city (%d) is invalid\n", start);
        exit(EXIT_FAILURE);
    }
    
    Agent agent = malloc(sizeof(struct agent));
    if (agent == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    agent->startLocation = start;
    agent->location = start;
    agent->maxStamina = stamina;
    agent->stamina = stamina;
    agent->strategy = strategy;
    agent->map = m;
    agent->name = strdup(name);
    agent->dfsCities = NULL;
    agent->currentDfsCityTag = 0;
    
    // TODO: You may need to add to this
    int totalCities = MapNumCities(m);

    // Allocate memory for visited count for each city
    agent->visitedCount = (int*)calloc(totalCities, sizeof(int));
    for (int i=0;i< totalCities; i++) {
        agent->visitedCount[i] = 0;
    }

    agent->visitedCount[start] = 1;
    
    return agent;
}

/**
 * Frees all memory associated with the agent
 * NOTE: You should not free the map because the map is owned by the
 *       main program, and the main program will free it
 */
void AgentFree(Agent agent) {
    // TODO: You may need to update this to free any extra memory you use
   
    free(agent->visitedCount);
    // Free DFS when full
    if (agent->dfsCities != NULL) {
        
        for (int i = 0; i < agent->dfsCitiesCount; i++) {
            free(agent->dfsCities[i]);
        }
        free(agent->dfsCities);
    }
    free(agent->name);
    free(agent);
}

////////////////////////////////////////////////////////////////////////
// Making moves

/**
 * Calculates the agent's next move
 */
Move AgentGetNextMove(Agent agent, Map m) {
    switch (agent->strategy) {
        case STATIONARY: return (Move){agent->location, 0};
        case RANDOM:     return chooseRandomMove(agent, m);
        case CHEAPEST_LEAST_VISITED:     return chooseLeastVisitedMove(agent, m);
        case DFS:     return chooseDfsMove(agent, m);
        // TODO: Implement more strategies here
        default:
            printf("error: strategy not implemented yet\n");
            exit(EXIT_FAILURE);
    }
}

static Move chooseRandomMove(Agent agent, Map m) {
    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Road *legalRoads = malloc(MapNumCities(m) * sizeof(Road));

    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, agent->location, roads);

    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);

    Move move;
    if (numLegalRoads > 0) {
        // Sort the roads using insertion sort
        for (int i = 1; i < numLegalRoads; i++) {
            Road r = legalRoads[i];
            int j = i;
            while (j > 0 && r.to < legalRoads[j - 1].to) {
                legalRoads[j] = legalRoads[j - 1];
                j--;
            }
            legalRoads[j] = r;
        }
        
        // nextMove is randomly chosen from the legal roads
        int k = rand() % numLegalRoads;
        move = (Move){legalRoads[k].to, legalRoads[k].length};
    } else {
        // The agent must stay in the same location
        move = (Move){agent->location, 0};
    }
    
    free(legalRoads);
    free(roads);
    return move;
}

// Function to get next moved based from LeastVisitedStrategy
static Move chooseLeastVisitedMove(Agent agent, Map m) {
    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Road *legalRoads = malloc(MapNumCities(m) * sizeof(Road));

    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, agent->location, roads);

    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);

    Move move;
    int destination = -1;
    int visitedAgentCount = -1;
    int destinationLength = -1;
    if (numLegalRoads > 0) {
        for (int i = 0; i < numLegalRoads; i++) {
            
            Road r = legalRoads[i];

            int thisRoadVisitedCount = agent->visitedCount[r.to];

            //  lowest visited cities have priority
            int firstCondition = thisRoadVisitedCount < visitedAgentCount;
            // cities visited same times and lowest length city has priority
            int secondCondition = thisRoadVisitedCount == visitedAgentCount && r.length < destinationLength;
            // cities visited same amount of times and same length,  lower index city has higher priority
            int thirdCondition = thisRoadVisitedCount == visitedAgentCount && r.length == destinationLength && destination > r.to;

            // If first city we are moving to
            if (destination == -1) {
                destination = r.to;
                visitedAgentCount = thisRoadVisitedCount;
                destinationLength = r.length;
            } else if (firstCondition || secondCondition || thirdCondition){
                destination = r.to;
                visitedAgentCount = thisRoadVisitedCount;
                destinationLength = r.length;
            }
        }
        
        move = (Move){destination, destinationLength};
    } else {
        // The agent must stay in the same location
        move = (Move){agent->location, 0};
    }
    
    free(legalRoads);
    free(roads);
    return move;
}

// Function to get next moved based from DfsStrategy
static Move chooseDfsMove(Agent agent, Map m) {

    // If agent hasnt expolored DFS specific road yet
    if (agent->dfsCities == NULL) {
        int counter = 0;
        agent->dfsCities = getDfsPath(agent->location, m, &counter);
        agent->dfsCitiesCount = counter;
        agent->currentDfsCityTag = 0;
    } else if (agent->currentDfsCityTag == agent->dfsCitiesCount) {
        // generate new DFS when already finished circulating map
        for (int i = 0; i < agent->dfsCitiesCount; i++) {
            free(agent->dfsCities[i]);
        }
        free(agent->dfsCities);
        int counter = 0;
        agent->dfsCities = getDfsPath(agent->location, m, &counter);
        agent->dfsCitiesCount = counter;
        agent->currentDfsCityTag = 0;
    }

    // check for stamina needed for next city
    Road* nextRoad = agent->dfsCities[agent->currentDfsCityTag];
    int length = nextRoad->length;
    
    // can travel to next city
    if(agent->stamina >= length) {
        agent->currentDfsCityTag += 1;
        return (Move){nextRoad->to, length};
    } else {
        return (Move){agent->location, 0};
    }
}

// Takes an array with all the possible roads and puts the ones the agent
// has enough stamina for into the legalRoads array
// Returns the number of legal roads
static int filterRoads(Agent agent, Road roads[], int numRoads,
                       Road legalRoads[]) {
    int numLegalRoads = 0;
    for (int i = 0; i < numRoads; i++) {
        if (roads[i].length <= agent->stamina) {
            legalRoads[numLegalRoads++] = roads[i];
        }
    }
    return numLegalRoads;
}

/**
 * Executes a given move by updating the agent's internal state
 */
void AgentMakeNextMove(Agent agent, Move move) {
    if (move.to == agent->location) {
        agent->stamina = agent->maxStamina;
    } else {
        agent->stamina -= move.staminaCost;
    }
    agent->location = move.to;
   
    // TODO: You may need to add to this to handle different strategies
    agent->visitedCount[move.to] +=1;
    // if(strcmp(agent->name, "D4") == 0) {
    //     printf(" Chaging loc %d %d  %d\n", move.to, agent->visitedCount[move.to], agent->visitedCount[0]);
    // }
}


////////////////////////////////////////////////////////////////////////
// Gets information about the agent
// NOTE: It is expected that these functions do not need to be modified

/**
 * Gets the name of the agent
 */
char *AgentName(Agent agent) {
    return agent->name;
}

/**
 * Gets the current location of the agent
 */
int AgentLocation(Agent agent) {
    return agent->location;
}

/**
 * Gets the amount of stamina the agent currently has
 */
int AgentStamina(Agent agent) {
    return agent->stamina;
}

////////////////////////////////////////////////////////////////////////
// Learning information

/**
 * Tells the agent where the thief is
 */
void AgentGainInfo(Agent agent, int thiefLocation) {
    // TODO: Stage 3
}

////////////////////////////////////////////////////////////////////////
// Displaying state

/**
 * Prints information about the agent (for debugging purposes)
 */
void AgentShow(Agent agent) {
    // TODO: You can implement this function however you want
    //       You can leave this function blank if you want
}

////////////////////////////////////////////////////////////////////////