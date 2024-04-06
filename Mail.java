import java.util.concurrent.locks.*;
import java.util.concurrent.atomic.*;

public class Mail {
	static final int n_letters = 500000;
	final OptNode head; 
	final AtomicInteger size;

	public static void main(String[] args) {

	}

	
}

class OptNode {
	Lock lock;
	int value;
	OptNode next;

	public OptNode(int k) {
		this.lock = new ReentrantLock();
		this.value = k;
	}

	public void lock() {
		this.lock.lock();
	}

	public void unlock() {
		this.lock.unlock();
	}
}
