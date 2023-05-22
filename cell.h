// cell.h
// IMS Projekt
// Dátum: 6. 12. 2020
// Autor: Pavol Dubovec, xdubov02
// Autor: Juraj Lazúr, xlazur00

#include <cmath>
#include <cstdio>

// Základný prvok celulárneho automatu
// 1 pixel
class cell {
protected:
    // Je pixel zhorený
    bool destroyed;
    // Priemerný počet ton horľavého materiálu na hektár
    double fuel_load;
    // Priemerná vlhkosť
    float rel_hum;
    // Teplota
    float temp;
    // Vysušenie pôdy
    float curing;
    // Rýchlosť vetra
    float wind_speed;
    // Smer vetra
    int wind_direction;
    // GFDI faktor
    long double gfdi;
public:
    cell();
    cell(float start_temp, float start_rel_hum, float start_curing, float start_wind_sp,
         int start_wind_dir, double start_fuel_load);
    void update_state();
    bool is_destroyed();
    double av_fire_load();
    long double get_gfdi();
    int get_wind_direction();
};

long double count_gfdi(float temp, float rel_hum, float curing, float wind_sp, double fuel_load);
