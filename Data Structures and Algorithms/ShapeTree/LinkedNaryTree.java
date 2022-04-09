/*
 * Assignment 4
 * LinkedNaryTree class
 * 
 * Main tree that has the starting root. Can iterative preorder.
 */

import java.util.Iterator;

public class LinkedNaryTree<T> {
	private NaryTreeNode<T> root; // Root node
	
	public LinkedNaryTree() { root = null; } // Initalize root to null
	public LinkedNaryTree(NaryTreeNode<T> r) { root = r; } // Initialize root to a node
	
	public void addNode(NaryTreeNode<T> parent, NaryTreeNode<T> child) { parent.addChild(child); } // Add a child to the parent using addChild()
	
	public NaryTreeNode<T> getRoot() { return root; } // Return root
	public T getRootElement() { return (T)root.getData(); } // Return root's data
	
	public boolean isEmpty() { // Is it empty?
		if (root == null) return true;
		else return false;
	}
	
	public int size(NaryTreeNode<T> node) { // Returns amount of nodes starting from a specified node
		int childs = node.getNumChildren() + 1;
		int ran = 0;
		
		LinkedNaryTree<T> base = new LinkedNaryTree<T>(node);
		for (int i = 0; i != node.getNumChildren(); i++) {
			childs += base.size(node.getChild(i));
			ran++;
		}
		
		childs -= ran; // Workaround for the additional parent node (+1) that we need
		
		return childs;
	}
	
	public void preorder(NaryTreeNode<T> node, ArrayUnorderedList<T> list) { // Preorder searches nodes and adds to a list
		if (node != null) {
			list.addToRear(node.getData());
			
			LinkedNaryTree<T> base = new LinkedNaryTree<T>(node);
			for (int i = 0; i != node.getNumChildren(); i++) {
				base.preorder(node.getChild(i), list);
			}
		}
	}
	
	public Iterator<T> iteratorPreorder() { // Iterate through and preorder
		ArrayUnorderedList<T> list = new ArrayUnorderedList<T>();
		preorder(root, list);
		
		return list.iterator();
	}
	
	public String toString() { // Print stuff from the nodes inside the tree
		String s = "";
		Iterator<T> i = iteratorPreorder();
		
		while (i.hasNext()) s += i.next() + "\n";
		return (s == "") ? "Tree is empty." : s;
	}
}
