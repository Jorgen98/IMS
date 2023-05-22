// grassland.h
// IMS Projekt
// Dátum: 6. 12. 2020
// Autor: Pavol Dubovec, xdubov02
// Autor: Juraj Lazúr, xlazur00

#include "cell.h"
#include <list>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <fstream>
#include <random>
#include <sstream>

#define X_SIZE 1000
#define Y_SIZE 920

using namespace std;

class grassland {
protected:
    cell* country[X_SIZE][Y_SIZE];
    long double cell_num;
    double general_fuel_load;
    long double average_gfdi;
    vector<pair<int, int>> on_fire_list;
    double random_prob();
    int random_x();
    int random_y();
    bool ignite(int x, int y);
    void ignite_neighbours(int x, int y);
    void update_state();
    void graphic_output(int index);
public:
    grassland();
    grassland(float temp, double fuel_load, const string& file_name, float REL_HUM, float WIND_SP, float CURING);
    void print_grassland_stats();
    void ignite_country(unsigned int start_fires);
    void simulation(unsigned int hours, bool graphic_out);
};
