// BSTNode, a node in a BST
public class BSTNode {
    private BSTNode parent;
    private BSTNode leftChild;
    private BSTNode rightChild;
    private NodeData data;

    // empty constructor
    public BSTNode() {
        parent = null;
        leftChild = null;
        rightChild = null;
        data = null;
    }

    // constructor, set the instance variables to the data
    public BSTNode(BSTNode newParent, BSTNode newLeftChild, BSTNode newRightChild, NodeData newData) {
        parent = newParent;
        leftChild = newLeftChild;
        rightChild = newRightChild;
        data = newData;
    }

    // get parent, left child, right child, or data
    public BSTNode getParent() { return parent; }
    public BSTNode getLeftChild() { return leftChild; }
    public BSTNode getRightChild() { return rightChild; }
    public NodeData getData() { return data; }

    // set parent, left child, right child, data
    public void setParent(BSTNode newParent) { parent = newParent; }
    public void setLeftChild(BSTNode newLeftChild) { leftChild = newLeftChild; };
    public void setRightChild(BSTNode newRightChild) { rightChild = newRightChild; }
    public void setData(NodeData newData) { data = newData; }
    public boolean isLeaf() { return (leftChild == null && rightChild == null); } // return if it is a leaf or not
}
