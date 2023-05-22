# IMS

[IMS - Modelování a simulace](https://www.fit.vut.cz/study/course/IMS/.cs)

Repozitár obsahuje kompletné riešenie projektu z predmetu IMS. Cieľom bolo implementovať program simulujúci vývoj lesných požiarov v Austrálií na základe rôznych podmienok. Podrobný rozbor zadania projektu ako aj riešenia obsahuje súbor **dokumentacia.pdf**. Príklad grafického výstupu simulácie je obsiahnutý v súbore **animacia.gif**.

[IMS - Modelling and Simulation](https://www.fit.vut.cz/study/course/IMS/.en)

The repository contains the complete solution of the IMS project. The aim was to implement a program simulating the evolution of forest fires in Australia under different conditions. A detailed discussion of the project brief as well as the solution is contained in the file **dokumentacia.pdf**. An example of the graphical output of the simulation is contained in the file **animacia.gif**.

## Spustenie programu

Priemerné hodnoty v simulácií je možné  ovplyvňovať nasledujúcimi voliteľnými argumentmi:

 - g :  generovanie grafického výstupu pre každý krok simulácie
 - f : priemerné množstvo horľavého materiálu v tonách na hektár, predvolená hodnota je 4,9
 - t : priemerná teplota v simulácií, predvolená hodnota je 26°C
 - sf : počet ohňov založených na začiatku simulácie, predvolená hodnota je 50
 - h : doba trvania simulácie v hodinách, predvolená hodnota je 96 hodín

Nasledujúcimi parametrami je možné nahradzovat’ skutočné hodnoty zo vstupného súboru dát **cordinates.txt**, čím je možné napríklad simulovať extrémne podmienky s vysokým priemerným GFDI indexom.

 - w  :  priemerná rýchlosť vetra v km/h
 - rh  :  relatívna vlhkost’ vzduchu
 - c  :  vysušenie pôdy

Príklad spustenie programu potom môže vyzerat’ nasledovne:

    ./ims –g –f 3,5 –t 35 –h 115

## Running the program

The average values in the simulation can be influenced by the following optional arguments:

 - g : generate graphical output for each step of the simulation
 - f : average amount of combustible material in tonnes per hectare, default value is 4.9
 - t : average temperature in the simulation, default value is 26°C
 - sf : number of fires established at the start of the simulation, default value is 50
 - h : simulation duration in hours, default value is 96 hours

The following parameters can be used to replace the actual values from the input data file **cordinates.txt**, for example to simulate extreme conditions with a high average GFDI index. 

 - w : average wind speed in km/h
 - rh : relative humidity
 - c : soil dryness

An example program execution may then look like the following:

    ./ims -g -f 3,5 -t 35 -h 115
