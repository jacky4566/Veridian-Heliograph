public class QTNode {
    private XY point;
    private city data;              // associated data

    QTNode(){
        point = new XY(0,0);
    }

    QTNode(double x, double y) {
        point = new XY(x,y);
        this.data = null;
    }

    QTNode(double x, double y, city data) {
        point = new XY(x,y);
        this.data = data;
    }

    public city getData(){
        return data;
    }

    public XY getXY(){
        return point;
    }
}
