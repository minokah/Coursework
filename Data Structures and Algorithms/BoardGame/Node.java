/*
    Assignment 2

    Node LinkedList to store data in hash table
*/

public class Node {
    private Layout data;
    private Node next;

    public Node(Layout data) {
        this.data = data;
        next = null;
    }

    public void setNext(Node next) {
        this.next = next;
    }

    public Node getNext() {
        return next;
    }

    public Layout getData() {
        return data;
    }
}
