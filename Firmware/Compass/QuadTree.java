public class QuadTree {
    QTNode node;

    // Hold details of the boundary of this node
    XY topLeft;
    XY botRight;

    // Children of this tree
    QuadTree topLeftTree;
    QuadTree topRightTree;
    QuadTree botLeftTree;
    QuadTree botRightTree;

    public QuadTree() {
        topLeft = new XY(0,0);
        botRight = new XY(0,0);
        node = null;
        topLeftTree  = null;
        topRightTree = null;
        botLeftTree  = null;
        botRightTree = null;
    }

    public QuadTree(XY topL, XY botR)
    {
        node = null;
        topLeftTree  = null;
        topRightTree = null;
        botLeftTree  = null;
        botRightTree = null;
        topLeft = topL;
        botRight = botR;
    }

    public void insert(QTNode newNode){
        if (newNode == null)
            return;
    
        // Current quad cannot contain it
        if (!inBoundary(newNode.getXY()))
            return;
    
        // We are at a quad of unit area
        // We cannot subdivide this quad further
        if (abs(topLeft.x - botRight.x) <= 1 &&
            abs(topLeft.y - botRight.y) <= 1)
        {
            if (node == null)
                node = newNode;
            return;
        }
    
        if ((topLeft.x + botRight.x) / 2 >= node->pos.x)
        {
            // Indicates topLeftTree
            if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
            {
                if (topLeftTree == NULL)
                    topLeftTree = new Quad(
                        Point(topLeft.x, topLeft.y),
                        Point((topLeft.x + botRight.x) / 2,
                            (topLeft.y + botRight.y) / 2));
                topLeftTree->insert(node);
            }
    
            // Indicates botLeftTree
            else
            {
                if (botLeftTree == NULL)
                    botLeftTree = new Quad(
                        Point(topLeft.x,
                            (topLeft.y + botRight.y) / 2),
                        Point((topLeft.x + botRight.x) / 2,
                            botRight.y));
                botLeftTree->insert(node);
            }
        }
        else
        {
            // Indicates topRightTree
            if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
            {
                if (topRightTree == NULL)
                    topRightTree = new Quad(
                        Point((topLeft.x + botRight.x) / 2,
                            topLeft.y),
                        Point(botRight.x,
                            (topLeft.y + botRight.y) / 2));
                topRightTree->insert(node);
            }
    
            // Indicates botRightTree
            else
            {
                if (botRightTree == NULL)
                    botRightTree = new Quad(
                        Point((topLeft.x + botRight.x) / 2,
                            (topLeft.y + botRight.y) / 2),
                        Point(botRight.x, botRight.y));
                botRightTree->insert(node);
            }
        }
    }

    // Find a node in a quadtree
    public QTNode search(XY p)
    {
        // Current quad cannot contain it
        if (!inBoundary(p))
            return NULL;
    
        // We are at a quad of unit length
        // We cannot subdivide this quad further
        if (n != NULL)
            return n;
    
        if ((topLeft.x + botRight.x) / 2 >= p.x)
        {
            // Indicates topLeftTree
            if ((topLeft.y + botRight.y) / 2 >= p.y)
            {
                if (topLeftTree == NULL)
                    return NULL;
                return topLeftTree->search(p);
            }
    
            // Indicates botLeftTree
            else
            {
                if (botLeftTree == NULL)
                    return NULL;
                return botLeftTree->search(p);
            }
        }
        else
        {
            // Indicates topRightTree
            if ((topLeft.y + botRight.y) / 2 >= p.y)
            {
                if (topRightTree == NULL)
                    return NULL;
                return topRightTree->search(p);
            }
    
            // Indicates botRightTree
            else
            {
                if (botRightTree == NULL)
                    return NULL;
                return botRightTree->search(p);
            }
        }
    }

    // Check if current quadtree contains the point
    public boolean inBoundary(XY p){
        return (p.x >= topLeft.x &&
            p.x <= botRight.x &&
            p.y >= topLeft.y &&
            p.y <= botRight.y);
    }
}
