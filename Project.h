#pragma once

#include <utility>
#include <iostream>

/**
 * For the purposes of creating the edge and the graph, we are using the starter code provided in the
 * the lab_ml directory. The graph and edge classes in particular are utllized. 
 */
#include "Graph/graph.h"
#include "Graph/edge.h"
#include "Graph/random.h"

/**
 * For the purposes of creating the Image and modifying the pixels to plot airports
 * and their connected routes, we are using the starter code provided in the
 * the mp_stickers directory. The Image, PNG, and HSLApixel. classes in particular are utllized. 
 */
#include "ImageData/Image.h"

#include <vector>
#include <algorithm>
#include <map>

class Project {
    public:
        //obtains x and y coordintaes as a pair on an image given latitude and longitude on a world map
        std::pair<double,double> get_coordinates(double latitude, double longitude);

        //outputs a graph onto the file input
        void map_projection(const std::string &filename);

        //constructor for the object (creates the graph and reads in an image)
        Project();

        // adds all the edges to the graph
        void create_edges();

        //adds all the vertices to the graph
        void add_airportvertex();

        //calculate distance on an image sgiven an x and y value
        double calculate_distance(int x, int y);

        /* Creates an empty weighted and directed graph to store our route data */
        Graph routes_graph;

        /* Stores a map of vertex to coordinates for lookup */
        std::map<std::string, std::pair<int, int>> vertex_to_coord;
        
        // find minimum number of edges between nodes input and print the path and stopovers
        void BFS (Vertex current_airport, Vertex destination);

        // find minimum dsitance between nodes input and print the path and distance
        void DijkstrasAlgorithm (Vertex source_airport, Vertex destination);

        //prints out edges from the graph onto an image
        void DrawEdgeOnMap(Image & backgroundImg, const std::string &filename);
        
        //prints out vertices from the graph onto an image
        void DrawAirportMarkerOnMap(Image & backgroundImg, const std::string &filename);

        //get source and destination vertices from the user
        Vertex getUserInput();

        //prints path from the input source to an input destination
        void printingPath(Vertex destination, std::map<Vertex, Vertex> parents);

        /**
         * Creates the graph by inserting vertices from the filename_airport file and
         * inserting edges from the filename_route file
         */
        void buildGraph(const std::string & filename_airport, const std::string & filename_route);

        //map to store each line of route file with key as row in file
        std::map<int, std::vector<std::string>> routes_map; 

        //map to store each line of airport file with key as row in file
        std::map<int, std::vector<std::string>> airports_map;

        //the Image that stores the map input in the graph constructor
        Image map_image;

        /**
        * A function to take in an input file and return a map where each line is
        * stored with the key as the row
        */
        std::map<int, std::vector<std::string>> parse_csv (const std::string &fileToRead);

        //a string to help concatenate strings to help with printing paths
        std::string returnStr; 
};