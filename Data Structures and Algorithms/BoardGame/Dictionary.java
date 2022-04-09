/*
    Assignment 2

    Dictionary using a hash table
*/

public class Dictionary implements DictionaryADT {
    private Node[] hash_table;
    private int hashX = 83; // change as necessary
    private int collisions = 0; // count collisions

    public Dictionary(int size) { // create dictionary
        hash_table = new Node[size];
        for (int i = 0; i != size; i++) hash_table[i] = null;
    }

    private int hashFunction(String s) { // hashing function (hasing + compression map)
        int value = s.charAt(0);

        for (int i = 1; i != s.length(); i++) {
            value = value % hash_table.length;
            value = value * hashX + (int)s.charAt(i);
        }
        value = value % hash_table.length;

        return value;
    }

    public int put(Layout data) throws DictionaryException { // puts a Layout object into dictionary
        int hashResult = hashFunction(data.getBoardLayout());

        if (hash_table[hashResult] != null) {
            Node next = hash_table[hashResult];

            while (next != null) { // loop through nodes
                if (next.getData().getBoardLayout() == data.getBoardLayout()) throw new DictionaryException();
                if (next.getNext() == null) {
                    next.setNext(new Node(data));
                    break;
                }

                next = next.getNext();
            }

            collisions += 1;
            //System.out.println("Collisions: " + collisions); // print collisions

            return 1;
        }
        else { // insert as first position
            hash_table[hashResult] = new Node(data);
            return 0;
        }
    }

    public void remove(String boardLayout) throws DictionaryException { // remove object from hash table
        int hashResult = hashFunction(boardLayout);

        boolean found = false;
        Node next = hash_table[hashResult];

        if (next == null) throw new DictionaryException();
        if (next.getData().getBoardLayout().equals(boardLayout)) {
            hash_table[hashResult] = next.getNext();
            return;
        }

        while (next != null) {
            if (next.getNext() != null) {
                if (next.getNext().getData().getBoardLayout().equals(boardLayout)) {
                    found = true;
                    next.setNext(next.getNext().getNext()); // reattach nodes
                    break;
                }
            }

            next = next.getNext();
        }

        if (!found) throw new DictionaryException();
    }

    public int getScore(String boardLayout) { // get the score
        int hashResult = hashFunction(boardLayout);
        Node next = hash_table[hashResult];

        //if (next.getData().getBoardLayout() == boardLayout) return next.getData().getScore();

        while (next != null) {
            if (next.getData().getBoardLayout().equals(boardLayout)) return next.getData().getScore();
            next = next.getNext();
        }

        return -1;
    }
}
