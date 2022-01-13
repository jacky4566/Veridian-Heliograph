import java.io.BufferedReader;  
import java.io.FileReader;  
import java.io.IOException;   

class QuadTreeGenerator {
    public static void main(String[] args) {
        if (args.length != 1){
            System.out.println("Incorect arguments"); // Display the string.
            System.exit(0);
        }
        
        String targetFile = args[0];
        String line = "";  
        
        System.out.println("Reading Target File: " + targetFile);
        try (BufferedReader br = new BufferedReader(new FileReader(targetFile));  ){
            while ((line = br.readLine()) != null){
                city newCity = new city(line);
                System.out.println("City: " + newCity.getCityName() + " (" + newCity.getShortName() + ")" + " Lat/Long: " + newCity.getLat() + "/" + newCity.getLong() + " Pop: " + newCity.getpop());
            }
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } 
    }
}