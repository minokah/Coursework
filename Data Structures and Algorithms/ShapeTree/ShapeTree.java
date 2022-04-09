/*
 * Assignment 4
 * ShapeTree class
 * 
 * Tree of various shapes with various colours. Adding them links them according
 * to several rules.
 */

public class ShapeTree {
	private LinkedNaryTree<Shape> tree; // Treeeeeeee
	
	public LinkedNaryTree<Shape> getTree() { return tree; } // Get the tree
	public NaryTreeNode<Shape> getRoot() { return tree.getRoot(); } // Get the root
	
	public void addShapeNode(NaryTreeNode<Shape> node, Shape shape) { tree.addNode(node, new NaryTreeNode<Shape>(shape)); } // Adds a shape to the tree from the node
	
	public void addShapeNode(Shape shape) { // Find the best possible node to add a shape.
		NaryTreeNode<Shape> s = new NaryTreeNode<Shape>(shape);
		if (tree == null || tree.isEmpty()) tree = new LinkedNaryTree<Shape>(s); // Grow a tree if it doesn't exist yet
		else {
			NaryTreeNode<Shape> spot = addShapeNodeHelper(shape); // Attempt to find a spot to put the next node
			if (spot != null) spot.addChild(s);
		}
	}

	public NaryTreeNode<Shape> addShapeNodeHelper(Shape shape) { // Attempt to find the next best node to put a shape
		if (getRoot() == null) return null;
		
		if (checkNode(getRoot(), shape)) return getRoot();
		
		ArrayStack<NaryTreeNode<Shape>> stack = new ArrayStack<NaryTreeNode<Shape>>();
		stack.push(getRoot()); // Start with the root
		
		while (!stack.isEmpty()) { // Pop and traverse through the children via stack, do the loop
			NaryTreeNode<Shape> node = stack.pop();
			for (int i = 0; i != node.getNumChildren(); i++) {
				if (checkNode(node.getChild(i), shape)) return node.getChild(i);
				stack.push(node.getChild(i));
			}
		}
		
		return null;
	}
	
	public boolean checkNode(NaryTreeNode<Shape> node, Shape shape) { // Checks a node against the rules to see if the shape is good to add
		if (node.getData().getNumSides() == node.getNumChildren()) {
			return false; // Rule 1
		}
		if (node.getData().getColour() == shape.getColour()) {
			return false; // Rule 2
		}
		for (int i = 0; i != node.getNumChildren(); i++) { // Rule 4
			if (node.getChild(i).getData().getColour() == shape.getColour()) {
				return false; // Rule 3
			}
		}
		
		return true;
	}
	
	public String toString() { return tree.toString(); } // Do trees grow words?
}
