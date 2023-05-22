// grassland.cpp
// IMS Projekt
// Dátum: 6. 12. 2020
// Autor: Pavol Dubovec, xdubov02
// Autor: Juraj Lazúr, xlazur00

#include "grassland.h"

grassland::grassland() {}

// Inicializácia
grassland::grassland(float temp, double fuel_load, const string& file_name, float REL_HUM, float WIND_SP, float CURING) {
    if ((0 > temp || temp > 45) || (0 > fuel_load || fuel_load > 25)) {
        exit(-1);
    }

    cell_num = 0;
    average_gfdi = 0;

    // Inicializácia array
    for (auto & x : country) {
        for (auto & y : x) {
            y = nullptr;
        }
    }

    printf("Loading file %s\n", file_name.c_str());

    // Otváranie súboru
    ifstream file(file_name);

    if (!file.good()){
        exit(-1);
    }

    char delimeter;
    int x;
    int y;
    float rel_hum;
    float wind_sp;
    int wind_dir;
    float curing;

    // Načítanie zo súboru
    while (file >> delimeter >> x >> delimeter >> y >> delimeter >> rel_hum >> delimeter
           >> wind_sp >> delimeter >> wind_dir >> delimeter >> curing >> delimeter){
        if ((0 > rel_hum || rel_hum > 100) || (59 > curing || curing > 100) || (0 > wind_sp || wind_sp > 70)) {
            exit(-1);
        }

        if ((1 > x || x > (X_SIZE-1)) || (1 > y || y > (Y_SIZE-1))){
            exit(-1);
        }

        if (REL_HUM != -1){
            rel_hum = REL_HUM;
        }
        if (CURING != -1){
            curing = CURING;
        }
        if (WIND_SP != -1){
            wind_sp = WIND_SP;
        }

        // Ukladanie do array
        country[x][y] = new cell(temp, rel_hum, curing, wind_sp, wind_dir, fuel_load);
        // Výpočet počiatočného GFDI indexu
        average_gfdi += country[x][y]->get_gfdi();
        cell_num++;
    }

    on_fire_list.clear();
    general_fuel_load = fuel_load;

    file.close();

    printf("File successfully loaded\n\n");
}

// Založenie ohňa
bool grassland::ignite(int x, int y) {
    if (country[x][y] == nullptr){
        return false;
    }

    // Overíme, či už políčko nehorí
    if (find(on_fire_list.begin(), on_fire_list.end(), pair<int, int>(x,y)) != on_fire_list.end()){
        return false;
    }

    // Uloží si políčko na zoznam horiacich
    on_fire_list.emplace_back(x, y);

    return true;
}

// Podpaľovanie susedných políčok
void grassland::ignite_neighbours(int x, int y) {

    // Získame polohu susedných políčok
    pair<int, int> neighbours[8];
    //S
    neighbours[0] = pair<int, int>(x, y-1);
    //SZ
    neighbours[1] = pair<int, int>(x+1, y-1);
    //Z
    neighbours[2] = pair<int, int>(x+1, y);
    //JZ
    neighbours[3] = pair<int, int>(x+1, y+1);
    //J
    neighbours[4] = pair<int, int>(x, y+1);
    //JV
    neighbours[5] = pair<int, int>(x-1, y+1);
    //V
    neighbours[6] = pair<int, int>(x-1, y);
    //SV
    neighbours[7] = pair<int, int>(x-1, y-1);

    cell* ac_cell = country[x][y];

    // Zaznamenáme si smer vetra
    // Smer vetra ovplyvňuje zvýšenie pravdepodobnosti zapálenia políčka, na ktoré vietor fúka
    int wind[3] = {ac_cell->get_wind_direction() - 1, ac_cell->get_wind_direction(), ac_cell->get_wind_direction() + 1};

    for (int & i : wind){
        if (i < 0){
            i += 8;
        }
    }

    // Skúmame okolité pixely
    for (int i = 0; i < 8; i++){
        // Pre každý pixel vypočítame na základe jeho GFDI, smeru a rýchlosti vetra pravdepodobnosť vzbĺknutia
        ac_cell = country[neighbours[i].first][neighbours[i].second];

        // Políčko nie je typu grassland, alebo už zhorelo
        if (ac_cell == nullptr || ac_cell->is_destroyed()) {
            continue;
        }

        long double prob;
        // Základom pravdepodobnosti je index GFDI
        prob = ac_cell->get_gfdi();

        // Vplyv smeru vetra
        for (int j = 0; j < 3; j++) {
            if (((wind[j] == i) && (j == 0)) || ((wind[j] == i) && (j == 2)))
                prob *= 1.15;
            else if ((wind[j] == i) && (j == 1))
                prob *= 1.30;
            else if (j == 2)
                prob *= 0.70;
        }

        // Pri hodnotách GFDI nižších ako 4 sa oheň nešíri
        // Pri hodnotách GFDI vyšších ako 50 sa oheň šíri nekontrolovane
        if (prob <= 4.0){
            continue;
        } else if (prob >= 100.0){
            if (find(on_fire_list.begin(), on_fire_list.end(),
                     pair<int, int>(neighbours[i].first, neighbours[i].second)) == on_fire_list.end()){
                on_fire_list.emplace_back(neighbours[i].first, neighbours[i].second);
            }
            continue;
        }

        prob = prob / 200;

        if (random_prob() <= prob){
            if (find(on_fire_list.begin(), on_fire_list.end(),
                     pair<int, int>(neighbours[i].first, neighbours[i].second)) == on_fire_list.end()){
                on_fire_list.emplace_back(neighbours[i].first, neighbours[i].second);
            }
        }
    }
}

// Aktualizácia stavu sledovanej plochy
void grassland::update_state() {
    cell* ac_cell;

    // Prechádzame horiace políčka
    for (long int i = 0; i < on_fire_list.size(); i++) {
        ac_cell = country[on_fire_list[i].first][on_fire_list[i].second];
        ac_cell->update_state();

        // Ak políčko zhorelo z na viac ako 50%, oheň je na opačnej hranici bunky
        // a môže sa rozšíriť
        if (ac_cell->av_fire_load() < (general_fuel_load / 4)){
            ignite_neighbours(on_fire_list[i].first, on_fire_list[i].second);
        }

        // Ak políčko zhorelo, odstránime ho zo zoznamu
        if (ac_cell->is_destroyed()){
            on_fire_list.erase(std::remove(on_fire_list.begin(), on_fire_list.end(),
                                           pair<int, int>(on_fire_list[i].first, on_fire_list[i].second)), on_fire_list.end());
        }
    }
}

// Výpis štatistík
void grassland::print_grassland_stats() {
    printf("---STATS---\n\n");
    printf("Observed cells:\t%.0Lf\n", cell_num);
    printf("Observed area:\t%.3Lf kilometers square\n\n", cell_num * 4.0 * 4.19);

    cout << "Average starting GFDI index: " << average_gfdi / cell_num << "\n\n";

    cell* ac_cell;
    long double on_fire_per = 0;

    if (on_fire_list.empty()){
        printf("Still burning:\t0 cells\n\n");

    } else {
        printf("Still burning:\t%ld cells\n", on_fire_list.size());

        for (auto &part : on_fire_list) {
            ac_cell = country[part.first][part.second];
            on_fire_per += ac_cell->av_fire_load();
        }

        on_fire_per = on_fire_per * 100 / (cell_num * general_fuel_load);

        cout << "Still burning:\t" << on_fire_per << '%' << "\n\n";
    }

    on_fire_per = 0;

    for (auto & x : country) {
        for (auto & y : x) {
            if (y == nullptr){
                continue;
            }
            on_fire_per += general_fuel_load - y->av_fire_load();
        }
    }

    on_fire_per = on_fire_per * 100 / (cell_num * general_fuel_load);

    cout << "Burned area:\t" << on_fire_per << '%' << '\n';
    printf("Burned area:\t%.3Lf kilometers square\n", (on_fire_per * cell_num / 100) * 4 * 4.19);
}

// Grafický výstup aktuálneho stavu
void grassland::graphic_output(int index) {

    constexpr auto dimx = 1000, dimy = 920;
    cell* ac_cell;
    long double fuel;

    index += 100;

    string file_name = "output-";
    file_name.append(to_string(index));
    file_name.append(".ppm");

    ofstream ofs(file_name, ios_base::out | ios_base::binary);
    ofs << "P6" << endl << dimx << ' ' << dimy << endl << "255" << endl;

    for (auto j = 0; j < dimy; ++j)
        for (auto & i : country){
            ac_cell = i[j];
            if (ac_cell == nullptr){
                ofs << (char) 255 << (char) 255 << (char) 255;
            } else if (ac_cell->is_destroyed()) {
                ofs << (char) (0) << (char) 0 << (char) 0;
            } else if (ac_cell->av_fire_load() < general_fuel_load) {
                fuel = ac_cell->av_fire_load();
                if (fuel < (general_fuel_load / 8))
                    ofs << (char) (255) << (char) 206 << (char) 0;
                else if (fuel < (general_fuel_load / 4))
                    ofs << (char) (255) << (char) 154 << (char) 0;
                else if (fuel < (general_fuel_load / 2))
                    ofs << (char) (255) << (char) 90 << (char) 0;
                else
                    ofs << (char) (255) << (char) 0 << (char) 0;
            } else {
                ofs << (char) 179 << (char) 222 << (char) 142;
            }
        }

    ofs.close();
}

// Funkcia zodpovedajúca za rozmiestnenie počiatočných ohňov
void grassland::ignite_country(unsigned int start_fires) {
    printf("---Set up fires---\n");
    int max = 2;
    for (int i = 0; i < start_fires; i++){
        if (!ignite(random_x(), random_y())){
            i--;
        } else {
            if ((i * 1.0 / start_fires * 100.0) > max) {
                cout << "[ " << max << "% ] [ ";
                for (int j = 0; j < max; j+=2)
                    cout << '#';
                for (int j = max; j < 100; j+=2)
                    cout << '.';
                cout << " ]" <<'\r' << flush;
                max += 2;
            }
        }
    }
    cout << "[ 100% ] [ ";
    for (int j = 0; j < 50; j++)
        cout << "#";
    cout << " ]";
}

// Funkcia riadiaca samotnú simuláciu
void grassland::simulation(unsigned int hours, bool graphic_out) {
    printf("\n\n---Processing simulation---\n");
    int max = 2;

    // Pre každú hodinu je vypočítaný nový stav
    for (int i = 0; i < hours; i++){
        update_state();
        if (graphic_out)
            graphic_output(i);

        if ((i * 1.0 / hours * 100.0) > max) {
            cout << "[ " << max << "% ] [ ";
            for (int j = 0; j < max; j+=2)
                cout << '#';
            for (int j = max; j < 100; j+=2)
                cout << '.';
            cout << " ]" <<'\r' << flush;
            max += 2;
        }
    }
    cout << "[ 100% ] [ ";
    for (int j = 0; j < 50; j++)
        cout << "#";
    cout << " ]";

    printf("\n\n---Simulation completed---\n\n");
}

// Funkcie pre náhodné stanovenie
// Pravdepodobnosti vzbĺknutia
double grassland::random_prob(){
    static random_device rd;
    static mt19937 mt(rd());
    static uniform_real_distribution<double> dist(0, 1);

    return dist(mt);
}

// Súradnice x
int grassland::random_x(){
    static random_device rd;
    static mt19937 mt(rd());
    static uniform_int_distribution<int> dist(0, X_SIZE);

    return dist(mt);
}

// Súradnice y
int grassland::random_y(){
    static random_device rd;
    static mt19937 mt(rd());
    static uniform_int_distribution<int> dist(0, Y_SIZE);

    return dist(mt);
}