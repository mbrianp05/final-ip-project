#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <mem.h>
#include <string.h>
#include <math.h>
#include <logica/types.h>

// Valida que el string sea un número adecuado
// Esta función será útil para parsear el string a número
// de forma segura.
bool isNumber(const char s[]) {
    bool isValid = true;
    bool isFloat = false;
    u32 i = 0;

    // Si el primer char es un - entonces saltárselo
    if (s[i] == '-') i++;
    // El primer digito (o el segundo si hay un "-") no puede ser 0
    isValid = isdigit(s[i]);
    u32 length = strlen(s);

    // Validar que el número solo pueda empezar por 0 si el siguiente caracter es un .
    // o sea el único dígito del número
    if (s[i] == '0' && s[i + 1] != '.' && i + 1 != length) isValid = false;
    i++;

    while (s[i] != '\0' && isValid) {
        isValid = isdigit(s[i]);

        // Si se encuentra otro punto en el mismo número
        // entonces el número es incorrecto
        if (isFloat && s[i] == '.') isValid = false;

        // Permitir un punto en el número
        // Si aparece un . entonces isFloat pasa a true
        if (s[i] == '.' && !isFloat) {
            isValid = true;
            isFloat = true;
        }

        i++;
    }

    return isValid;
}

// Parsea un string a un float
// Si el string es un número se devuelve true
// y se le asigna al puntero el valor numérico.
// Si no lo es, se devuelve false.
bool parseFloat(const char x[], float *num) {
    bool valid = isNumber(x);
    if (valid) *num = atof(x);

    return valid;
}

// Verifica que un caracter se encuentra
// en un string o array de caracteres
bool has(const char items[], char x, u32 size) {
    bool exists = false;
    u32 i = 0;

    while (i < size && !exists) {
        exists = items[i++] == x;
    }

    return exists;
}

// Implementacion de la típica función trim que existe en cualquier lenguaje :)
// elimina caracteres basura (espacios, salto de lineas, etc) del principio y final
// de un string, función especialmente útil para "sanear" la entrada del stdin
// que siempre terminan en \n y podrían tener espacios accidentales.
void trim(char s[]) {
    u32 size = strlen(s);
    int i = 0;
    char trash[] = { '\n', '\r', ' ' };
    // Verificar si el caracter es uno de los de trash
    // Si lo es sigue al siguiente, si no lo es, se termina el ciclo
    // Y la variable i se queda en el índice del primer caracter no basura + 1
    while (has(trash, s[i++], len(trash)));

    i -= 1;
    // Correr hacia la izquierda, es decir al inicio del string
    // la cantidad de caracteres basura quitados
    memcpy(s, s + i, size - i + 1);

    // Poner el valor sentinela del string en el último índice
    s[size - i + 1] = '\0';
    // Obtener el último índice del string
    i = strlen(s) - 1;

    // Proceso análogo pero empezando por la derecha
    while (has(trash, s[i--], len(trash)));
    s[i + 2] = '\0';
}

// Funciones de validacion de que un número sea un entero
bool isInt(const char s[]) {
    float parse;
    bool result = false;

    if (parseFloat(s, &parse)) {
        result = (int)parse - parse == 0;
    }

    return result;
}

// Funciones de validacion de que un número sea positivo
bool isPositive(const char s[]) {
    float parse;
    bool result = false;

    if (parseFloat(s, &parse)) {
        result = parse >= 0;
    }

    return result;
}

// Numeros positivos con un valor sentinela negativo
bool notNegSentinel(const char s[]) {
    const int sentinelValue = SENTINEL;
    bool result = false;
    float parse;

    if (parseFloat(s, &parse)) {
        result = parse >= 0 || parse == sentinelValue;
    }

    return result;
}

// Ejecuta todas las funciones de tipp fn_ptr pasadas por parametro
// Si todas devuelven true al ejecutarlas con el string de la entrada del usuario
// entonces se devuelve true
bool all(const char s[], fn_ptr *constraints, u32 size) {
    bool assertion = true;
    u32 i = 0;

    while (i < size && assertion) {
        fn_ptr fn = constraints[i];
        assertion = fn(s);

        i++;
    }

    return assertion;
}

bool verifyChanges(Mortality *data) {
    u32 i = 0;
    bool hasChanged = false;

    while (i < PROVINCES && !hasChanged) {
        hasChanged = data[i].births > 0;
        i++;
    }

    return hasChanged;
}

char *causeName(MortalityCausesMenu cause) {
    char *causeName;

    switch (cause) {
    case ChildBirthDeath:
        causeName = "durante el parto";
        break;
    case OneMonthDeath:
        causeName = "durante el primer mes de vida";
        break;
    case UnderOneYearDeath:
        causeName = "entre el segundo mes y el primer a\244o de vida";
        break;
    default:
        causeName = "";
        // Inalcanzable ya que la opcion de salida se valida antes
        // Pongo el default para quitar el warning del QT
        break;
    }

    return causeName;
}

// En C los string literals se pueden devolver con seguridad
// ya que aunque sean un puntero a datos creado dentro de la funcion
// los strings literals son parte del binario por lo que estan siempre
// disponibles en el programa
// A diferencia de los otros tipos que se guardan en el stack de la funcion
char *provinceName(Province province) {
    char *name;

    switch (province) {
    case Habana:
        name = "La Habana";
        break;
    case PinarDelRio:
        name = "Pinar del R\241o";
        break;
    case Mayabeque:
        name = "Mayabeque";
        break;
    case Artemisa:
        name = "Artemisa";
        break;
    case Matanzas:
        name = "Matanzas";
        break;
    case Cienfuegos:
        name = "Cienfuegos";
        break;
    case VillaClara:
        name = "Villa Clara";
        break;
    case CiegoDeAvila:
        name = "Ciego de \265vila";
        break;
    case LasTunas:
        name = "Las Tunas";
        break;
    case SanctiSpiritus:
        name = "Sancti Sp\241ritus";
        break;
    case Camaguey:
        name = "Camag\201ey";
        break;
    case Holguin:
        name = "Holgu\241n";
        break;
    case Granma:
        name = "Granma";
        break;
    case Santiago:
        name = "Santiago de Cuba";
        break;
    case Guantanamo:
        name = "Guant\240namo";
        break;
    case IslaJuventud:
        name = "Isla de la Juventud";
        break;
    default:
        name = "";
    }

    return name;
}

char *shortenedProvinceName(Province province) {
    char *name;

    switch (province) {
    case Habana:
        name = "Hab";
        break;
    case PinarDelRio:
        name = "Pna";
        break;
    case Mayabeque:
        name = "May";
        break;
    case Artemisa:
        name = "Atm";
        break;
    case Matanzas:
        name = "Mat";
        break;
    case Cienfuegos:
        name = "Cfg";
        break;
    case VillaClara:
        name = "Vcl";
        break;
    case CiegoDeAvila:
        name = "Cga";
        break;
    case LasTunas:
        name = "Tna";
        break;
    case SanctiSpiritus:
        name = "Scp";
        break;
    case Camaguey:
        name = "Cmg";
        break;
    case Holguin:
        name = "Hln";
        break;
    case Granma:
        name = "Gma";
        break;
    case Santiago:
        name = "Sgo";
        break;
    case Guantanamo:
        name = "Gtm";
        break;
    case IslaJuventud:
        name = "Isv";
        break;
    default:
        name = "";
    }

    return name;
}

void setDeathsData(Mortality *data, MortalityCausesMenu cause, u32 deaths) {
    switch (cause) {
        case ChildBirthDeath:
            data->childBirthDeaths = deaths;
            break;
        case OneMonthDeath:
            data->oneMonthOfAgeDeaths = deaths;
            break;
        default:
            data->underOneYearOfAgeDeaths = deaths;
    }
}

void setDefaultValues(Mortality *mortalityData) {
    mortalityData->births = 0;
    mortalityData->childBirthDeaths = 0;
    mortalityData->oneMonthOfAgeDeaths = 0;
    mortalityData->underOneYearOfAgeDeaths = 0;
}

void setBinaryOptions(Option *options) {
    options[0] = (Option) { Affirmative, "S\241" };
    options[1] = (Option) { Negative, "No" };
}

void setMainOptions(Option *options) {
    options[0] = (Option){ InsertMortalityData, "Ingresar datos de mortalidad" };
    options[1] = (Option){ CountrywideMortalityRate, "Conocer la tasa de mortalidad del pa\241s" };
    options[2] = (Option){ ProvincesDataList, "Listado de datos por provincia" };
    options[3] = (Option){ ProvinceWithLeastDeathsFrom, "La provincia con menor fallecidos de..." };
    options[4] = (Option){ CountrywideDeathsFrom, "Cantidad nacional de fallecidos de..." };
    options[5] = (Option){ SortProvincesWithMostDeathsInOneMonth, "Provincias con mayor cantidad de fallecidos durante el primer mes de vida" };
    options[6] = (Option){ ExitMainScreen, "\236 Salir" };
}

void setCauseOptions(Option *options) {
    options[0] = (Option){ ChildBirthDeath, "Fallecimiento durante el parto" };
    options[1] = (Option){ OneMonthDeath, "Fallecimiento al mes de nacido" };
    options[2] = (Option){ UnderOneYearDeath, "Fallecimiento entre el segundo mes y el a\244o" };
    options[3] = (Option){ ExitMortalityCauses, "\256 Volver" };
}

// Estado inicial de la aplicacion donde todas las provincias tienen los datos en 0
void initState(Mortality *data) {
    // Datos randoms
    data[0] = (Mortality){ PinarDelRio, 0, 0, 0, 0 };
    data[1] = (Mortality){ Artemisa, 0, 0, 0, 0 };
    data[2] = (Mortality){ Habana, 0, 0, 0, 0 };
    data[3] = (Mortality){ Mayabeque, 0, 0, 0, 0 };
    data[4] = (Mortality){ Matanzas, 0, 0, 0, 0 };
    data[5] = (Mortality){ Cienfuegos, 0, 0, 0, 0 };
    data[6] = (Mortality){ VillaClara, 0, 0, 0, 0 };
    data[7] = (Mortality){ SanctiSpiritus, 0, 0, 0, 0 };
    data[8] = (Mortality){ CiegoDeAvila, 0, 0, 0, 0 };
    data[9] = (Mortality){ Camaguey, 0, 0, 0, 0 };
    data[10] = (Mortality){ LasTunas, 0, 0, 0, 0 };
    data[11] = (Mortality){ Granma, 0, 0, 0, 0 };
    data[12] = (Mortality){ Holguin, 0, 0, 0, 0 };
    data[13] = (Mortality){ Santiago, 0, 0, 0, 0 };
    data[14] = (Mortality){ Guantanamo, 0, 0, 0, 0 };
    data[15] = (Mortality){ IslaJuventud, 0, 0, 0, 0 };
}

void mockCountrywideData(Mortality *data) {
    // Datos randoms
    data[0] = (Mortality){ PinarDelRio, 3745, 189, 100, 95 };
    data[1] = (Mortality){ Artemisa, 1030, 115, 56, 24 };
    data[2] = (Mortality){ Habana, 4012, 210, 110, 65 };
    data[3] = (Mortality){ Mayabeque, 3000, 102, 91, 75 };
    data[4] = (Mortality){ Matanzas, 3800, 190, 32, 99 };
    data[5] = (Mortality){ Cienfuegos, 2021, 168, 10, 165 };
    data[6] = (Mortality){ VillaClara, 1099, 100, 56, 21 };
    data[7] = (Mortality){ SanctiSpiritus, 1022, 86, 74, 21 };
    data[8] = (Mortality){ CiegoDeAvila, 1213, 44, 201, 25 };
    data[9] = (Mortality){ Camaguey, 3958, 395, 240, 110 };
    data[10] = (Mortality){ LasTunas, 2768, 54, 56, 88 };
    data[11] = (Mortality){ Granma, 938, 4, 67, 34 };
    data[12] = (Mortality){ Holguin, 1430, 329, 21, 129 };
    data[13] = (Mortality){ Santiago, 2543, 23, 345, 32 };
    data[14] = (Mortality){ Guantanamo, 1200, 123, 45, 18 };
    data[15] = (Mortality){ IslaJuventud, 540, 10, 35, 12 };
}

u32 findMortalityByProvince(Mortality *mortalityData, Province province) {
    u32 i = 0;
    while (i < PROVINCES && mortalityData[i].province != province) i++;

    return i;
}

u32 nationalBirths(Mortality *data) {
    u32 births = 0;

    u32 i;
    for (i = 0; i < PROVINCES; i++) {
        births += data[i].births;
    }

    return births;
}

u32 provinceTotalDeaths(Mortality data) {
    return data.childBirthDeaths + data.oneMonthOfAgeDeaths + data.underOneYearOfAgeDeaths;
}

u32 deathsFrom(Mortality data, MortalityCausesMenu *cause, u32 size) {
    u32 deaths = 0;
    u32 i;

    for (i = 0; i < size; i++) {
        switch (cause[i]) {
        case ChildBirthDeath:
            deaths += data.childBirthDeaths;
            break;
        case OneMonthDeath:
            deaths += data.oneMonthOfAgeDeaths;
            break;
            // Fuera de la funcion nos aseguramos que la opcion de salida del menu este descartada
        default:
            deaths += data.underOneYearOfAgeDeaths;
        }
    }

    return deaths;
}

// Calcula las muertes de todo el pais por las causas de muertes pasadas por parametros
u32 nationalDeathsFrom(Mortality *data, MortalityCausesMenu *causes, u32 size) {
    u32 deaths = 0;
    u32 i;

    // Por cada provincia
    for (i = 0; i < PROVINCES; i++) {
        deaths += deathsFrom(data[i], causes, size);
    }

    return deaths;
}

u32 nationalDeaths(Mortality *data) {
    MortalityCausesMenu causes[] = {
        ChildBirthDeath,
        OneMonthDeath,
        UnderOneYearDeath,
    };

    return natDeathsFrom(data, causes);
}

Mortality regionWithLeastDeathsFrom(Mortality *data, MortalityCausesMenu cause) {
    Mortality provinceMortality = data[0];
    u32 i;

    for (i = 1; i < PROVINCES; i++) {
        if (deathsFrom(provinceMortality, &cause, 1) > deathsFrom(data[i], &cause, 1)) {
            provinceMortality = data[i];
        }
    }

    return provinceMortality;
}


// Calcula la tasa de mortalidad por cada 1000 nacidos
double mortalityRate(Mortality *data) {
    // Nacimientos totales
    u32 births = nationalBirths(data);
    // Muertes totales
    u32 deaths = nationalDeaths(data);

    double rate = 0;

    if (deaths != 0) {
        rate = (float)deaths / births * 1000;
    }

    return rate;
}

u32 countDigits(int x) {
    u32 i = 0;
    x = abs(x);

    while ((int)(x / pow(10, i)) > 0) {
        i++;
    }

    if (x == 0) i = 1;

    return i;
}

// Ordenar los datos de mortalidad de forma descendente segun las muertes al primer mes de vida
void sortByDeathInOneMonth(Mortality *data) {
    u32 i;
    for (i = 0; i < PROVINCES; i++) {
        u32 j;
        for (j = 0; j < PROVINCES; j++) {
            if (j + 1 != PROVINCES) {
                if (data[j].oneMonthOfAgeDeaths < data[j + 1].oneMonthOfAgeDeaths) {
                    Mortality temp = data[j + 1];
                    data[j + 1] = data[j];
                    data[j] = temp;
                }
            }
        }

    }
}
