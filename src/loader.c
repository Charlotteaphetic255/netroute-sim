#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256

graph* loadGraphFromFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filepath);
        return NULL;
    }

    char line[MAX_LINE_LEN];
    graph* g = NULL;
    int nodesCount = 0;

    while (fgets(line, sizeof(line), file)) {
        //skip empty lines
        if (line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        //skip comment lines
        if (line[0] == '#') {
            continue;
        }

        //parse NODES line to create graph
        if (strncmp(line, "NODES", 5) == 0) {
            if (sscanf(line, "NODES %d", &nodesCount) == 1) {
                g = createGraph(nodesCount);
                if (!g) {
                    printf("Error: Failed to create graph with %d nodes\n", nodesCount);
                    fclose(file);
                    return NULL;
                }
            }
            continue;
        }

        //skip EDGES line
        if (strncmp(line, "EDGES", 5) == 0) {
            continue;
        }

        //parse edge lines (src dest weight)
        if (g) {
            int src, dest, weight;
            if (sscanf(line, "%d %d %d", &src, &dest, &weight) == 3) {
                //validate node indices
                if (src >= 0 && src < nodesCount && dest >= 0 && dest < nodesCount) {
                    addEdge(g, src, dest, weight);
                } else {
                    printf("Warning: Invalid edge (%d, %d) - node index out of range\n", src, dest);
                }
            }
        }
    }

    fclose(file);
    return g;
}
