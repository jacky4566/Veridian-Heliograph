import java.io.*;  
import java.util.*;
import java.util.Scanner;  

public class TreeBuilder {
	public static void main(String[] args) {
		System.out.println("Hello World");
		
		List<List<String>> records = new ArrayList<>();
		try (BufferedReader br = new BufferedReader(new FileReader("Coordinates.csv"))) {
			String line;
			while ((line = br.readLine()) != null) {
				String[] values = line.split(",");
				records.add(Arrays.asList(values));
			}
			br.close();
		}catch (FileNotFoundException e){
		}
		catch (IOException e){
		}
		System.out.println(records.get(1));
		System.out.println("Entries Read: " + records.size());
	}
}

