/*
    Assignment 5
    Solver class: creates the labyrinth graph and then solves it
 */

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Iterator;
import java.util.ArrayList;

public class Solver {
    private Graph graph;

    // set up instance variables to store things while loading the file
    private int blast = 0;
    private int melt = 0;
    private Node entrance = null;
    private Node exit = null;
    private ArrayList stack = null;

    public Solver(String inputFile) throws LabyrinthException {
        try {
            stack = new ArrayList();

            // set up reader and read first few lines
            BufferedReader reader = new BufferedReader(new FileReader(inputFile));
            reader.readLine(); // scale factor
            int width = Integer.parseInt(reader.readLine());
            int height = Integer.parseInt(reader.readLine());
            graph = new Graph(width * height);
            blast = Integer.parseInt(reader.readLine()); // blast bombs
            melt = Integer.parseInt(reader.readLine()); // melt bombs

            System.out.println("Blast bombs: " + blast);
            System.out.println("Melts bombs: " + melt);

            // begin reading labyrinth
            String nodeLine = reader.readLine();
            String connectLine = reader.readLine();
            int currentNode = -1;
            int currentLink = -1;

            /*
            – 1: corridor
            – 2: brick wall
            – 3: rock wall
            – 4: metal wall
             */
            while (nodeLine != null) {
                // read the node line (ie. emo*omo)
                for (int i = 0; i != nodeLine.length(); i += 2) {
                    currentNode++;
                    if (i + 1 == nodeLine.length()) break;

                    char wall = nodeLine.charAt(i + 1);
                    int wallType = 1; // corridor
                    if (Character.toLowerCase(wall) == 'b') wallType = 2; // brick
                    if (Character.toLowerCase(wall) == 'r') wallType = 3; // rock
                    if (Character.toLowerCase(wall) == 'm') wallType = 4; // metal

                    if (nodeLine.charAt(i + 1) != '*') { // if not solid wall, link
                        //System.out.println("row " + currentNode + " - " + (currentNode + 1));
                        graph.insertEdge(graph.getNode(currentNode), graph.getNode(currentNode + 1), wallType);
                        graph.insertEdge(graph.getNode(currentNode + 1), graph.getNode(currentNode), wallType);
                    }

                    if (nodeLine.charAt(i) == 'e') entrance = graph.getNode(currentNode); // set entrance
                    if (nodeLine.charAt(i) == 'x') exit = graph.getNode(currentNode); // set exit
                }

                if (connectLine == null) break; // kill if we've reached the last node line

                // read the "connection" line (ie. |*|*|*|)
                for (int i = 0; i != connectLine.length(); i += 2) {
                    currentLink++;
                    if (i + 1 == connectLine.length()) break;

                    char wall = connectLine.charAt(i);
                    int wallType = 1; // corridor
                    if (Character.toLowerCase(wall) == 'b') wallType = 2; // brick only 1
                    if (Character.toLowerCase(wall) == 'r') wallType = 3; // rock REQUIRES 2
                    if (Character.toLowerCase(wall) == 'm') wallType = 4; // metal melt

                    //System.out.println("column " + currentLink + " - " + (currentLink + width));

                    if (connectLine.charAt(i) != '*') { // if not solid wall, link
                        graph.insertEdge(graph.getNode(currentLink), graph.getNode(currentLink + width), wallType);
                        graph.insertEdge(graph.getNode(currentLink + width), graph.getNode(currentLink), wallType);
                    }
                }

                nodeLine = reader.readLine();
                connectLine = reader.readLine();
            }
        }
        catch (FileNotFoundException e) { throw new LabyrinthException("Failed to find file"); }
        catch (IOException e) { System.out.println("Failed to read file"); }
        catch (GraphException e) { System.out.println(e); }
    }

    public Graph getGraph() { return graph; } // return the graph

    // DFS code to solve the labyrinth
    private boolean DFS(Node node, int blasts, int melts) {
        try {
            stack.add(node); // add to stack
            node.setMark(true);
            //System.out.println("traverse: " + node.getName());

            if (node.getName() == exit.getName()) { // exit found, return true
                //System.out.println("blasts/melts left: " + blasts + " " + melts);
                return true; // found exit!
            }

            ArrayList<Edge> adjacent = graph.incidentEdges(node);
            for (int i = 0; i != adjacent.size(); i++) { // for all the edges
                Edge a = adjacent.get(i);
                if (a.secondEndpoint().getMark() == false) { // if second point is not marked, visit it
                    if (a.getType() == 2) {
                        if (blasts >= 1) {
                            if (DFS(a.secondEndpoint(), blasts - 1, melts)) return true; // check brick
                        }
                    }
                    else if (a.getType() == 3) {
                        if (blasts >= 2) {
                            if (DFS(a.secondEndpoint(), blasts - 2, melts)) return true; // check rock
                        }
                    }
                    else if (a.getType() == 4) {
                        if (melts >= 1) {
                            if (DFS(a.secondEndpoint(), blasts, melts - 1)) return true; // check metal
                        }
                    }
                    else if (DFS(a.secondEndpoint(), blasts, melts)) return true; // corridor
                }
            }

            // dead end, bo back :(
            node.setMark(false); // unmark and remove from the stack
            stack.remove(stack.size() - 1);
            //System.out.println("backtrack: " + node.getName());
        }
        catch (GraphException e) { System.out.println(e); }
        return false; // cannot go this way
    }

    // call this to solve the labyrinth
    public Iterator solve() {
        if (DFS(entrance, blast, melt)) {
            System.out.println("Solution found!");
            return stack.iterator();
        }
        else {
            //System.out.println("failed to find solution");
            return null;
        }
    }
}
