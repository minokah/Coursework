/*
    Assignment 5
    Graph class: graph that can connect nodes using edges

    Adjacency Matrix used
 */

import java.util.ArrayList;

public class Graph implements GraphADT {
    private Node[] nodes; // Nodes
    private Edge[][] edges; // Adjacency Matrix

    Graph(int n) {
        nodes = new Node[n];
        for (int i = 0; i != n; i++) nodes[i] = new Node(i); // populate the node graph with n nodes
        edges = new Edge[n][n]; // initialize empty edge list
    }

    // insert an edge, will throw exception if node does not exist
    public void insertEdge(Node u, Node v, int edgeType) throws GraphException {
        if (nodes.length == 0) throw new GraphException("There are no nodes in the graph");
        getNode(u.getName());
        getNode(v.getName());
        edges[u.getName()][v.getName()] = new Edge(u, v, edgeType);
        edges[v.getName()][u.getName()] = new Edge(v, u, edgeType);
    }

    // get a node, will throw exception if node does not exist
    public Node getNode(int name) throws GraphException {
        if (nodes.length == 0) throw new GraphException("There are no nodes in the graph");
        try { return nodes[name]; }
        catch (Exception e) { throw new GraphException("Node " + name + " does not exist"); }
    }

    // get a list of incident edges on a node
    public ArrayList<Edge> incidentEdges(Node u) throws GraphException {
        if (nodes.length == 0) throw new GraphException("There are no nodes in the graph");
        getNode(u.getName()); // try getting node u if it exists

        ArrayList<Edge> list = new ArrayList();
        for (int y = 0; y != edges.length; y++) { if (edges[u.getName()][y] != null) list.add(edges[u.getName()][y]); }
        if (list.size() == 0) throw new GraphException("There are no incident edges for node " + u.getName());
        else return list;
    }

    // get an edge (if u and v exist)
    public Edge getEdge(Node u, Node v) throws GraphException {
        if (nodes.length == 0) throw new GraphException("There are no nodes in the graph");
        getNode(u.getName()); // try getting nodes u and v if they exist
        getNode(v.getName());

        if (edges[u.getName()][v.getName()] != null) return edges[u.getName()][v.getName()];
        else throw new GraphException("There is no edge connecting nodes " + u.getName() + " and " + v.getName());
    }

    // check if two nodes are adjacent or not
    public boolean areAdjacent(Node u, Node v) throws GraphException {
        if (nodes.length == 0) throw new GraphException("There are no nodes in the graph");
        getNode(u.getName()); // try getting nodes u and v if they exist
        getNode(v.getName());

        if (edges[u.getName()][v.getName()] == null) return false;
        else return true;
    }
}
