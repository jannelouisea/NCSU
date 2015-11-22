import java.util.*;

public class Graph {
	
    public TreeMap<Integer, Integer> vertices;
	// public ArrayList<Integer> vertices;
	public ArrayList<Edge> edges;
	
	public Graph() {
        this.vertices = new TreeMap<Integer, Integer>();
		// this.vertices = new ArrayList<Integer>();
		this.edges = new ArrayList<Edge>();
	}
	
	public void addVertex(int key, int value) {
        vertices.put(key, value);
		// this.vertices.add(v);
	}
    
    public Integer getValue(int key) {
        return vertices.get(key);
    }
	
	public boolean contains(Integer v) {
		if (this.vertices.containsValue(v)) {
			return true;
		} else {
			return false;
		}
	}
	
	public void addEdge(Edge e) {
		this.edges.add(e);
	}
	
	public void removeEdge(Edge e) {
		this.edges.remove(e);
	}
	
	public boolean hasEdges() {
		if (edges.size() > 0) {
			return true;
		} else {
			return false;
		}
	}
    	
	public void printVertices() {
        Set set = this.vertices.entrySet();
        Iterator i = set.iterator();
        while(i.hasNext()) {
            Map.Entry current = (Map.Entry)i.next();
            System.out.print(current.getKey() + ": ");
            System.out.println(current.getValue());
        }
        /**
		String list = "";
		for (int i = 0; i < vertices.size(); i++) {
			list += this.vertices.getValue(i) + " ";
		}
		return list;
         */
	}
	
	public String printEdges() {
		String list = "";
		for (int i = 0; i < edges.size(); i++) {
			list += "[" + edges.get(i).getStart() + ", " + edges.get(i).getEnd() + "] ";
		}
		return list;
	}
	
}