/**
 * 
 * CompressedArray class
 * Takes a 2D array and removes the diagonal and top right elements
 */

public class CompressedArray {
	private int origArraySize;
	private double[] array;
	
	/**
	 	CompressedArray constructor, compresses a 2D array by shaving off the diagonal and top right entries
	 */
	public CompressedArray(double[][] array) {
		this.array = new double[]{};
		this.origArraySize = array.length;
		
		for (int i = 0; i != this.origArraySize; i++) {
			for (int a = 0; a != array[i].length; a++) {
				if (i != a) {
					extendArray(this.array, 1);
					this.array[this.array.length - 1] = array[i][a];
				}
				else break; // stop at the diagonal, continue to the next iteration
			}
		}
	}
	
	/**
	 * @return Get the length of the array
	 */
	public int getLength() { return this.array.length; }
	
	/**
	 * @return Get the object at the current element #
	 * @param element Element number
	 */
	public double getElement(int element) { return this.array[element]; }
	
	/**
	 * @return Compares two compressedArray-s and checks if they're equal
	 * @param array compressedArray object
	 */
	public boolean equals(CompressedArray array) { 
		for (int i = 0; i != this.array.length; i++) {
			if (this.array[i] != array.array[i]) return false;
		}
		return true;
	}
	
	/**
	 * @return Print statement
	 */
	public String toString() {
		String returnString = "";
		
		int currentArrayItem = 0;	
		for (int i = 0; i != this.origArraySize; i++) {
			for (int a = 0; a != i; a++) {
				returnString += String.format("%8.2f", this.array[currentArrayItem]);
				currentArrayItem++;
			}
			returnString += "\n";
		}
		return returnString;
	}
	
	/**
	 * @return Extends the array by num spaces
	 * @param array array of doubles
	 * @param num number to extend the array by
	 */
	// can't import array package, so here's a workaround
	// gradescope specifies that you cannot have any additional public variables, so this is now private
	private void extendArray(double[] array, int num) {
		double[] newArray = new double[array.length + num];
		for (int i = 0; i != array.length; i++) { newArray[i] = array[i]; }
		this.array = newArray;
	}
}
