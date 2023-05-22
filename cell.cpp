// cell.cpp
// IMS Projekt
// Dátum: 6. 12. 2020
// Autor: Pavol Dubovec, xdubov02
// Autor: Juraj Lazúr, xlazur00

#include "cell.h"

cell::cell() {}
// Počiatočný stav pixelu
cell::cell(float start_temp, float start_rel_hum, float start_curing, float start_wind_sp,
           int start_wind_dir, double start_fuel_load) {
    destroyed = false;
    fuel_load = start_fuel_load;
    temp = start_temp;
    rel_hum = start_rel_hum;
    curing = start_curing;
    wind_speed = start_wind_sp;
    wind_direction = start_wind_dir;
    gfdi = count_gfdi(start_temp, start_rel_hum, start_curing, start_wind_sp, start_fuel_load);
}

// Výpočet aktuálneho horľavého materiálu na jednom pixeli
void cell::update_state() {
    // Počítame, koľto ton zhorelo za hodinu a stanovíme nový počet ton na pixeli
    // Z rozmerov kontinentu vieme spočítať, aké rozmery má 1 pixel
    /* 1 pixel = 4 km * 4,19 km -> 16,76 km štvorcových
     * 1 pixel = 16,76 * 100 -> počet hektárov
     * 1 pixel = 16,76 * 100 * fuel_load -> poceť ton horľavého materiálu na jednom pixely
     */

    // Aktuálny počet ton horľavého materiálu
    long double start = 1676.0 * fuel_load;

    // Spočítame nový index nebezpečenstva
    gfdi = count_gfdi(temp, rel_hum, curing, wind_speed, fuel_load);

    // Vypočítame rýchlosť horenia -> gfdi * 0.13
    // a následne počet ton, ktoré zhoreli za 1 hodinu
    // Za hodinu zhorí gfdi * 0.13 * 2 kilometrov štvorcových
    // prevedieme ich na hektáre, vynásobíme ich priemerným počtom ton horľavého materiálu
    // a získame počet ton, ktoré zhoreli, ten odpočítame od aktuálneho počtu ton
    start -= ((gfdi * 0.13) * 2) * 100 * fuel_load;

    // Vypočítame nový priemerný počet ton horľavého materiálu na 1 hektár na 1 pixeli
    fuel_load = start / (1676);

    // Ak zhorí 99% plochy, môžeme ju označiť za zhorenú
    if (fuel_load <= 0.125){
        destroyed = true;
        return;
    }
}

bool cell::is_destroyed() {
    return destroyed;
}

double cell::av_fire_load() {
    return fuel_load;
}

long double cell::get_gfdi() {
    return gfdi;
}

int cell::get_wind_direction() {
    return wind_direction;
};

// Výpočet GFDI
long double count_gfdi(float temp, float rel_hum, float curing, float wind_sp, double fuel_load){

    long double mc = (97.7 + 4.06 * rel_hum) / (temp + 6) - 0.00854 * rel_hum + 3000 / curing - 30;

    if (mc < 18.8) {
        return 3.35 * fuel_load * exp(-0.0897 * mc + 0.0403 * wind_sp);
    } else {
        return 0.299 * fuel_load * exp(-1.686 + 0.0403 * wind_sp) * (30 - mc);
    }
}