/*
 *  Assignment 2
 *  StartSearch class.
 *  Cupid will attempt to fire arrows to hit targets based on some restrictions.
 */

import java.io.FileNotFoundException;
import java.io.IOException;

public class StartSearch {
	private Map targetMap;
	private int numArrows;
	private int inertia;
	private int direction; // 0123 = NESW
	
	// Initialize the class. Sets up map and arrow count.
	StartSearch(String filename) throws InvalidMapException, FileNotFoundException, IOException, InvalidNeighbourIndexException {
		targetMap = new Map(filename);
		numArrows = targetMap.quiverSize();
	}
	
	public static void main(String[] args) throws InvalidMapException, FileNotFoundException, IOException, InvalidNeighbourIndexException {
		try {
			if (args.length < 1) {
				System.out.println("You must provide the name of the input file");
				System.exit(0);
			}
			
			// Debug purposes.
			System.out.println("Debug Mode is disabled, change that in the nextCell() method!");
			
			String mapFileName = args[0];
			int maxPathLength = -1;
			if (args.length > 1) maxPathLength = Integer.parseInt(args[1]);
			
			int cellsTravelled = 0;
			
			int targetCount = 0;
			int backtracks = 3;
			
			StartSearch search = new StartSearch(mapFileName);
			ArrayStack<MapCell> mapCells = new ArrayStack<MapCell>();

			// While loop until Cupid is out of arrows.
			while (search.numArrows > 0) {
				search.inertia = 3;
				search.direction = 5;
				backtracks = 3;
				cellsTravelled = 0;
				
				mapCells.push(search.targetMap.getStart());
				mapCells.peek().markInStack();
				
				while (mapCells.size() != 0) {
					if (cellsTravelled == maxPathLength) break; // Immediately terminate if the step limit has been reached.
					else {
						// Next best possible cell found!
						MapCell next = search.nextCell(mapCells.peek());
						if (next != null && backtracks > 0) {
							cellsTravelled++;
							mapCells.push(next);
							mapCells.peek().markInStack();
							if (mapCells.peek().isTarget()) {
								targetCount++;
								break;
							}
						}
						else {
							// Next best possible cell doesn't exist, attempt to backtrack.
							// If it can't it will terminate this arrow.
							cellsTravelled++;
							MapCell current = mapCells.peek();
							mapCells.pop();
							if (backtracks != 0) {
								backtracks--;
								if (mapCells.size() > 0) search.direction = search.getDirection(current, mapCells.peek());
							}
							else break;
						}
					}
				}
				
				// Clear the stack.
				while (mapCells.size() != 0) {
					if (mapCells.size() == 2) mapCells.peek().markOutStack();
					mapCells.pop();
				}
				
				search.numArrows--;
			}
			
			System.out.println("Targets Hit: " + targetCount);
		}
		// Handle the potential exceptions.
		catch (FileNotFoundException e) { System.out.println("That file doesn't exist."); }
		catch (IOException e) { System.out.println("Failed to read something. Check your parameters."); }
		catch (InvalidMapException e) { System.out.println("Failed to read map. " + e.getMessage() ); }
		catch (EmptyStackException e) { System.out.println(e.getMessage()); }
		catch (InvalidNeighbourIndexException e) { System.out.println("Invalid neighbour index."); }
	}
	
	// Determines the next best cell.
	// Will attempt to search for the next cell in front of the current direction first,
	// and then try to turn if possible if the cell in front is blocked/not possible to traverse.
	
	/*
	 * I have split up each possible cell into separate arrays, then for each direction
	 * I checked to see if they were a valid move or not.
	 * 
	 * Lots of copy pasting!
	 */
	private MapCell nextCell(MapCell cell) {
		boolean debugMode = false;
		MapCell picked = null;
		
		// Get all the neighbouring cells
		MapCell n = cell.getNeighbour(0);
		MapCell e = cell.getNeighbour(1);
		MapCell s = cell.getNeighbour(2);
		MapCell w = cell.getNeighbour(3);
		
		// Rule out all the walls as possible solutions
		MapCell[] notWall = new MapCell[] {};
		if (n != null && !n.isBlackHole() && !n.isMarked()) notWall = expandMe(notWall, n);
		if (e != null && !e.isBlackHole() && !e.isMarked()) notWall = expandMe(notWall, e);
		if (s != null && !s.isBlackHole() && !s.isMarked()) notWall = expandMe(notWall, s);
		if (w != null && !w.isBlackHole() && !w.isMarked()) notWall = expandMe(notWall, w);
		
		//MapCell[] possibleDirectionMoves = new MapCell[] {};
		// Find out all valid possible moves
		MapCell[] possibleCross = new MapCell[] {};
		MapCell[] possibleTarget = new MapCell[] {};
		MapCell[] possibleVertHor = new MapCell[] {};
		MapCell[] possibleVert = new MapCell[] {};
		MapCell[] possibleHor = new MapCell[] {};
		MapCell[] possibleAll = new MapCell[] {};
		for (int i = 0; i != notWall.length; i++) {
			if (notWall[i].isTarget()) {
				if (debugMode) System.out.println("Possible target found!");
				possibleTarget = expandMe(possibleTarget, notWall[i]);
				possibleAll = expandMe(possibleAll, notWall[i]);
			}
			if (notWall[i].isCrossPath()) {
				possibleCross = expandMe(possibleCross, notWall[i]);
				possibleAll = expandMe(possibleAll, notWall[i]);
			}
			
			// left/right valid moves
			if (notWall[i].isVerticalPath()) { 
				if (n != null) if (notWall[i].getIdentifier() == n.getIdentifier()) { // make sure it's actually above/below cell
					possibleVertHor = expandMe(possibleVertHor, notWall[i]);
					possibleVert = expandMe(possibleVert, notWall[i]);
					possibleAll = expandMe(possibleAll, notWall[i]);
				}
				if (s != null) if (notWall[i].getIdentifier() == s.getIdentifier()) {
					possibleVertHor = expandMe(possibleVertHor, notWall[i]);
					possibleVert = expandMe(possibleVert, notWall[i]);
					possibleAll = expandMe(possibleAll, notWall[i]);
				}
			}
			if (notWall[i].isHorizontalPath()) {	
				if (w != null) if (notWall[i].getIdentifier() == w.getIdentifier()) { 
					possibleVertHor = expandMe(possibleVertHor, notWall[i]);
					possibleHor = expandMe(possibleHor, notWall[i]);
					possibleAll = expandMe(possibleAll, notWall[i]);
				}
				if (e != null) if (notWall[i].getIdentifier() == e.getIdentifier()) {
					possibleVertHor = expandMe(possibleVertHor, notWall[i]);
					possibleHor = expandMe(possibleHor, notWall[i]);
					possibleAll = expandMe(possibleAll, notWall[i]);
				}
			}
		}
		
		// Priority is always: whatever is in front > target > cross > verthor.
		// Lots of copy paste as initially I wanted to check for each direction
		// so for every check I did for direction I had to do for the others.
		
		if (picked == null) {
			if (cell.isStart()) { // is it the first cell (no direction)?
				if (debugMode) System.out.println("Starting first find");
				picked = lowestIndex(cell, possibleTarget);
				if (picked == null) picked = lowestIndex(cell, possibleCross);
				if (picked == null) picked = lowestIndex(cell, possibleVertHor);
				if (picked == null && debugMode) System.out.println("No first step");
			}
			else if (direction == 0) {
				if (n != null) {
					if (n.isCrossPath() && !n.isMarked() || n.isVerticalPath() && !n.isMarked() || n.isTarget() && !n.isMarked()) {
						picked = n;
					}
					else {
						if (cell.isCrossPath()) {
							picked = lowestIndex(cell, possibleTarget);
							if (picked == null) picked = lowestIndex(cell, possibleCross);
							if (picked == null) picked = lowestIndex(cell, possibleHor);
						}
					}
				}
				else {
					if (cell.isCrossPath()) {
						picked = lowestIndex(cell, possibleTarget);
						if (picked == null) picked = lowestIndex(cell, possibleCross);
						if (picked == null) picked = lowestIndex(cell, possibleHor);
					}
				}
			}
			else if (direction == 1) {
				if (e != null) {
					if (e.isCrossPath() && !e.isMarked() || e.isHorizontalPath() && !e.isMarked() || e.isTarget() && !e.isMarked()) {
						picked = e;
					}
					else {
						if (cell.isCrossPath()) {
							picked = lowestIndex(cell, possibleTarget);
							if (picked == null) picked = lowestIndex(cell, possibleCross);
							if (picked == null) picked = lowestIndex(cell, possibleVert);
						}
					}
				}
				else {
					if (cell.isCrossPath()) {
						picked = lowestIndex(cell, possibleTarget);
						if (picked == null) picked = lowestIndex(cell, possibleCross);
						if (picked == null) picked = lowestIndex(cell, possibleVert);
					}
				}
			}
			else if (direction == 2) {
				if (s != null) {
					if (s.isCrossPath() && !s.isMarked() || s.isVerticalPath() && !s.isMarked() || s.isTarget() && !s.isMarked()) {
						picked = s;
					}
					else {
						if (cell.isCrossPath()) {
							picked = lowestIndex(cell, possibleTarget);
							if (picked == null) picked = lowestIndex(cell, possibleCross);
							if (picked == null) picked = lowestIndex(cell, possibleHor);
						}
					}
				}
				else {
					if (cell.isCrossPath()) {
						picked = lowestIndex(cell, possibleTarget);
						if (picked == null) picked = lowestIndex(cell, possibleCross);
						if (picked == null) picked = lowestIndex(cell, possibleHor);
					}
				}
			}
			else if (direction == 3) {
				if (w != null) {
					if (w.isCrossPath() && !w.isMarked() || w.isHorizontalPath() && !w.isMarked() || w.isTarget() && !w.isMarked()) {
						picked = w;
					}
					else {
						if (cell.isCrossPath()) {
							picked = lowestIndex(cell, possibleTarget);
							if (picked == null) picked = lowestIndex(cell, possibleCross);
							if (picked == null) picked = lowestIndex(cell, possibleVert);
						}
					}
				}
				else {
					if (cell.isCrossPath()) {
						picked = lowestIndex(cell, possibleTarget);
						if (picked == null) picked = lowestIndex(cell, possibleCross);
						if (picked == null) picked = lowestIndex(cell, possibleVert);
					}
				}
			}
		}
		
		// Check for turning. Inertia checks.
		int oldDirection = direction;
		
		direction = getDirection(cell, picked);
		
		if (oldDirection == direction && !cell.isStart()) inertia--;
		else if (inertia > 3) inertia = 3;
		
		if (picked == null || picked.isMarked() || inertia <= 0 && oldDirection != direction) {
			if (inertia <= 0 && oldDirection != direction) {
				if (debugMode) System.out.println("Ran out of inertia!");
				else return null;
			}
			if (n != null) if (oldDirection == 0 && n.isBlackHole()) return picked;
			if (e != null) if (oldDirection == 1 && e.isBlackHole()) return picked;
			if (s != null) if (oldDirection == 2 && s.isBlackHole()) return picked;
			if (w != null) if (oldDirection == 3 && w.isBlackHole()) return picked;
			if (picked == null) {
				if (debugMode) System.out.println("No valid move found! :(");
			}
			else if (picked.isMarked()) if (debugMode) System.out.println("This is already marked!");
			return null;
		}
		else return picked;
	}
	
	// Expands a MapCell array.
	private MapCell[] expandMe(MapCell[] a, MapCell b) {
		MapCell[] c = new MapCell[a.length + 1];
		for (int i = 0; i != a.length; i++) c[i] = a[i];
		c[a.length] = b;
		return c;
	}

	// Returns the lowest indexed item relative to the current cell.
	private MapCell lowestIndex(MapCell cell, MapCell[] list) {
		MapCell lowest = null;
		
		if (list.length > 0) {
			for (int i = 0; i != list.length; i++) { if (cell.getNeighbour(0) != null && list[i].getIdentifier() == cell.getNeighbour(0).getIdentifier()) return list[i]; }
			for (int i = 0; i != list.length; i++) { if (cell.getNeighbour(1) != null && list[i].getIdentifier() == cell.getNeighbour(1).getIdentifier()) return list[i]; }
			for (int i = 0; i != list.length; i++) { if (cell.getNeighbour(2) != null && list[i].getIdentifier() == cell.getNeighbour(2).getIdentifier()) return list[i]; }
			for (int i = 0; i != list.length; i++) { if (cell.getNeighbour(3) != null && list[i].getIdentifier() == cell.getNeighbour(3).getIdentifier()) return list[i]; }	
		}
		
		return null;
	}
	
	// Returns the direction of another cell relative to the current cell.
	private int getDirection(MapCell main, MapCell cell) {
		int a = 5; // 5 is a dummy value- the direction will never be 5, effectively null
		MapCell n = main.getNeighbour(0);
		MapCell e = main.getNeighbour(1);
		MapCell s = main.getNeighbour(2);
		MapCell w = main.getNeighbour(3);
		if (n != null && cell != null) if (n.getIdentifier() == cell.getIdentifier()) a = 0;
		if (e != null && cell != null) if (e.getIdentifier() == cell.getIdentifier()) a = 1;
		if (s != null && cell != null) if (s.getIdentifier() == cell.getIdentifier()) a = 2;
		if (w != null && cell != null) if (w.getIdentifier() == cell.getIdentifier()) a = 3;
		return a;
	}
}