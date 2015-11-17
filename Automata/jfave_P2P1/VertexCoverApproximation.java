import java.io.*;
import java.util.*;
import java.util.ArrayList;
import java.util.Arrays;

public class VertexCoverApproximation {
	public static void main(String[] args) throws FileNotFoundException {
	
		// long startTime = System.nanoTime();	
		
		String fileName = args[0];
		int algorithm = Integer.parseInt(args[1]);
		Scanner input = new Scanner(new File(fileName));
		Graph graph = new Graph();
		
		while (input.hasNextLine()) {
			String line = input.nextLine();
			Scanner scnLine = new Scanner(line);
			Integer u = new Integer(scnLine.nextInt());
			Integer v = new Integer(scnLine.nextInt());
			if (!graph.contains(u)) {
				graph.addVertex(u);
			}
			if (!graph.contains(v)) {
				graph.addVertex(v);
			}
			Edge e = new Edge();
			e.setStart(u.intValue());
			e.setEnd(v.intValue());
			graph.addEdge(e);
		}
		input.close();
		
		String cover = "";	
		if (algorithm == 0) {
		    cover = greedyVertexApprox(graph);
		} else {
		    cover = greedyEdgeApprox(graph);
		}
		
		File outputFile = new File("approxOutput.txt");
		PrintStream output = new PrintStream(outputFile);
		output.println(cover);
		output.close();
		
		// long estimatedTime = System.nanoTime() - startTime;
		// System.out.println("Nanoseconds: " + estimatedTime);
	}
	
	public static String greedyVertexApprox(Graph g) {
		String cover = "";
		while (g.hasEdges()) {

			int mode = 0;
			int maxCount = 0;
			int[] copy = new int[g.vertices.size()];
			for (int i = 0; i < g.edges.size(); i++) {
				int index1 = g.edges.get(i).getStart();
				int index2 = g.edges.get(i).getEnd();
				copy[index1]++;
				copy[index2]++;
			}
			for (int i = 0; i < copy.length; i++) {
				if (copy[i] > maxCount) {
					maxCount = copy[i];
					mode = i;
				}
			}

			cover += mode + "\n";

			int[] removeList = new int[g.edges.size()];
			for (int i = 0; i < g.edges.size(); i++) {
				if (g.edges.get(i).contains(mode)) {
					removeList[i]++;
				}
			}	
			for (int i = removeList.length - 1; i >= 0; i--) {
					if (removeList[i] == 1) {
					g.removeEdge(g.edges.get(i));
				}
			}
		}
		return cover;
	}
	
	public static String greedyEdgeApprox(Graph g) {
		String cover = "";
		while (g.hasEdges()) {
			int vertex1 = g.edges.get(0).getStart();
			int vertex2 = g.edges.get(0).getEnd();
	
			cover += vertex1 + "\n" + vertex2 + "\n";
			
			int[] removeList = new int[g.edges.size()];
			for (int i = 0; i < g.edges.size(); i++) {
				if (g.edges.get(i).contains(vertex1) || g.edges.get(i).contains(vertex2)) {
					removeList[i]++;
				}
			}
			for (int i = removeList.length - 1; i >= 0; i--) {
				if (removeList[i] == 1) {
					g.removeEdge(g.edges.get(i));
				}
			}
		}
		return cover;
	}	
}
