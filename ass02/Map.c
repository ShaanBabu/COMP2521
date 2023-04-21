// Implementation of the Map ADT | z5280731

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"

typedef struct {
    char name[1000];
    int identify;
} City;

struct map { 
    // TODO
    int numCities;
    int numRoads;
    City** cities;
    Road** roads;
};

/**
 * Creates a new map with the given number of cities
 * Assumes that numCities is positive
 */
Map MapNew(int numCities) {
    // TODO
    Map m = (Map)malloc(sizeof(struct map));
    m->numCities = numCities;
    m->numRoads = 0;
    m->roads = NULL;

    // initialise array
    m->cities = (City**)calloc(numCities, sizeof( City*));
    for (int i = 0; i < numCities; i++) {
        m->cities[i] = (City*)malloc(sizeof(City));
        m->cities[i]->identify = i;
    }
    
    return m;
}

/**
 * Frees all memory associated with the given map
 */
void MapFree(Map m) {
    // TODO
    for (int i = 0; i < m->numRoads; i++) {
        free(m->roads[i]);
    }

    for (int i = 0; i < m->numCities; i++) {
        free(m->cities[i]);
    }

    if (m->numRoads > 0) {
        free(m->roads);
    }

    if (m->numCities > 0) {
        free(m->cities);
    }

    free(m);
}

/**
 * Returns the number of cities on the given map
 */
int MapNumCities(Map m) {
    // TODO
    return m->numCities;
}

/**
 * Returns the number of roads on the given map
 */
int MapNumRoads(Map m) {
    // TODO
    return m->numRoads;
}

/**
 * Inserts a road between two cities with the given length
 * Does nothing if there is already a road between the two cities
 * Assumes that the cities are valid and are not the same
 * Assumes that the length of the road is positive
 */
void MapInsertRoad(Map m, int city1, int city2, int length) {
    // TODO
    if (MapContainsRoad(m, city1,city2) == 1) {
        return;
    }
    m->numRoads += 1;

    if (m->numRoads == 1) {
        m->roads = (Road**)calloc(1,sizeof(Road*));
    } else {
        m->roads = (Road**)realloc(m->roads,(sizeof(Road*))*(m->numRoads));
    }

    Road* road = (Road*)malloc(sizeof(Road));
    road->from = city1;
    road->to = city2;
    road->length = length;

    m->roads[(m->numRoads) - 1] = road;
}

/**
 * Sets the name of the given city
 */
void MapSetName(Map m, int city, char *name) {
    // TODO
    if (city >= m->numCities) {
        return;
    }

    strcpy(m->cities[city]->name, name);
}

/**
 * Returns the name of the given city
 */
char *MapGetName(Map m, int city) {
    if (city >= m->numCities) {
        return NULL;
    }

    return m->cities[city]->name;
}

/**
 * Checks if there is a road between the two given cities
 * Returns the length of the road if there is a road, and 0 otherwise
 */
int MapContainsRoad(Map m, int city1, int city2) {
    // TODO
    // find if road is present between the two given cities
    for (int i = 0; i < m->numRoads; i++) {
        int src = m->roads[i]->from;
        int dest = m->roads[i]->to;
        if ((src == city1 && dest == city2) || (src == city2 && dest == city1)) {
            return 1;
        }
    }

    return 0;
}

/**
 * Returns the number of roads connected to the given city and stores
 * them in the given roads array. The `from` field should be equal to
 * `city` for all the roads in the array.
 * Assumes that the roads array is large enough to store all the roads
 */
int MapGetRoadsFrom(Map m, int city, Road roads[]) {
    // TODO
    int counter = 0;
    // find all connecting roads with city - in map terms find degrees
    for (int i = 0; i < m->numRoads; i++) {
        Road* r = m->roads[i];
        if (r->from == city) {
            roads[counter].from = city;
            roads[counter].to = r->to;
            roads[counter].length = r->length;
            counter++;
        } else if (r->to == city) {
            roads[counter].from = city;
            roads[counter].to = r->from;
            roads[counter].length = r->length;
            counter++;
        }
    }

    return counter;
}

/**
 * Displays the map
 * !!! DO NOT EDIT THIS FUNCTION !!!
 * This function will work once the other functions are working
 */
void MapShow(Map m) {
    printf("Number of cities: %d\n", MapNumCities(m));
    printf("Number of roads: %d\n", MapNumRoads(m));
    
    Road *roads = malloc(MapNumRoads(m) * sizeof(Road));
    if (roads == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);    
    }
    
    for (int i = 0; i < MapNumCities(m); i++) {
        printf("[%d] %s has roads to:", i, MapGetName(m, i));
        int numRoads = MapGetRoadsFrom(m, i, roads);
        for (int j = 0; j < numRoads; j++) {
            if (j > 0) {
                printf(",");
            }
            printf(" [%d] %s (%d)", roads[j].to, MapGetName(m, roads[j].to),
                   roads[j].length);
        }
        printf("\n");
    }
    
    free(roads);
}
