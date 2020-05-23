#ifndef MAP_H
#define MAP_H


class map
{
public:
    map() {}

    //function to render the ground
    void render(unsigned long long key);

    //functions for differnet types of roads
    void topRoad(int currentX, int currentZ);
    void rightRoad(int currentX, int currentZ);
    void leftRoad(int currentX, int currentZ);
    void bottomRoad(int currentX, int currentZ);

    //functions for the different types of buildings
    void prismBuilding(int currentX, int currentZ);
    void cylBuilding(int currentX, int currentZ);
    void sphereBuilding(int currentX, int currentZ);

private:

};

#endif // MAP_H
