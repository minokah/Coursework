/**
 * 
 * City class
 * Stores city information
 */

public class City {
	private String name;
	private int x;
	private int y;
	private CityMarker marker;
	
	/**
	 	City constructor, create a city with the parameters
	 */
	public City(String city, int x, int y) {
		this.name = city;
		this.x = x;
		this.y = y;
		this.marker = new CityMarker();
	}
	
	// all these functions are self explanatory...
	public String getName() { return this.name; }
	public int getX() { return this.x; }
	public int getY() { return this.y; }
	public CityMarker getMarker() { return this.marker; }
	
	public void setName(String name) { this.name = name; }
	public void setX(String x) { this.x = Integer.parseInt(x); }
	public void setY(String y) { this.y = Integer.parseInt(y); }
	public void setMarker(CityMarker marker) { this.marker = marker; }
	
	public String toString() { return this.name; }
}
