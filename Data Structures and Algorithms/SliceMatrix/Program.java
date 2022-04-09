/**
 * 
 * Program class
 * Stores information about the cities and creates a map if needed
 */

public class Program {
	private int cityCount;
	private City[] cityArray;
	private CompressedArray array;
	
	/**
	 	Program constructor, reads and stores the file's information and creates a map if needed
	 */
	public Program(String file, boolean showMap) {
		this.cityArray = new City[3];
		MyFileReader reader = new MyFileReader(file);
		reader.readString(); // skip first line
		
		cityCount = 0;
		while (!reader.endOfFile()) {
			String city = reader.readString();
			int x = reader.readInt();
			int y = reader.readInt();

			if (cityCount > 2) extendCityArray(cityArray, 1); // adapt to the array size constraint
			cityArray[cityCount] = new City(city, x, y);
			
			cityCount++;
		}
		cityCount = cityArray.length;
		
		if (showMap) { // create the map if necessary
			Map map = new Map();
			for (int i = 0; i != cityCount; i++) map.addCity(cityArray[i]);
		}
	}
	
	/**
	 * @return Get the city list as an array
	 */
	public City[] getCityList() { return cityArray; }
	
	/**
	 * @return Get the compressed array
	 */
	public CompressedArray getArray() { return array; } 
	
	/**
	 * @return Expands the city array by 3
	 */
	public void expandCapacity() { extendCityArray(cityArray, 3); }
	
	/**
	 * @return Calculates the distance between two cities
	 * @param city1 city number one
	 * @param city2 city number two
	 */
	public double distBetweenCities(City city1, City city2) { return Math.hypot(city2.getX() - city1.getX(), city2.getY() - city1.getY()); }
	
	/**
	 * @return Compare the distances for all cities and saves it to a compressedArray
	 */
	public void compareDistances() {
		double[][] array = new double[cityCount][cityCount];
		
		for (int i = 0; i != cityCount; i++) {
			for (int a = 0; a != cityCount; a++) {
				array[i][a] = distBetweenCities(cityArray[i], cityArray[a]);
			}
		}

		this.array = new CompressedArray(array);
	}
	
	/**
	 * @return Extends the array by num spaces
	 * @param array array of doubles
	 * @param num number to extend the array by
	 */
	// can't import array package, so here's a workaround
	// gradescope specifies that you cannot have any additional public variables, so this is now private
	private void extendCityArray(City[] array, int num ) {
		City[] newCityArray = new City[array.length + num];
		for (int i = 0; i != array.length; i++) { newCityArray[i] = array[i]; }
		this.cityArray = newCityArray;
	}
}
