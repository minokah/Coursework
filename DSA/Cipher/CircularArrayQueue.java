/*
 * Assignment 3
 * Circle Array Queue
 */
public class CircularArrayQueue<T> implements QueueADT<T> {
	private int front;
	private int rear;
	private int count;
	private T[] queue;
	private final int DEFAULT_CAPACITY = 20;
	
	// Constructors, one default and one with specified capacity
	public CircularArrayQueue() {
		front = 1;
		rear = DEFAULT_CAPACITY;
		count = 0;
		queue = (T[])new Object[DEFAULT_CAPACITY];
	}
	
	public CircularArrayQueue(int capacity) {
		front = 1;
		rear = capacity;
		count = 0;
		queue = (T[])new Object[capacity];
	}
	
	// Enqueue object to rear
	public void enqueue(T object) {
		if (count + 1 == queue.length) expandCapacity();
		rear = (rear + 1) % queue.length;
		if (rear - 1 >= 0) queue[rear - 1] = object;
		else queue[queue.length - 1] = object;
		count++;
	}
	
	// Dequeue object from front
	public T dequeue() {
		if (count == 0) throw new EmptyCollectionException ("queue");
			
		T result;
		// I'm not sure what I did here but queue was always 1 short unless I added this
		if (front - 1 < 0) {
			result = queue[queue.length - 1];
		    queue[queue.length - 1] = null;
		}
		else {
			result = queue[front - 1];
		    queue[front - 1] = null;
	    }
	      
	    front = (front + 1) % queue.length;
	    count--;
	
	    return result;
	}
	
	// Returns first element in queue
	public T first() {
		if (count == 0) throw new EmptyCollectionException("queue");
		if (front - 1 < 0) return queue[queue.length - 1]; // Hacky fix again, read above
		return queue[front - 1];
	}
	
	// Is it empty?
	public boolean isEmpty() {
		if (count == 0) return true;
		return false;
	}
	
	// Obvious
	public int size() {
		return count;
	}
	
	public int getFront() {
		return front;
	}
	
	public int getRear() {
		return rear;
	}
	
	public int getLength() {
		return queue.length;
	}
	
	// Print queue
	public String toString() {
		String s = "QUEUE: ";
		if (count == 0) return "The queue is empty";
		for (int i = front - 1; i != count - 1; i++) s += queue[i] + ", ";
		s += queue[count - 1] + ".";
		return s;
	}
	
	// Expand queue
	private void expandCapacity() {
		T[] temp = (T[]) new Object[queue.length + 20];
		for (int i = 0; i != queue.length; i++) {
			temp[i] = queue[i];
		}
	    queue = temp;
	}
}
