import java.io.*;
import java.util.ArrayList;

// Query, similar to a command prompt to find and play files among other things
public class Query {
    public static void main(String[] args) {
        try {
            if (args.length < 1) throw new FileNotFoundException("Your must provide a text file");
            else if (!args[0].endsWith(".txt")) throw new FileNotFoundException("You must provide a text file");

            BufferedReader reader = new BufferedReader(new FileReader(args[0]));
            StringReader keyboard = new StringReader();
            BSTOrderedDictionary dictionary = new BSTOrderedDictionary();

            // read args[0] for the file, store it in the tree
            while (true) {
                String name = reader.readLine();

                if (name == null) break;
                else name = name.toLowerCase();

                String content = reader.readLine().toLowerCase();
                //System.out.println("read " + name + ": " + content);

                int type = 1;
                if (content.endsWith(".wav") || content.endsWith(".mid")) type = 2;
                if (content.endsWith(".jpg") || content.endsWith(".gif")) type = 3;
                if (content.endsWith(".html")) type = 4;

                dictionary.put(dictionary.getRoot(), name, content, type);
            }

            SoundPlayer player = new SoundPlayer();
            PictureViewer viewer = new PictureViewer();
            ShowHTML browser = new ShowHTML();

            // continually ask for a prompt
            while (true) {
                String line = keyboard.read("Enter next command: ");
                String[] command = line.split(" ");

                switch (command[0]) {
                    case "get": // get a node's items and play everything
                        ArrayList<MultimediaItem> a = dictionary.get(dictionary.getRoot(), command[1]);
                        if (a == null) { // doesn't exist
                            System.out.println(command[1] + " is not in the ordered dictionary");

                            dictionary.put(dictionary.getRoot(), command[1], "", 0);

                            NodeData pre = dictionary.predecessor(dictionary.getRoot(), command[1]);
                            NodeData fol = dictionary.successor(dictionary.getRoot(), command[1]);

                            if (pre != null) System.out.println("Preceding word: " + pre.getName());
                            else System.out.println("Preceding word: ");
                            if (fol != null) System.out.println("Following word: " + fol.getName());
                            else System.out.println("Following word: ");

                            dictionary.remove(dictionary.getRoot(), command[1]);
                        }
                        else { // play it
                            for (int i = 0; i != a.size(); i++) {
                                MultimediaItem item = a.get(i);
                                switch (item.getType()) {
                                    case 1:
                                        System.out.println(item.getContent());
                                        break;
                                    case 2:
                                        player.play(item.getContent());
                                        break;
                                    case 3:
                                        viewer.show(item.getContent());
                                        break;
                                    case 4:
                                        browser.show(item.getContent());
                                        break;
                                }
                            }
                        }
                        break;
                    case "remove": // remove a node from the tree
                        if (dictionary.get(dictionary.getRoot(), command[1]) == null) System.out.println("No record in the ordered dictionary chas key " + command[1]);
                        else {
                            if (command[2] != null) { // remove a specific item
                                int num = Integer.parseInt(command[2]);
                                if (num >= 0 && num <= 4) {
                                    dictionary.remove(dictionary.getRoot(), command[1], num);
                                    System.out.println("Removed " + command[2] + " from " + command[1]);
                                }
                                else System.out.println("That is not a valid input type");
                            }
                            else { // or remove the entire thing
                                dictionary.remove(dictionary.getRoot(), command[1]);
                                System.out.println("Removed " + command[1]);
                            }
                        }
                        break;
                    case "add": {// add a node to the tree
                        if (command[3] != null) {
                            int num = Integer.parseInt(command[3]);
                            if (num < 0 || num > 4) {
                                System.out.println("You must input a type from 0-4"); // only allow types 0-4 into the arraylist
                                continue;
                            }

                        }
                        else if (dictionary.get(dictionary.getRoot(), command[1]) == null) dictionary.put(dictionary.getRoot(), command[1], "", -1); // doesn't exist

                        dictionary.put(dictionary.getRoot(), command[1], command[2], Integer.parseInt(command[3])); // finally add it

                        break;
                    }
                    case "next": { // get the next n successors of a node
                        ArrayList<MultimediaItem> node = dictionary.get(dictionary.getRoot(), command[1]);
                        String key = command[1];
                        boolean append = false;

                        if (command.length < 3) {
                            System.out.println("You must enter an integer for next");
                            break;
                        }

                        int amount = Integer.parseInt(command[2]);

                        if (node == null) { // temporarily add a node to get its successor
                            dictionary.put(dictionary.getRoot(), command[1], "", 0);
                            append = true;
                        }

                        NodeData successor = dictionary.successor(dictionary.getRoot(), key);
                        if (successor == null) System.out.println("There are no keys larger than or equal to " + command[1]); // doesn't exist
                        else {
                            while (amount > 0 && successor != null) {
                                key = successor.getName();
                                System.out.print(key + " ");
                                successor = dictionary.successor(dictionary.getRoot(), key);
                            }
                            System.out.println();
                        }

                        if (append) dictionary.remove(dictionary.getRoot(), command[1]); // remove fake node

                        break;
                    }
                    case "prev": { // get the previous n predecessors of a node
                        ArrayList<MultimediaItem> node = dictionary.get(dictionary.getRoot(), command[1]);
                        String key = command[1];
                        boolean append = false;

                        if (command.length < 3) {
                            System.out.println("You must enter an integer for next");
                            break;
                        }

                        int amount = Integer.parseInt(command[2]);

                        if (node == null) { // temporarily add a node to get its predecessor
                            dictionary.put(dictionary.getRoot(), command[1], "", 0);
                            append = true;
                        }

                        NodeData predecessor = dictionary.predecessor(dictionary.getRoot(), key);
                        if (predecessor == null) System.out.println("There are no keys less than or equal to " + command[1]); // doesn't exist
                        else {
                            while (amount > 0 && predecessor != null) {
                                key = predecessor.getName();
                                System.out.print(key + " ");
                                predecessor = dictionary.predecessor(dictionary.getRoot(), key);
                            }
                            System.out.println();
                        }

                        if (append) dictionary.remove(dictionary.getRoot(), command[1]); // remove the "fake" node

                        break;
                    }
                    case "first": // get first thing in dictionary
                        if (dictionary.getRoot().getData() == null) System.out.println("The ordered dictionary is empty");
                        else System.out.println(dictionary.smallest(dictionary.getRoot()).getName());
                        break;
                    case "last": // get last thing in dictionary
                        if (dictionary.getRoot().getData() == null) System.out.println("The ordered dictionary is empty");
                        else System.out.println(dictionary.largest(dictionary.getRoot()).getName());
                        break;
                    case "size": // get the size
                        System.out.println("There are " + dictionary.getNumInternalNodes() + " keys stored in the dictionary");
                        break;
                    case "end": // kill the program
                        return;
                    default: // invalid command
                        System.out.println("Invalid command");
                }
            }
        }
        // handle exceptions
        catch (FileNotFoundException e) { System.out.println("File not found: " + e); }
        catch (IOException e) { System.out.println("Failed to read file: " + e); }
        catch (MultimediaException e) { System.out.println("Failed to play/show " + e); }
        catch (DictionaryException e) { System.out.println(e); }
    }
}
