// main.cpp
// IMS Projekt
// Dátum: 6. 12. 2020
// Autor: Pavol Dubovec, xdubov02
// Autor: Juraj Lazúr, xlazur00

#include "grassland.h"
#include <random>
#include <cstring>

using namespace std;

double FUEL_LOAD = 4.9;
float TEMPERATURE = 26;
string FILE_NAME = "cordinates.txt";
int START_FIRES = 50;
int HOURS = 96;
bool GRAPHIC = false;

float REL_HUM = -1;
float WIND_SP = -1;
float CURING = -1;

// Funkcia pre spracovanie argumentov
void process_params(int argc, char* argv[]){
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-help") == 0){
            printf("---GRASSLAND FIRE SIMULATION---\n\n");
            printf("---HELP---\n\n");
            printf("Simulation can be run without any arguments, "
                   "or you can modify simulation with additional arguments.\n\n");

            printf("Argument\t\tOption\tDefault\t\tRange\n");
            printf("graphic output\t\t-g\tfalse\n");
            printf("fuel load - t/h\t\t-f\t4,9\t\t0-24,9\n");
            printf("temperature - C\t\t-t\t26\t\t0-44\n");
            printf("num of start fires\t-sf\t50\t\t0-400\n");
            printf("duration - hours\t-h\t96\t\t0-672\n\n");
            printf("Special options\n");
            cout << "relative humidity - %";
            printf("\t-rh\tby country part\t1-99\n");
            printf("wind speed - km/h\t-w\tby country part\t1-69\n");
            cout << "curing - %";
            printf("\t\t-c\tby country part\t60-100\n");
            exit(0);
        } else if (strcmp(argv[i], "-g") == 0)
            GRAPHIC = true;
        else if (strcmp(argv[i], "-f") == 0){
            i++;
            FUEL_LOAD = stod(argv[i]);
        } else if (strcmp(argv[i], "-t") == 0){
            i++;
            TEMPERATURE = stof(argv[i]);
        } else if (strcmp(argv[i], "-sf") == 0){
            i++;
            START_FIRES = stoi(argv[i]);
        } else if (strcmp(argv[i], "-h") == 0){
            i++;
            HOURS = stoi(argv[i]);
        } else if (strcmp(argv[i], "-w") == 0){
            i++;
            WIND_SP = stof(argv[i]);
        } else if (strcmp(argv[i], "-rh") == 0){
            i++;
            REL_HUM = stof(argv[i]);
        } else if (strcmp(argv[i], "-c") == 0){
            i++;
            CURING = stof(argv[i]);
        }
    }
}

int main(int argc, char* argv[]) {

    // Spracovanie argumentov
    if (argc > 1) {
        process_params(argc, argv);
    }

    printf("---GRASSLAND FIRE SIMULATION---\n\n");

    printf("---Starting simulation---\n\n");

    // Inicializácia plochy
    static grassland grass = grassland(TEMPERATURE, FUEL_LOAD, FILE_NAME, REL_HUM, WIND_SP, CURING);

    // Umiestnenie počiatočných bodov horenia
    grass.ignite_country(START_FIRES);
    // Aktualizácia stavu simulácie
    // 1 krok je 1 hodina
    grass.simulation(HOURS, GRAPHIC);

    // Výpis štatistík
    grass.print_grassland_stats();

    return 0;
}
