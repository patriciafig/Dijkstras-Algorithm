/*
 *  File: main.cpp
 *  Created by Patricia on 4/24/17
 *
 *  CSCI 4761 Introduction to Computer Networks
 *  An implementation of the Link-State-Routing Algorithm
 *  using Dijkstra's Algorithm
 *
 *  Description:
 *  Dijkstra's algorithm was used to solve the single source shortest path
 *  problem for a graph.
 *
 */



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <sstream>

#define MAX 1073741824         // can be INF 
using namespace std;

// Method to find the minimum distance among all those elements which are not visited
int minDistance(const vector<int>& distance, const vector<bool>& visited) {
    // Initialize min value
    int min = MAX, minIndex;

    for (int v = 1; v < distance.size(); v++) {
        // If this vertex is not visited then update min distance if needed
        if (!visited[v] && distance[v] <= min) {
            min = distance[v];
            minIndex = v;
        }
    }

    return minIndex;
}

// This method applies the dijikstra's algorithm
// It returns the last parent of all the vertices on the shortest path
// It accepts adjacency matrix as a paratmeter along with source which will be the starting vertex
// for dijkistra algoithm. If flag is 1 then it will output distance for each iteration
vector<int> dijkistra(const vector<vector<int>>& adjMatrix, int source, vector<int>& distance, int flag) {
    // Array to keep which records are visited
    vector<bool> visited(adjMatrix.size(), false);
    vector<int> parent(adjMatrix.size(), source);
    vector<int> visitedVertices;

    // Resize the distance vector to that of N vertex, and mark the distane of source to 0 others to max or inf
    distance.resize(adjMatrix.size(), MAX);
    distance[source] = 0;

    // Used when printing only
    int t, d;
    if (flag == 1) {
        // t is used for printing ' ' in the N' column and d is number of '-' to be printed
        t = log(adjMatrix.size()), d = t + 20 + 8 * adjMatrix.size();
        t *= pow(10, t - 2) * 9;

        // Print dashes followed by column headers
        cout << string(d, '-') << "\nStep\tN'" << string(t, ' ') << "\t";
        for (int i = 1; i < adjMatrix.size(); ++i) {
            if (i != source)
                cout << " " << i << "\t";
        }
        cout << "\n" << string(d, '-') << "\n";
    }

    // This is the main part of dijkistra's algorithm
    for (int k = 1; k < adjMatrix.size() - 1; ++k) {
        // Find the vertex which is at closest distance
        int u = minDistance(distance, visited);

        // Visit that vertex and push it to visited Vertices which is used during printing only
        visited[u] = true;
        visitedVertices.push_back(u);

        // Update the distances of neighbors of the newly visited vertex
        for (int i = 1; i < adjMatrix.size(); ++i) {
            // If there is a edge from newly vertex to i'th node and vertex i is not already visited
            // and if we find new shortest distance then update it
            if (adjMatrix[u][i] != MAX && !visited[i] && distance[u] != MAX && distance[u] + adjMatrix[u][i] < distance[i]) {
                distance[i] = distance[u] + adjMatrix[u][i];
                parent[i] = u;
            }
        }

        // This is printing the details of each iteration
        if (flag == 1) {
            cout << k - 1 << "\t";

            // Firstly printing the N' column to a string so that we can output the string with the help of cout field width
            stringstream sout;
            sout <<"{" << visitedVertices[0];
            for (int i = 1; i < visitedVertices.size(); ++i) {
                sout << "," << visitedVertices[i];
            }
            sout << "}";

            // Using cout field width to properly pad the remaining space with spaces
            cout << setw(t) << left << sout.str() << "\t";

            // Print the details of other columns
            for (int i = 1; i < adjMatrix.size(); ++i) {
                if (i != source) {
                    if (!visited[i]) {
                        // Display INF if this vertex is not directly connected otherwise show distance
                        if (distance[i] == MAX)
                            cout << "INF";
                        else
                            cout << distance[i];

                        cout  << "," << parent[i] << "\t";
                    }
                    else
                        cout << "    \t";
                }

            }
            cout << "\n";
        }
    }

    return parent;
}

// This method is used to get the shortest path from source vertex to destination vertex
vector<int> getPath(const vector<int>& parent, int source, int destination) {
    int cur = destination;
    vector<int> path;

    if (source == destination) {
        path.push_back(source);
        return path;
    }

    // Start from destination and move to its parent and then to its parent and so on
    // to construct the shortest path in reverse order
    while (cur != source) {
        path.push_back(cur);
        cur = parent[cur];
    }

    return path;
}

int main(int argc, char* args[]) {
    if (argc != 4) {
        cout << "USAGE:  mylinkstate <test-input-file> <node i> <flag>";
        return 0;
    }
    // Start the clock to measure elapsed time
    clock_t begin = clock();
    int n, from, to, wt;
    ifstream fin(args[1]);
    vector<vector<int>> adjMatrix;
    // Read the number of vertices from the file
    fin >> n;

    // Incremented since we are storing vertices from 1 to n so index 0 is ignored
    n++;

    // Initalize adjacency matrix to store n elements
    adjMatrix.resize(n);
    for (int i = 0; i < n; ++i) {
        adjMatrix[i].resize(n, 0);
    }

    // Read the remaining lines from the file and update adjacency matrix
    while (fin >> from >> to >> wt) {
        adjMatrix[from][to] = wt;
    }
    fin.close();

    int vertex = atoi(args[2]);

    int flag = args[3][0] - '0';
    vector<int> distance;
    // Apply dijisktra's algorithm
    vector<int> parent = dijkistra(adjMatrix, vertex, distance, flag);

    // Print forwarding table
    cout << "\nFORWARDING TABLE:\n";
    cout << "\n  Destination\t|\t Link\n"
         << "____________________________________\n";
    for (int i = 1; i < adjMatrix.size(); ++i) {
        if (i != vertex) {
            vector<int> path = getPath(parent, vertex, i);
            cout << "\t" << i << "\t|\t(" << vertex << ", " << path[path.size() - 1] << ")\n";
        }
    }

    clock_t end = clock();

    // Display elasped time
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "\nExecution time: " << elapsed_secs << "s";
    return 0;
}
