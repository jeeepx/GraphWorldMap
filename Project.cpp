/**
 * For the purposes of our intended graphical output projection of all the airports in our dataset, we
 * utilized the CS225 namespace - particularly the PNG and HSLAPixel classes. We also utilized our 
 * code from mp_intro and lab_ml for the Image and Graph classes.
 */
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include "ImageData/Image.h"

#include "Project.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <sstream>
#include <queue>

//Constructor for our graph - which initializes our graph as weighted and directed
Project::Project() : routes_graph(true, true) {
  //The image file on which the graph is projected is read in
  map_image.readFromFile("data/newmap.png");
}

/** 
 * The graph is built with the files of airports and routes data 
 * which is parsed into our private members - routes_map and airports_map.
 */
void Project::buildGraph(const std::string & filename_airport, const std::string & filename_route) {

  routes_map = parse_csv(filename_route);
  airports_map = parse_csv(filename_airport);

  /** 
   * We also create all the vertices and edges in our graph from the private maps 
   * that are based on the datasets provided
   */
  add_airportvertex();
  create_edges();
}

// To add all vertices into our graph
void Project::add_airportvertex() {
 
  std::string airport, lat_string, longi_string;
  
  // To obtain the airport code, latitude, longitude 
  // of each line of airport data from our airports_map 
  for (unsigned long i = 0; i < airports_map[0].size(); i++) {
    //Based on the format of each line in the airports.txt dataset, retrives the information
    airport = airports_map[4][i];
    lat_string = airports_map[6][i];
    longi_string = airports_map[7][i];

    //change to ICAI code if IATA code does not exist
    if (airport.length() != 3) {
      airport = airports_map[5][i];
    }

    double latitude = 0.0;
    double longitude = 0.0;

    //converting the string output to double
    std::stringstream temp_lat(lat_string);
    temp_lat >> latitude;

    std::stringstream temp_long(longi_string);
    temp_long >> longitude;

    //initializing a pair of coodinates which takes the return value of get_coordinates
    std::pair<int, int> coordinates;
    coordinates = get_coordinates(latitude, longitude);

    //adds each airport code as an Vertex (node) in our graph
    routes_graph.insertVertex(airport);
    //maps the airport to a pair of ints corresponding to the coordinates
    vertex_to_coord.insert(std::pair<std::string, std::pair<int, int>>(airport, coordinates));
  }
}

/**
 * BFS Traversal Function for the airport's graph.
 */
void Project::BFS (Vertex current_airport, Vertex destination) {
  std::cout << " -- BFS Implementation to print the path with minimum number of stopovers -- " << std::endl;
  
  //making a structure to keep track of distances and predecessor
  std::map<Vertex, int> distance;
  std::map<Vertex, Vertex> parent_node;

  //create an empty queue
  std::queue<Vertex> BFS_queue;
  BFS_queue.push(current_airport);
  
  std::map<Vertex, bool> visited_nodes;
  std::vector<Vertex> vertices = routes_graph.getVertices();

  //initialize all vertices being visited as false
  for (Vertex v : vertices) {
    visited_nodes.insert(std::pair<Vertex, bool>(v, false));
    distance.insert(std::pair<Vertex, int>(v, -1));
    parent_node.insert(std::pair<Vertex, Vertex>(v, "Unvisited"));
  }

  //set current airport's been visited as true
  visited_nodes[current_airport] = true;
  distance.at(current_airport) = 0;

  if (distance.at(destination) == 0) { // No flight if destination is zero since same as parent
     std::cout << "The source is the same as destination. No flight taken." << std::endl;
     return;
  }

  while (!BFS_queue.empty()) { //iterate until all nodes visited and queue is empty
    Vertex currentNode = BFS_queue.front(); // store current node
    BFS_queue.pop(); // remove it from front of queue

    //make adjacency list
    std::vector<Vertex> adjacent_nodes = routes_graph.getAdjacent(currentNode);// obtain adjacent vertices
    for (Vertex connected_airport : adjacent_nodes) { // range based for loop to access each node
      if (!(visited_nodes.find(connected_airport))->second) { //check if node is visited, if not only then access it
        visited_nodes.at(connected_airport) = true; // mark node which is now being visited as true
        BFS_queue.push(connected_airport); // add unvisited node to queue

        distance.at(connected_airport) = distance.at(currentNode) + 1;// update distance by an additional edge 
        parent_node.at(connected_airport) = currentNode; // update parent with previous node
      }
    }
  }

  std::cout << " " << std::endl;

  if (distance.at(destination) == -1) { //could not reach that node since still initialized value so no path
    std::cout << "No possible flight path" << std::endl;
    return;
  }
  //print out total sum of edges which corresponds to each flight
  std::cout << "Number of flights to take: " << distance.at(destination) << std::endl;
  // stopovers is simply 1 minus the edges
  std::cout << "Number of stopovers: " << distance.at(destination) - 1 << std::endl; 

  printingPath(destination, parent_node); // helper function to print path from source to destination
}


/**
 * Dijkstra's Algorithm Traversal Function for the airports graph
 * Finds the shortest path in terms of distance from source to destination
 * using edge weights 
 */
void Project::DijkstrasAlgorithm (Vertex source_airport, Vertex destination) {
  if (source_airport == destination) { //if both source and destination vertex are same
     std::cout << "The source is the same as destination. No flight taken." << std::endl;
     return;
  }

  Vertex current_airport;

  //declare variables
  std::map<Vertex, int> distances;// keeps track of actual distance of each node from source
  std::map<Vertex, Vertex> predecessor; // keeps track of parents
  std::map<Vertex, bool> visited_nodes; //keep track of nodes accessed

  std::vector<Vertex> all_vertices = routes_graph.getVertices(); //obtain all vertices adjacent to current airport

  //initialize all the variables
  int INF = std::numeric_limits<int>::max();

  for (Vertex v : all_vertices) { // range based for loop through all neighbors to initialize variables
    visited_nodes.insert(std::pair<Vertex, bool>(v, false));
    distances.insert(std::pair<Vertex, int>(v, INF)); //set all distances to infinity as they are unknown
    predecessor.insert(std::pair<Vertex, Vertex>(v, "Unvisited"));
  }

  distances.at(source_airport) = 0;

  current_airport = source_airport; //initialize current with source at the beginning to see start of path

  // iterate till all nodes are visited so all paths traversed
  while (visited_nodes.at(current_airport) == false) {
    visited_nodes.at(current_airport) = true;
    std::vector<Vertex> adjacent_nodes = routes_graph.getAdjacent(current_airport); // obtain adjacent for current

    //obtain each edge weight which is distance from twp nodes
    for (Vertex neighbor : adjacent_nodes) {
      Edge e = routes_graph.getEdge(current_airport, neighbor);
      int weight = e.getWeight();
      
      //if smaller path between current and neighbor found update distnace to smaller
      if (distances.at(current_airport) + weight < distances.at(neighbor)) {
        int add_checker = distances.at(current_airport) + weight;
        distances.at(neighbor) = add_checker; //update distance at node in map
        predecessor.at(neighbor) = current_airport;// update predecessor
      }
    }

    int minimum_distance = INF; // set min distance as max to replace whenever lower distance found
    Vertex reference = source_airport;

/**
 * Go through each adjacent node and if we found a path with a smaller distance and replace it
 * with path with new neighbor if it is not source and not already visited.
 */
    for (Vertex v : all_vertices) {
      if (v != source_airport && visited_nodes.at(v) != true && (distances.at(v) < minimum_distance)) {
        current_airport = v; // update node
        minimum_distance = distances.at(v); // replace with minimum distance 
      }
    }
  }

  std::cout << " -- Dijkstra's Algorithm to print the path with minimum distance -- " << std::endl;
  std::cout << " " << std::endl;

  // if not visited is still initialized to infinity
  if (distances.at(destination) == INF) { 
    std::cout << "No possible flight path" << std::endl;
    return;
  }

  // print out minimum distance stored
  std::cout << "Path distance: " << distances.at(destination) << std::endl;
  printingPath(destination, predecessor); //helper function to print out the path
}

/**
 * Helper function to print the shortest path output
 * between two airports that was found by running
 * BFS or Dijkstra's Algorithm.
 */
void Project::printingPath(Vertex destination, std::map<Vertex, Vertex> parents) {
  //Initializes a vector of vertices to store the vertices chosen in the shortest path
  std::vector <Vertex> toPrint;
  toPrint.push_back(destination);

  //We start from the destinaton and access the part at each point and
  //push this to a vector until we reach a parent that is Unvisited, this
  //vertex is the source.
  while (parents.find(destination)->second != "Unvisited") {
    toPrint.push_back(parents.at(destination));
    destination = parents.at(destination);
  }

  //As the vector stored the path in the reverse, we need
  //to reverse the path to print it in the correct order.
  reverse(toPrint.begin(), toPrint.end());

  returnStr = "";

  //For all vertices but the first one we want to add a "->" to show the connecting flights
  int count = 0;
  for (Vertex v : toPrint) {
    if (count == 0) {
      returnStr = v;
    } else {
      returnStr = returnStr + " -> " + v;
    }
    count++;
  }
  //Prints the path to terminal
  std::cout << "Path: " << returnStr << std::endl;
}


/**
 * Function to prompt user input and take 
 * in a string of the desired destination node
 * to be passed into the BFS and Dijkstra Algorithm 
 * functions as a parameter.
 */
Vertex Project::getUserInput() {
  Vertex destinationNode;
  std::cin >> destinationNode;

  // Checks for lowercase input of airport codes, converts to uppercase to search 
  // for the vertex in our graph.
  std::transform(destinationNode.begin(), destinationNode.end(), destinationNode.begin(), ::toupper);

  //Exits the function if the vertex does not exist as neither of BFS or Dijikstra's can be run.
  if (!routes_graph.vertexExists(destinationNode)) {
    std::cout << "Invalid input. This airport may not be present in the dataset or the entered code is incorrect." << std::endl;
    std::cout << "Please enter the correct Airport Code." << std::endl;
    exit(1);
  }

  //returns the user input in uppercase.
  return destinationNode;
}

/**
 * Helper function to create our graph.
 * Sets the weight for all the edges as the distance
 * between two given airports.
 */
void Project::create_edges() {
  std::map<std::string, std::vector<std::string>> string_map;
  std::string routes_data, source, destination;

  for (unsigned long i = 0; i < routes_map[0].size(); i++) {
    //Based on the format of each line in the routes.txt dataset, retrives the information
    //needed for source and destination
    source = routes_map[2][i];
    destination = routes_map[4][i];
    
    //Checks to see if both the source and the destination exist as vertices in the
    //graph and inserts an edge only if both exist
    if (routes_graph.vertexExists(source) && routes_graph.vertexExists(destination)) {
      routes_graph.insertEdge(source, destination);

      //Retriving the values for xy coordinates for the source and destination nodes
      int x1 = (vertex_to_coord.find(source)->second).first;
      int x2 = (vertex_to_coord.find(destination)->second).first;
      int y1 = (vertex_to_coord.find(source)->second).second;
      int y2 = (vertex_to_coord.find(destination)->second).second;

      //calls the helper function to calculate euclidean distance between the points
      double distance = calculate_distance((x1-x2), (y1-y2));
      //sets the weight of the edge to the distance between the source and destination
      routes_graph.setEdgeWeight(source, destination, distance);
    }
  }
}

/**
 * Helper function to calculate the Euclidean Distance
 * between two coordniate points.
 * Distance = sqrt(x^2 + y^2);
 */
double Project::calculate_distance(int x, int y) {
  double dist = sqrt(x * x + y * y);
  return dist;
}

/**
 * Function to project data onto the map.
 */
void Project::map_projection(const std::string &filename){
  Image background;

  //Reads in the PNG to output the airports and edges onto
  background.readFromFile("data/newmap.png");

  //Calls helper functions to draw edges and add airport markers
  std::cout << " -- Graphical Output Projection to plot airports and their connected routes -- " << std::endl;
  DrawEdgeOnMap(background,filename);
  DrawAirportMarkerOnMap(background,filename);
}


/**
 * Function to calculate a pair of coordinates to plot on the map
 * using latitude and longitude as parameter
 */
std::pair<double,double> Project::get_coordinates (double latitude, double longitude) {
  double temp_lat, temp_long;
  // Latitude increases to positive values above the equator while deceases to  
  // negative values below the equator which is found in the middle of the graph.  
  // The following code converts the latitude and longitude to work with PNG images  
  // which grow from the top-left corner.
  if (latitude >= 0) {
    temp_lat =  90 - latitude; //Positive latitudes scaled to start from zero (the value +90 is now 0).
  } else {
    temp_lat = 90 + abs(latitude); //Negative latitudes are offset to begin at positive values after 90.
  }
  
  temp_long = longitude + 180;
  // This offsets the longitiudes to begin at 0 instead of -180.
  double y_coord = (temp_lat/181) * map_image.height(); //Total latitudes are 180 + 1 for the equator.
  double x_coord = (temp_long/361) * map_image.width(); //Total longitudes are 360 + 1 for the prime meridian.

  return std::make_pair(x_coord, y_coord);
}

/**
 * Uses Edges of the routes graph to draw paths using distances as the edge
 * weights between connected airports as per the routes provided in the dataset.
 */
void Project::DrawEdgeOnMap(Image & backgroundImg, const std::string &filename) {
  //Vector to store all the edges of the graph created.
  std::vector<Edge> all_edges = routes_graph.getEdges();

  //Range-based for loop to access each each and retrive the xy coordinate pair
  //values from both source and destination vertices of the edges to plot edges onto the world map.
  for (Edge e : all_edges) {
    int x1 = (vertex_to_coord.find(e.source)->second).first;
    int x2 = (vertex_to_coord.find(e.dest)->second).first;
    int y1 = (vertex_to_coord.find(e.source)->second).second;
    int y2 = (vertex_to_coord.find(e.dest)->second).second;

    for (int i = x1; i < x2; i++) {
      double y_val = (((double) (y2 - y1)/ (double) (x2 - x1)) * abs((double) (i - x1))) + y1;
      int rounded = abs(round(y_val));
      unsigned int y_entry = rounded;
      unsigned int x_entry = i;
      if (abs(i) < (int) backgroundImg.width() && abs(rounded) < (int) backgroundImg.height()) {
        cs225::HSLAPixel & pixel = backgroundImg.getPixel(x_entry, y_entry);
        //changing HSLApixel values to pink.
        pixel.h = 306;
        pixel.s = 1;
        pixel.l = 0.6;
        pixel.a = 1;
      }
    }

    for (int i = x2; i < x1; i++) {
      double y_val = (((double) (y1 - y2)/ (double) (x1 - x2)) * abs((double) (i - x2))) + y2;
      int rounded = abs(round(y_val));
      unsigned int y_entry = rounded;
      unsigned int x_entry = i;
      if (abs(i) < (int) backgroundImg.width() && abs(rounded) < (int) backgroundImg.height()) {
        cs225::HSLAPixel & pixel = backgroundImg.getPixel(x_entry, y_entry);
        //changing HSLApixel values to pink.
        pixel.h = 306;
        pixel.s = 1;
        pixel.l = 0.6;
        pixel.a = 1;
      }
    }

  }
  //Writes the edited image onto an output file.
  backgroundImg.writeToFile(filename);
}

/**
 * Uses vertices in graph to plot airport coodinates onto our world map 
 * as a part of the Graphical Output Projection part of this project.
 */
void Project::DrawAirportMarkerOnMap(Image & backgroundImg,const std::string &filename) {
  //Vector to store all the vetices of the graph created.
  std::vector<Vertex> all_vertices = routes_graph.getVertices();

  //Range-based for loop to access each vertex and retrive the x and y coordinate
  //value to plot points on the world map.
  for (Vertex v : all_vertices) {
    int x = (vertex_to_coord.find(v)->second).first;
    int y = (vertex_to_coord.find(v)->second).second;
    //plotting the airport by making the marker 2 pixels in width and height.
    for (int i = x - 1; i < x + 1; i++) {
      for (int j = y - 1; j < y + 1; j++) {
        //check to see whether the points are within the boundaries of the world map.
        if (i >= 0 && i < (int) backgroundImg.width() && j >= 0 && j < (int) backgroundImg.height()) {
          cs225::HSLAPixel & pixelA = backgroundImg.getPixel(i, j);
          //changing HSLApixel values to yellow.
          pixelA.h = 42;
          pixelA.s = 1;
          pixelA.l = 0.6;
          pixelA.a = 1;
        }
      }
    }
  }
  //Writes the edited image onto an output file.
  backgroundImg.writeToFile(filename);
}

/**
  * The parse_csv method is used to create a map based on the information in the
  * datafiles - routes.txt and airports.txt by accessing the data present in the files
  * then editing and storing them in a map which maps each column as the key and the 
  * data as a vector of strings.
  */
std::map<int, std::vector<std::string>> Project::parse_csv (const std::string &fileToRead) {
  std::string current_line;
  std::string entry;
  std::map<int, std::vector<std::string>> toReturn;

  //reading in the file to parsed
  std::ifstream data_file (fileToRead);

  //checking if the file can be opened, if not then exit
  if (!data_file.is_open()) {
    std::cout << "File could not be opened, please try again" << std::endl;
    exit(1);
  }

  //Checks to see if none of the file's state flags are set.
  if (data_file.good()) {

    //Loop to access each line and store it in the current_line variable.
    while(getline(data_file, current_line)) {
      /**
       * The stringstream class was used for the insertion of data to our airport string.
       * It can be found at: http://www.cplusplus.com/reference/sstream/stringstream/
       */
      std::stringstream line_as_string(current_line);  //Stores the entire current line as a string
      
      //Counter is used to represent the number of csv values in each line (columns) in the datafile.
      int counter = 0;

      //Loop to access each part of the string as separated by the delimiter ","
      while(std::getline(line_as_string, entry, ',')) {

        //If there is no entry for a particular value, populate the map with an empty string
        //to ensure correct indices.
        if (entry.empty() == true) {
          //add the entry to our vector
          toReturn[counter].push_back(entry);
          counter++;
          continue;
        }

        //This checks to see if quotation marks present around data:

        //We found that some airport names have a comma as a part of its name which it to read as two attributes.
        //To only identify the commas we desire, checking to see that if the first character of the
        //attribute's string is a " but the last character is not (implyning that this entry 
        //has a comma in the airport name), we need to ignore this comma.
        if (entry[0] == '"') {
          if (entry[entry.size() - 1] != '"') {
            std::string temporary =  entry + ",";
            std::getline(line_as_string, entry, ',');
            temporary += entry;
            //This allows us to push back the entire airport name as one entry after 
            //removing the double quotes from the start and end of the string.
            temporary.erase(std::remove(temporary.begin(), temporary.end(), '\"'), temporary.end());
            //add the entry to our vector
            toReturn[counter].push_back(temporary);
            counter++;
            continue;
          }
        }

        // For correctly formatted entries that do not contain a "," in the airport name, 
        // we can direct remove double quotes from start and end of the string and stoer it.
        entry.erase(std::remove(entry.begin(), entry.end(), '\"'), entry.end());
        //add the entry to our vector
        toReturn[counter].push_back(entry);
        counter++;
      }
    }
  }
  return toReturn;
}