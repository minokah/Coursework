/*
    Assignment 5
    Node class: represents a node in the graph
 */

public class Node {
    private int name;
    private boolean marked = false;

    Node(int nodeName) { name = nodeName; }
    public void setMark(boolean mark) { marked = mark; } // set mark state
    public boolean getMark() { return marked; } // get mark state
    public int getName() { return name; } // get the name
    public boolean equals(Node otherNode) { return name == otherNode.getName(); } // check if two nodes are equal
}
