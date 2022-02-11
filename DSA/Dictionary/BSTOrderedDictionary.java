import java.util.ArrayList;


// BSTOrderedDictionary, implementation of a dictionary using a BST
public class BSTOrderedDictionary implements BSTOrderedDictionaryADT {
    private BSTNode root; // root
    private int numInternalNodes; // number of internal nodes

    // constructor
    public BSTOrderedDictionary() {
        root = new BSTNode();
        numInternalNodes = 0;
    }

    public BSTNode getRoot() { return root; } // get root
    public int getNumInternalNodes() { return numInternalNodes;  } // get num nodes

    // brute force and search for the node
    public BSTNode find(BSTNode r, String key) {
        if (r.getData() == null) return null;
        else if (r.getData().getName().equals(key)) return r;
        else {
            BSTNode leftResult = null;
            BSTNode rightResult = null;

            if (r.getLeftChild() != null) leftResult = find(r.getLeftChild(), key);
            if (r.getRightChild() != null) rightResult = find(r.getRightChild(), key);

            if (leftResult == null && rightResult == null) return null;
            else if (leftResult != null) return leftResult;
            else return rightResult;
        }
    }

    // find the node, get the arraylist for the data
    public ArrayList<MultimediaItem> get(BSTNode r, String key) {
        BSTNode list = find(r, key);
        if (list == null) return null;
        else return list.getData().getMedia();
    }

    // put a new node into the tree, or add to the data if it already exists
    public void put(BSTNode r, String name, String content, int type) {
        if (r == root && r.getData() == null) { // root empty, put into the root
            root = new BSTNode(null, new BSTNode(), new BSTNode(), new NodeData(name));
            root.getLeftChild().setParent(root);
            root.getRightChild().setParent(root);
            root.getData().add(new MultimediaItem(content, type));
            numInternalNodes++;
        }
        else { // traversal
            ArrayList<MultimediaItem> array = get(r, name);
            if (array != null) array.add(new MultimediaItem(content, type)); // found, put it in the array
            else { // not found, insert brand new node somewhere
                BSTNode spot = r;
                BSTNode newNode = null;

                while (spot.getData() != null) {
                    if (spot.getData().getName().compareTo(name) < 0) { // name larger, right
                        //System.out.println(name + " -> (right) " + spot.getData().getName());
                        if (spot.getRightChild().getData() == null) { // goes to the right
                            newNode = new BSTNode(spot, new BSTNode(), new BSTNode(), new NodeData(name));
                            newNode.getLeftChild().setParent(newNode);
                            newNode.getRightChild().setParent(newNode);
                            spot.setRightChild(newNode);
                            numInternalNodes++;
                            break;
                        }
                        spot = spot.getRightChild(); // traverse right
                    }
                    else if (spot.getData().getName().compareTo(name) > 0) { // name is smaller, left
                        //System.out.println(name + " -> (left) " + spot.getData().getName());
                        if (spot.getLeftChild().getData() == null) { // goes to the left
                            newNode = new BSTNode(spot, new BSTNode(), new BSTNode(), new NodeData(name));
                            newNode.getLeftChild().setParent(newNode);
                            newNode.getRightChild().setParent(newNode);
                            spot.setLeftChild(newNode);
                            numInternalNodes++;
                            break;
                        }
                        spot = spot.getLeftChild(); // traverse left
                    }
                }

                if (type != -1) newNode.getData().add(new MultimediaItem(content, type)); // add the node if the type is not -1
            }
        }
    }

    // remove a node from the tree completely
    public void remove(BSTNode r, String key) throws DictionaryException {
        BSTNode node = find(r, key);

        if (node == null || (node.getLeftChild() == null && node.getRightChild() == null)) throw new DictionaryException("There is no key " + key + " in the dictionary"); // doesn't exist
        else if (node.getLeftChild().isLeaf() && node.getRightChild().isLeaf()) { // leaf node, wipe it
            node.setData(null);
            node.setLeftChild(null);
            node.setRightChild(null);
        }
        else { // internal node
            BSTNode p2 = node.getParent();
            BSTNode c2 = null;
            if ((node.getLeftChild().isLeaf() && !node.getRightChild().isLeaf()) || (node.getRightChild().isLeaf() && node.getLeftChild().isLeaf())) {
                if (node.getLeftChild().isLeaf() && !node.getRightChild().isLeaf()) c2 = node.getRightChild(); // left leaf, use right node
                else if (node.getRightChild().isLeaf() && !node.getLeftChild().isLeaf()) c2 = node.getLeftChild(); // right leaf, use right node

                if (p2 == null) root = c2; // move up the node to become the new root
                else {
                    if (p2.getLeftChild() == node) p2.setLeftChild(c2);
                    else if (p2.getRightChild() == node) p2.setRightChild(c2);
                }
            }
            else { // get largest node on the left branch, or smallest on the right
                NodeData right = smallest(node.getRightChild());
                NodeData left = largest(node.getLeftChild());
                BSTNode current = null;
                if (right != null) current = find(r, smallest(node.getRightChild()).getName());
                else if (left != null) current = find(r, largest(node.getLeftChild()).getName());

                node.setData(current.getData());
                remove(current, current.getData().getName());
            }
        }

        numInternalNodes--;
    }

    // remove a multimedia item from an arraylist
    private int removeMultimediaItem(ArrayList<MultimediaItem> list, int type)  {
        for (int i = 0; i != list.size() - 1; i++) {
            if (list.get(i).getType() == type) {
                list.remove(list.get(i));
                break;
            }
        }

        return list.size();
    }

    // remove data from a node, delete the node if it is now empty
    public void remove(BSTNode r, String key, int type) throws DictionaryException {
        BSTNode node = find(r, key);
        if (removeMultimediaItem(node.getData().getMedia(), type) == 0) remove(r, key);
    }

    // get the smallest node starting from a node
    public NodeData smallest(BSTNode r) {
        if (r == null || r.isLeaf()) return null;
        else {
            BSTNode node = r;
            while (!node.isLeaf()) {
                node = node.getLeftChild();
            }
            return node.getParent().getData();
        }
    }

    // get the largest node starting from a node
    public NodeData largest(BSTNode r) {
        if (r == null || r.isLeaf()) return null;
        else {
            BSTNode node = r;
            while (!node.isLeaf()) {
                node = node.getRightChild();
            }
            return node.getParent().getData();
        }
    }

    // get the successor of a node if it exists
    public NodeData successor(BSTNode r, String key) {
        BSTNode p = find(r, key);
        if (!p.getRightChild().isLeaf()) return smallest(p.getRightChild());
        else {
            BSTNode p2 = p.getParent();
            while (p2 != null && p == p2.getRightChild()) {
                System.out.println(p2.getData().getName());
                p = p2;
                p2 = p2.getParent();
            }
            if (p2 != null) return p2.getData();
            return null;
        }
    }

    // get the predecessor of a node if it exists
    public NodeData predecessor(BSTNode r, String key) {
        BSTNode p = find(r, key);
        if (!p.getLeftChild().isLeaf()) return largest(p.getLeftChild());
        else {
            BSTNode p2 = p.getParent();
            while (p2 != null && p == p2.getLeftChild()) {
                p = p2;
                p2 = p2.getParent();
            }
            if (p2 != null) return p2.getData();
            return null;
        }
    }
}
