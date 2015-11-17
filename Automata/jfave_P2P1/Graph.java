import java.util.ArrayList;
import java.util.Arrays;

public class Graph {
	
	public ArrayList<Integer> vertices;
	public ArrayList<Edge> edges;
	
	public Graph() {
		this.vertices = new ArrayList<Integer>();
		this.edges = new ArrayList<Edge>();
	}
	
	public void addVertex(Integer v) {
		this.vertices.add(v);
	}
	
	public boolean contains(Integer v) {
		if (this.vertices.contains(v)) {
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
	
	public String printVertices() {
		String list = "";
		for (int i = 0; i < vertices.size(); i++) {
			list += vertices.get(i).intValue() + " ";
		}
		return list;
	}
	
	public String printEdges() {
		String list = "";
		for (int i = 0; i < edges.size(); i++) {
			list += "[" + edges.get(i).getStart() + ", " + edges.get(i).getEnd() + "] ";
		}
		return list;
	}
	
}