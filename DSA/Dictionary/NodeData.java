import java.util.ArrayList;

// NodeData, holds MultimediaItems and a key
public class NodeData {
    private String name;
    private ArrayList<MultimediaItem> media;

    public NodeData(String newName) {
        name = newName;
        media = new ArrayList<MultimediaItem>();
    }

    public void add(MultimediaItem newItem) { media.add(newItem); }
    public String getName() { return name; }
    public ArrayList<MultimediaItem> getMedia() { return media; }
}
