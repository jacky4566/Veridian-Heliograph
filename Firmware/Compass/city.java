public class city {
    private String splitBy = ",";  
    private String cityName;
    private String shortName;
    private float latitude;
    private float longitude;
    private int population;

    public city(String data){
        String[] items = data.split(splitBy);    // use comma as separator
        this.cityName = items[0];
        this.latitude = Float.parseFloat(items[1]);
        this.longitude = Float.parseFloat(items[2]);
        this.population = Integer.parseInt(items[3]);
        this.shortName = String.format("%.8s", items[0]);
    }

    public String getCityName(){
        return cityName;
    }

    public String getShortName(){
        return shortName;
    }

    public float getLat(){
        return this.latitude;
    }

    public float getLong(){
        return this.longitude;
    }

    public int getpop(){
        return population;
    }

}
