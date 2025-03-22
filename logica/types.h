#include <stdbool.h>

#ifndef TYPES_H
#define TYPES_H

// Calcular la longitud de un array
// Como es una macro este código se inserta
// en donde se llama la macro
// por lo que el array no se convierte en un puntero
#define len(array) sizeof(array) / sizeof(array[0])
// Una macro para hacer el codigo mas legible y omitir pasar los sizes
#define readInto(buffer, constraints) input(buffer, len(buffer), constraints, len(constraints))
#define readFloat(constraints) inputFloat(constraints, len(constraints))
#define natDeathsFrom(data, causes) nationalDeathsFrom(data, causes, len(causes))

#define BIRTHS_OPTION_VALUE 110
#define RESET_OPTION_VALUE 100
#define SENTINEL -1
#define CAUSES_NUMBER 3
#define MORTALITY_MENU CAUSES_NUMBER + 1
#define MAIN_SCREEN_OPTIONS 7

#define TABLE_COLUMNS 6

#define PROVINCES 16

// Enum para las provincias
typedef enum {
    PinarDelRio,
    Artemisa,
    Habana,
    Mayabeque,
    Matanzas,
    Cienfuegos,
    VillaClara,
    SanctiSpiritus,
    CiegoDeAvila,
    Camaguey,
    LasTunas,
    Granma,
    Holguin,
    Santiago,
    Guantanamo,
    IslaJuventud,
} Province;

// Enum para las opciones de las causas de muerte
typedef enum {
    ChildBirthDeath,
    OneMonthDeath,
    UnderOneYearDeath,
    ExitMortalityCauses,
} MortalityCausesMenu;

// Enum para las opciones de la primera pantalla
typedef enum {
    InsertMortalityData,
    CountrywideMortalityRate,
    ProvincesDataList,
    ProvinceWithLeastDeathsFrom,
    CountrywideDeathsFrom,
    SortProvincesWithMostDeathsInOneMonth,
    ExitMainScreen,
} MainScreenMenu;

typedef enum {
    Affirmative,
    Negative,
} BinaryMenu;

typedef bool (*fn_ptr)(const char[]);

// Definicion de numeros enteros positivos
typedef unsigned int u32;

typedef struct {
    int value;
    char *label;
} Option;

typedef struct {
    Province province;
    u32 births;
    // Cantidad de muertes por diferentes causas
    u32 childBirthDeaths;
    u32 oneMonthOfAgeDeaths;
    u32 underOneYearOfAgeDeaths;
} Mortality;

#endif
