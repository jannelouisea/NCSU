public class Edge {
	
	private int start;
	private int end;
	
	public void Edge() { 
		start = 0;
		end = 0;
	}
	
	public void setStart(int v) {
		this.start = v;
	}
	
	public void setEnd(int v) {
		this.end = v;	
	}

	
	public int getStart() {
		return start;
	}
	
	public int getEnd() {
		return end;
	}
	
	public boolean contains(int v) {
		if (start == v || end == v) {
			return true;
		} else {
			return false;
		}
	}
}