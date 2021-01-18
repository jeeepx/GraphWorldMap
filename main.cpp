#include "Project.h"
#include <string>

int main() {

  // This produces a projection of all the airports in our data file onto an image of a world map
  Project openFlights;
  int ch;

  //Ask the input for our data files to pass to buildGraph
  std::string airport_input;
  std::cout << "" << std::endl;
  std::cout << "Input path to your airport CSV file: " << std::endl;
  std::cin >> airport_input;
  std::string routes_input;
  std::cout << "" << std::endl;
  std::cout << "Input path to your routes CSV file: " << std::endl;
  std::cin >> routes_input;

  // creates a new graph from text files passed in
  openFlights.buildGraph(airport_input, routes_input);

  // Testing file input edge cases
  // Checking the 10th coumn as routes csv file can only contain 7-8 columns, 
  //to ensure it is an airports csv file passed in0
  std::cout<< "" <<std::endl;

  if (openFlights.airports_map[10].size() == 0) {
    std::cout << "Please check your inputted airports csv file to ensure it is accurately formatted and contains airports." << std::endl;
    exit(1);
  } 
  else if (openFlights.routes_map[0].size() == 0) {    
    std::cout << "Please check your input routes csv file to ensure it is accurately formatted and contains routes." << std::endl;
    exit(1);
  } 
  else {
    std::cout << "Both airports and routes file have been read correctly" << std::endl;
  }
 
  // to get inputs for source and destination airports
  std::cout << "" << std::endl;
  std::cout << "Input source airport code: " << std::endl;
  Vertex sourceNode = openFlights.getUserInput();
  std::cout << " " << std::endl;
  std::cout << "Input destination airport code: " << std::endl;
  Vertex destinationNode = openFlights.getUserInput();

  do {
    std::cout << "" << std::endl;
    std::cout << "******************************************" << std::endl;
    std::cout << "*   Choose which algorithm to run:       *" << std::endl;
    std::cout << "*   1. BFS Traversal                     *" << std::endl;
    std::cout << "*   2. Dijkstra's Algorithm              *" << std::endl;
    std::cout << "*   3. Graphical Output Projection       *" << std::endl;
    std::cout << "*   4. Run All Three Algorithms          *" << std::endl;
    std::cout << "*   5. Exit this program                 *" << std::endl;
    std::cout << "******************************************" << std::endl;
    std::cout << "Enter the choice's index number to run: "<< std::endl;

    //take in user input for choice
    std::cin >> ch;

    // to run different algorithms for each case based on user's choice
    switch(ch)
    {
      case 1: { // BFS function to calculate minimum flights between the nodes passed in
                openFlights.BFS(sourceNode, destinationNode);
                 break;
              }
      case 2: { // Dijkstras function to calculate minimum distance between the nodes passed in
                std::cout << " " << std::endl;
                std::cout << "Kindly allow time for Dijkstra's Algorithm to run." << std::endl;
                openFlights.DijkstrasAlgorithm(sourceNode, destinationNode); 
                break;
              }
      case 3: { // Plots airports and routes between them onto world map as part of Graphical output projection
                openFlights.map_projection("output-worldmap.png");
                std::cout << "Map can be viewed at output-worldmap.png" << std::endl;
                std::cout << "" << std::endl;
                break;               
              }
      case 4: { // Runs all of our three algorithms
                openFlights.map_projection("output-worldmap.png");
                std::cout<< ""<< std::endl;
                std::cout << "Map can be viewed at output-worldmap.png" << std::endl;
                std::cout << "" << std::endl;
                openFlights.BFS(sourceNode, destinationNode);
                std::cout << " " << std::endl;
                std::cout << "Kindly allow time for Dijkstra's Algorithm to run." << std::endl;
                openFlights.DijkstrasAlgorithm(sourceNode, destinationNode);
                break;
              }
      case 5: { // to exit the program once you have finished running chosen algorithms
                std::cout << "-- Exiting the program --" << std::endl;
                exit(1);
              }
      default: std::cout<< "Invalid choice number entered. Enter a number between 1-5" << std::endl;
    } 
  }
  while(ch != 5);
  
  return 0;
}