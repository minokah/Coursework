/*
 * Assignment 4
 * NaryTreeNode class
 * 
 * Tree node. Can contain any amount of children.
 */


public class NaryTreeNode<T> {
	private T data; // Data
	private int numChildren; // Number of children
	private NaryTreeNode<T>[] children; // The children
	
	public NaryTreeNode(T d) { // Initialize above variables
		data = d;
		children = null;
		numChildren = 0;
	}
	
	public void addChild(NaryTreeNode<T> child) { // Add a child node to the array
		if (children == null) children = (NaryTreeNode<T>[])new NaryTreeNode[3];
		else if (children.length == numChildren) expandCapacity();
		children[numChildren] = child;
		numChildren++;
	}
	
	public void expandCapacity() { // Expand me
		NaryTreeNode<T>[] temp = (NaryTreeNode<T>[])new NaryTreeNode[children.length + 3];
		for (int i = 0; i != children.length; i++) temp[i] = children[i];
		children = temp;
	}
	
	public int getNumChildren() { return numChildren; } // Returns number of children
	public NaryTreeNode<T> getChild(int i) { return children[i]; } // Pick out a specific child
	public T getData() { return data; } // Get the data
	public String toString() { return "Node containing: " + data; } // What am I?
}
