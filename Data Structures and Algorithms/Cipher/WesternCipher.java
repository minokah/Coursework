/*
 * Assignment 3
 * Encoding/Decoding Western Cipher
 * 
 * Encodes or Decodes a string of text from instructions
 */

import java.io.*;

public class WesternCipher {
	CircularArrayQueue encodingQueue;
	CircularArrayQueue decodingQueue;
	
	// Create queues, one default and one specified size
	public WesternCipher() {
		encodingQueue = new CircularArrayQueue<Character>(10);
		decodingQueue = new CircularArrayQueue<Character>(10);
	}
	
	public WesternCipher(int c) {
		encodingQueue = new CircularArrayQueue<Character>(c);
		decodingQueue = new CircularArrayQueue<Character>(c);
	}
	
	// Encoding method, returns encoded string
	public String encode(String input) {
		String result = "";
		String[] split = input.split("");
		int tick = 0; // Current index
		char prev = 0; // Previous character
		boolean skip4 = false; // Exception for #4
		
		for (int i = 0; i != input.length(); i++) encodingQueue.enqueue(split[i]); // Split and enqueue characters into queue
		
		for (int i = 0; i != input.length(); i++) {
			char a = ((String)encodingQueue.dequeue()).charAt(0); // Gets the character that is currently at front
			char orig = a;
			if (a == ' ') { // Add space
				a = ' '; // redundant, lazy to change this
				skip4 = false;
			}
			else if (a == 'A') { // Exceptions for table 1
				skip4 = true;
				if (prev == '1' || prev == '2' || prev == '3' || prev == '4' || prev == '5' || prev == '6') { // Exception for table 2
					a = '3';
				}
				else a = '1';
				// Table 1 and 2 exceptions repeat below for each character AEIOUY
			}
			else if (a == 'E') {
				skip4 = true;
				if (prev == '1' || prev == '2' || prev == '3' || prev == '4' || prev == '5' || prev == '6') {
					a = '4';
				}
				else a = '2';
			}
			else if (a == 'I') {
				skip4 = true;
				if (prev == '1' || prev == '2' || prev == '3' || prev == '4' || prev == '5' || prev == '6') {
					a = '5';
				}
				else a = '3';
			}
			else if (a == 'O') {
				skip4 = true;
				if (prev == '1' || prev == '2' || prev == '3' || prev == '4' || prev == '5' || prev == '6') {
					a = '6';
				}
				else a = '4';
			}
			else if (a == 'U') {
				skip4 = true;
				if (prev == '1' || prev == '2' || prev == '3' || prev == '4' || prev == '5' || prev == '6') {
					a = '1';
				}
				else a = '5';
			}
			else if (a == 'Y') {
				skip4 = true;
				if (prev == '1' || prev == '2' || prev == '3' || prev == '4' || prev == '5' || prev == '6') {
					a = '2';
					
				}
				else a = '6';
			}
			else { // For every other character
				int charInt = (int)a;
				charInt += 5; // Shift 5 forward
				charInt += (2 * tick); // Shift 2 * index of char forward
				if (skip4) { // Instruction to skip #4
					if (prev == '1') charInt -= 2*1; // Shift again based on previous character's number * 2
					if (prev == '2') charInt -= 2*2; // No, I'm not lazy at evaluating easy math
					if (prev == '3') charInt -= 2*3;
					if (prev == '4') charInt -= 2*4;
					if (prev == '5') charInt -= 2*5;
					if (prev == '6') charInt -= 2*6;
				}
				else skip4 = false;
				
				while (charInt > (int)'Z') charInt -= 26; // I did not use modulo % but instead just subtract/add for the boundaries
				while (charInt < (int)'A') charInt += 26;
				a = (char)charInt;
			}
			
			tick++; // Increment index, add char to result
			prev = a;
			result += a;
		}
		
		return result;
	}
	
	// Decoding method, returns decoded string
	// Basically the same thing as encode but reversed
	public String decode(String input) {
		String result = "";
		String[] split = input.split("");
		int tick = 0;
		char prev = 0;
		boolean skip4 = false;
		boolean conv2 = false; // There are 2 tables to deal with, see below
		
		for (int i = 0; i != input.length(); i++) decodingQueue.enqueue(split[i]);
		
		for (int i = 0; i != input.length(); i++) {
			char a = ((String)decodingQueue.dequeue()).charAt(0);
			if (a == ' ') {
				a = ' ';
				conv2 = false;
				skip4 = false;
			}
			else if (a == '1') {
				skip4 = true;
				if (prev == 'A' || prev == 'E' || prev == 'I' || prev == 'O' || prev == 'U' || prev == 'Y') {
					a = 'U';
					conv2 = true;
				}
				else a = 'A';
			}
			else if (a == '2') {
				skip4 = true;
				if (prev == 'A' || prev == 'E' || prev == 'I' || prev == 'O' || prev == 'U' || prev == 'Y') {
					a = 'Y';
					conv2 = true;
				}
				else a = 'E';
			}
			else if (a == '3') {
				skip4 = true;
				if (prev == 'A' || prev == 'E' || prev == 'I' || prev == 'O' || prev == 'U' || prev == 'Y') {
					a = 'A';
					conv2 = true;
				}
				else a = 'I';
			}
			else if (a == '4') {
				skip4 = true;
				if (prev == 'A' || prev == 'E' || prev == 'I' || prev == 'O' || prev == 'U' || prev == 'Y') {
					a = 'E';
					conv2 = true;
				}
				else a = 'O';
			}
			else if (a == '5') {
				skip4 = true;
				if (prev == 'A' || prev == 'E' || prev == 'I' || prev == 'O' || prev == 'U' || prev == 'Y') {
					a = 'I';
					conv2 = true;
				}
				else a = 'U';
			}
			
			else if (a == '6') {
				skip4 = true;
				if (prev == 'A' || prev == 'E' || prev == 'I' || prev == 'O' || prev == 'U' || prev == 'Y') {
					a = 'O';
					conv2 = true;
				}
				else a = 'Y';
			}
			else {
				int charInt = (int)a;
				charInt -= 5;
				charInt -= (2 * tick);
				if (skip4) {
					if (!conv2) { // Since there are 2 tables to deal with, we have to check which one to change
						if (prev == 'A') charInt += 2*1; // This is for table 1
						if (prev == 'E') charInt += 2*2;
						if (prev == 'I') charInt += 2*3;
						if (prev == 'O') charInt += 2*4;
						if (prev == 'U') charInt += 2*5;
						if (prev == 'Y') charInt += 2*6;
					}
					else {
						if (prev == 'A') charInt += 2*3; // This is for table 2 (that's why variable is named conv2, "convert 2[nd table]")
						if (prev == 'E') charInt += 2*4;
						if (prev == 'I') charInt += 2*5;
						if (prev == 'O') charInt += 2*6;
						if (prev == 'U') charInt += 2*1;
						if (prev == 'Y') charInt += 2*2;
						conv2 = false;
					}
				}
				else skip4 = false;
				
				while (charInt > (int)'Z') charInt -= 26;
				while (charInt < (int)'A') charInt += 26;
				a = (char)charInt;
			}
			
			tick++;
			prev = a;
			result += a;
		}
		
		return result;
	}
	
	// Main method for testing.
	public static void main(String[] args) {
		while (true) {
			char type = 0; // selection input
			String text = null; // string to work with
			
			while (true) { // Keep asking for input until e or d
				try {
					System.out.println("Encode or Decode your string? (e or d)");
					BufferedReader scan = new BufferedReader(new InputStreamReader(System.in));
					type = scan.readLine().charAt(0);
					if (type == 'e' || type == 'd') break;
					else System.out.println("Invalid selection: " + type);
				}
				catch (IOException e) {
					System.out.println("Invalid selection: " + type);
					continue;
				}
			}
			
			try { // Try to read the input
				System.out.println("Input your text to encode/decode.");
				BufferedReader scan = new BufferedReader(new InputStreamReader(System.in));
				text = scan.readLine();
			}
			catch (IOException e) {}; // stops java from being upset
			
			try { // In case you enter an invalid character or something
				// Encode/decode the string then print it
				String reconstructed;
				WesternCipher cipher = new WesternCipher(text.length());
				if (type == 'e') { // encode
					reconstructed = cipher.encode(text);
					System.out.println("Encode Result: " + reconstructed);
				}
				else { // decode
					reconstructed = cipher.decode(text);
					System.out.println("Decode Result (test): " + reconstructed);
				}
			}
			catch (Exception e) { System.out.println("Failed to encode/decode text: " + text); }
			
			char exit;
			while (true) { // Want to keep going?
				try {
					System.out.println("Enter another string? (n to exit, anything else to continue)");
					BufferedReader scan = new BufferedReader(new InputStreamReader(System.in));
					exit = scan.readLine().charAt(0);
				}
				catch (IOException e) {
					System.out.println("What did you just enter?");
					continue;
				}
				break;
			}
			if (exit == 'n') break;
		}
	}
}
