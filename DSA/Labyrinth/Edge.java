/*
    Assignment 5
    Edge class: represents an edge connecting two nodes
 */

public class Edge {
    private Node firstEndpoint, secondEndpoint;
    private int type;

    Edge(Node u, Node v, int edgeType) {
        type = edgeType;
        firstEndpoint = u;
        secondEndpoint = v;
    }

    public Node firstEndpoint() { return firstEndpoint; } // get the first point
    public Node secondEndpoint() { return secondEndpoint; } // get the second point
    public int getType() { return type; } // get the wall type
    public void setType(int newType) { type = newType; } // set the wall type
    public boolean equals(Edge otherEdge) { // check BOTH ways to see if the node is equal
        boolean first = firstEndpoint.getName() == otherEdge.firstEndpoint().getName() && secondEndpoint.getName() == otherEdge.secondEndpoint().getName();
        boolean second = secondEndpoint.getName() == otherEdge.firstEndpoint().getName() && firstEndpoint.getName() == otherEdge.secondEndpoint().getName();
        return (first || second);
    }
}
