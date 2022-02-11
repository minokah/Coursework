// MultimediaItem, stores a key and the name of the file
public class MultimediaItem {
    private String content;
    private int type;

    public MultimediaItem(String newContent, int newType) {
        content = newContent;
        type = newType;
    }

    public String getContent() { return content; }
    public int getType() { return type; }
}
