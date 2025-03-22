#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <stdbool.h>
#include <unistd.h>
#include <logica/types.h>
#include <logica/logica.h>


// Caracteres de escape ANSI para darle color al texto
// \x1[32m es el color ver \x1b[0m resetea al color por defecto de la terminal
void success(const char *output) {
    // Descomentar linea en Win11
    //    printf("\x1b[32m%s\x1b[0m", output);
    printf("%s", output);
}

void error(const char *output) {
    // Descomentar linea en Win11
    //    printf("\x1b[31m%s\x1b[0m", output);
    printf("%s", output);
}

void warning(const char *output) {
    // Descomentar linea en Win11
    //    printf("\x1b[33m%s\x1b[0m", output);
    printf("%s", output);
}

void info(const char *output) {
    // Descomentar linea en Win11
    //   printf("\x1b[36m%s\x1b[0m", output);
    printf("%s", output);
}

void awaitKey() {
    system("pause");
}

void clearScreen() {
    system("cls");
}

// Para las validaciones use un enfoque funcional inspirado en las API de javascript
// donde por ejemplo puedes filtrar un array con .filter() pasando una funcion de parametro
// que devuelva true o false en dependencia de si el valor cumple una condicion determinada
// En este caso segui un enfoque parecido
// Como pueden existen muchas validaciones para la entrada
// decidi pasar como parametro funciones que aceptan un string de parametro y devuelven un bool
// El input será válido si una vez evaluda todas las funciones, todas devuelven true
// Este enfoque ahorra codigo ya que es una abstracción general para todas los tipos de validaciones
// De otra menera hubiese tenido que repetir una función de input para cada validación con el
// codigo del do-while, la variable valid, la variable x, el fgets, etc.
void input(char buffer[], u32 bytes, fn_ptr *constraints, u32 size) {
    bool valid;

    do {
        // Obtener la entrada en forma de string y colocarla en el buffer
        fgets(buffer, bytes, stdin);
        // Eliminar saltos de linea y espacios finales
        trim(buffer);
        // Realizar todas las validaciones
        valid = all(buffer, constraints, size);

        if (!valid) {
            error("El dato de entrada no es correcto\n");
            fflush(stdin);
        }
    } while (!valid);
}

float inputFloat(fn_ptr *constraints, u32 size) {
    float parse;
    char buffer[45];

    input(buffer, len(buffer), constraints, size);
    parseFloat(buffer, &parse);

    return parse;
}

int inputIntNotNegSentinel() {
    fn_ptr constraints[] = { isInt, notNegSentinel };
    // No se puede castear ya que el valor sentinela si tiene signo
    return readFloat(constraints);
}

void printPadding(char x, u32 times) {
    u32 i = 0;
    while (i < times) {
        printf("%c", x);
        i++;
    }
}

void heading(const char *text) {
    u32 length = strlen(text);

    // Dejar un padding horizontal de 1 espcio
    printPadding('\304', length + 2);
    printf("\n %s\n", text);
    printPadding('\304', length + 2);
    printf("\n");
}

u32 menu(Option *options, u32 size) {
    // Memoria suficiente para el buffer 5 bytes
    u32 i;
    u32 maxSize = countDigits(size);

    for (i = 0; i < size; i++) {
        printf("\257 %d. ", i + 1);
        // Alinear los elementos añandiendo espcios
        printPadding(' ', maxSize - countDigits(i + 1));
        printf("%s\n", options[i].label);
    }

    fn_ptr constraints[] = { isInt, isPositive };
    u32 choice;
    bool inRange = true;

    do {
        choice = (u32)readFloat(constraints);
        inRange = choice > 0 && choice <= size;

        if (!inRange) {
            char text[125];
            sprintf(text, "Las opciones posibles se encuentran en el rango de 1 a %d. Intente de nuevo\n", size);
            error(text);
            fflush(stdin);
        }
    } while (!inRange);

    u32 optionIndex = choice - 1;

    return options[optionIndex].value;
}

void showData(Mortality data, MortalityCausesMenu cause) {
    char buffer[50];
    u32 stat = deathsFrom(data, &cause, 1);

    if (cause == BIRTHS_OPTION_VALUE) {
        stat = data.births;
    }

    sprintf(buffer, "%-5d ", stat);
    info(buffer);
}

void showMortalityData(Mortality data) {
    printf("Datos actuales de la provincia\n");
    printf("\372 Nacidos: ");
    showData(data, BIRTHS_OPTION_VALUE);

    printf("\372 Fallecidos: En el parto ");
    showData(data, ChildBirthDeath);

    printf("Al primer mes ");
    showData(data, OneMonthDeath);

    printf("Antes del a\244o ");
    showData(data, UnderOneYearDeath);

    printf("\n\n");
}

Province provincesMenu() {
    printf("Escoja la provincia\n");
    Option options[] = {
        { PinarDelRio, provinceName(PinarDelRio) },
        { Artemisa, provinceName(Artemisa) },
        { Habana, provinceName(Habana) },
        { Mayabeque, provinceName(Mayabeque) },
        { Matanzas, provinceName(Matanzas) },
        { Cienfuegos, provinceName(Cienfuegos) },
        { VillaClara, provinceName(VillaClara) },
        { SanctiSpiritus, provinceName(SanctiSpiritus) },
        { CiegoDeAvila, provinceName(CiegoDeAvila) },
        { Camaguey, provinceName(Camaguey) },
        { LasTunas, provinceName(LasTunas) },
        { Holguin, provinceName(Holguin) },
        { Santiago, provinceName(Santiago) },
        { Granma, provinceName(Granma) },
        { Guantanamo, provinceName(Guantanamo) },
        { IslaJuventud, provinceName(IslaJuventud) },
    };

    return menu(options, PROVINCES);
}

MortalityCausesMenu causesMenu() {
    printf("Seleccione la causa de muerte\n");
    Option options[MORTALITY_MENU];
    setCauseOptions(options);

    return menu(options, MORTALITY_MENU);
}

u32 insertionMenu() {
    Option options[MORTALITY_MENU + 2];
    setCauseOptions(options);

    u32 i;
    // Corre hacia la derecha todos las opciones
    // para dejar hueco en la primer indidice a la opcion de
    // nacimientos
    for (i = MORTALITY_MENU; i >= 1; i--) {
        options[i] = options[i - 1];
    }

    options[MORTALITY_MENU + 1] = options[MORTALITY_MENU];
    // Agregar dos opciones mas
    // La opcion de resetear los valores y la de actualizar los nacimientos
    options[MORTALITY_MENU] = (Option){ RESET_OPTION_VALUE, "Restablecer a valores predeterminados" };
    options[0] = (Option){ BIRTHS_OPTION_VALUE, "Nacimientos" };

    printf("Indique el dato que desea registrar o actualizar\n");

    return menu(options, MORTALITY_MENU + 2);
}

BinaryMenu confirm() {
    Option options[2];
    setBinaryOptions(options);

    return menu(options, 2);
}

void displayInsertionHeading(Mortality mortalityData) {
    char *name = provinceName(mortalityData.province);
    char text[50];
    sprintf(text, "Provincia %s", name);

    heading(text);
    showMortalityData(mortalityData);
}

// Registra la cantidad de nacimientos y devuelve true
// Si el usuario introduce el valor sentinela se devuelve false
bool inputBirths(Mortality *mortalityData) {
    printf("Indique la cantidad de nacimientos o -1 para volver\n");

    bool stop = false;
    bool recorded = false;

    u32 totalDeaths = provinceTotalDeaths(*mortalityData);

    do {
        int userInput = inputIntNotNegSentinel();

        if (userInput != SENTINEL) {
            // La cantidad de nacimientos no puede ser inferior a la de fallecidos
            if ((u32)userInput >= totalDeaths) {
                mortalityData->births = (u32)userInput;
                success("Dato de natalidad registrado correctamente!\n\n");
                awaitKey();
                stop = true;
                recorded = true;
            } else error("La cantidad de nacimientos no puede ser inferior a la cantidad de fallecimientos\nIntente de nuevo\n\n");
        } else {
            stop = true;
        }
    } while (!stop);

    return recorded;
}

void inputDeaths(Mortality *mortalityData, MortalityCausesMenu cause) {
    bool stop = false;

    do {
        printf("Indique los fallecimientos %s o -1 para cancelar\n", causeName(cause));
        u32 deaths = provinceTotalDeaths(*mortalityData);
        int userInput = inputIntNotNegSentinel();

        if (userInput != SENTINEL) {
            // La cantidad de fallecidos tiene que
            if ((u32)userInput + deaths <= mortalityData->births) {
                setDeathsData(mortalityData, cause, userInput);
                success("Se ha registrado el dato correctamente\n\n");
                awaitKey();
                stop = true;
            } else {
                error("La cantidad total de fallecidos supera los nacimientos. Intente de nuevo\n\n");
            }
        } else {
            clearScreen();
            stop = true;
        }
    } while (!stop);
}

void handleReset(Mortality *mortalityData) {
    // Preguntar si esta seguro de realizar la operacion
    warning("Est\240 seguro de restablecer los valores?\n");
    BinaryMenu choice = confirm();

    if (choice == Affirmative) {
        setDefaultValues(mortalityData);
        info("Los datos han sido restablecidos a 0\n\n");
    }

    awaitKey();
}

void retrieveData(Mortality *mortalityData) {
    displayInsertionHeading(*mortalityData);

    // Si la cantidad de nacimientos es 0 entonces no
    // mostrar las otras opciones y forzar la introduccion de los nacimientos
    if (mortalityData->births > 0) {
        u32 registry = insertionMenu();

        if (registry != ExitMortalityCauses) {
            switch (registry) {
            case BIRTHS_OPTION_VALUE:
                inputBirths(mortalityData);
                break;
            case RESET_OPTION_VALUE:
                handleReset(mortalityData);
                break;
            default:
                inputDeaths(mortalityData, registry);
            }

            clearScreen();
            retrieveData(mortalityData);
        }
    } else if (inputBirths(mortalityData)) {
        clearScreen();
        retrieveData(mortalityData);
    }
}

void handleInsertion(Mortality *mortalityData) {
    Province province = provincesMenu();
    clearScreen();
    u32 index = findMortalityByProvince(mortalityData, province);
    retrieveData(&mortalityData[index]);
}

void showMortalityRate(double rate) {
    char text[100];
    // Hacer el formato del texto fuera y pasar el resultado por parametro
    sprintf(text, "La mortalidad infantil en el pa\241s es de %.4f por cada mil nacidos", rate);
    heading(text);
}

void showDeathsFrom(u32 deaths, MortalityCausesMenu cause) {
    heading("A nivel nacional");
    printf("Hubo un total de %d muertes %s\n", deaths, causeName(cause));
}

void showBestProvince(Province province, MortalityCausesMenu cause, u32 deaths) {
    char *provinceStr = provinceName(province);
    char *causeLabel = causeName(cause);

    char buffer[100];
    // Cambiar esto
    // Arreglar opcion numero 4
    sprintf(buffer, "%s obtuvo los mejores resultados!", provinceStr);
    heading(buffer);

    printf("En la provincia ");

    if (deaths == 0) {
        printf("no hubo ning\243n fallecido %s\n", causeLabel);
    } else {
        printf("solo hubo %d fallecimientos %s\n", deaths, causeLabel);
    }
}

void printTable(const Mortality* data) {
    // Almacenar por cada provincia:
    // El nombre de la provincia
    // Cantidad de nacidos
    // Cantidad de fallecidos
    // Cantidad de fallecidos desglosada por causas
    // En total 6 columnas
    // Y como se necesita una cabecera para la tabla
    // entonces la cantidad de filas es PROVINCES + 1
    char table[PROVINCES + 1][6][50];

    MortalityCausesMenu causes[] = {
        ChildBirthDeath,
        OneMonthDeath,
        UnderOneYearDeath,
    };

    strcpy(table[0][0], "Provincias");
    strcpy(table[0][1], "Nacidos");
    strcpy(table[0][2], "Fallecidos");
    strcpy(table[0][3], "En el parto");
    strcpy(table[0][4], "Primer mes");
    strcpy(table[0][5], "Al a\244o");

    u32 i;
    for (i = 0; i < PROVINCES; i++) {
        strcpy(table[i + 1][0], shortenedProvinceName(data[i].province));
        sprintf(table[i + 1][1], "%d", data[i].births);
        sprintf(table[i + 1][2], "%d", deathsFrom(data[i], causes, CAUSES_NUMBER));
        sprintf(table[i + 1][3], "%d", data[i].childBirthDeaths);
        sprintf(table[i + 1][4], "%d", data[i].oneMonthOfAgeDeaths);
        sprintf(table[i + 1][5], "%d", data[i].underOneYearOfAgeDeaths);
    }

    printf("\332"); // Esquina arriba izquierda

    for (i = 0; i < TABLE_COLUMNS; i++) {
        // 21 es el tamanio de cada celda
        printPadding('\304', 13);

        if (i + 1 != TABLE_COLUMNS) {
            printf("\302"); // Punta hacia abajo
        } else {
            // Si es el caracter final porner el caracter de esquina
            printf("\277"); // Esquina derecha
        }
    }

    printf("\n");

    u32 l;
    u32 k = 1000;

    for (i = 0; i < PROVINCES + 1; i++) {
        u32 j;
        for (j = 0; j < TABLE_COLUMNS; j++) {
            printf("\263 %-11s ", table[i][j]);
        }

        printf("\263\n"); // Carater barra vertical

        if (i == 0) {
            printf("\303"); // Caracter en forma de T hacia fuera

            for (l = 0; l < TABLE_COLUMNS; l++) {
                printPadding('\304', 13); // Barra horizontal

                if (l + 1 != TABLE_COLUMNS) {
                    printf("\305"); // Punta hacia arriba y abajo
                } else {
                    // Si es el caracter final porner el caracter de esquina
                    printf("\264"); // Punta hacia dentro
                }
            }
            printf("\n");
        }

        // 600ms para mostrar la siguiente fila
        usleep(100 * k);
        // Reducir el tiempo de la animacion
        if  (k > 100) {
            k -= 75;
        }
    }

    printf("\300");

    for (l = 0; l < TABLE_COLUMNS; l++) {
        printPadding('\304', 13); // Barra horizontal

        if (l + 1 != TABLE_COLUMNS) {
            printf("\301"); //
        } else {
            // Si es el caracter final porner el caracter de esquina
            printf("\331"); // Punta hacia dentro
        }
    }

    printf("\n");
}

void printChart(Mortality *data) {
    printf("\n");

    // El array esta ordenado de forma descendente
    // por lo que el min esta en el ultimo indice
    u32 min = data[PROVINCES - 1].oneMonthOfAgeDeaths;
    u32 max = data[0].oneMonthOfAgeDeaths;
    u32 diff = max - min;
    u32 num = max;
    u32 fractions = countDigits(diff);

    // Coeficiente de escala
    u32 k = diff / fractions;

    u32 j = 0;
    u32 i = 1;

    const u32 ySeparation = 5;

    u32 spaces = countDigits(max) + 1;
    u32 height = (ySeparation * fractions) + ySeparation + 1;

    // X axis
    const u32 xSlotSize = 4;
    const u32 xSlots = PROVINCES;

    printPadding(' ', spaces - 2);
    printf("Muertes\n\n");

    while (i < height) {
        // Dibujar el eje y
        if (i % ySeparation == 0 && num - k * j > 0) {
            printPadding(' ', spaces - countDigits(num - k * j));
            printf("%d ", num - k * j);
            printf("\264");

            j++;
        } else {
            printPadding(' ', spaces);
            printf(" \263");
        }

        if (max != 0) {
            // Dibujar las barras
            u32 k;
            for (k = 0; k < PROVINCES; k++) {
                float rate = (float)data[k].oneMonthOfAgeDeaths / max;
                float barHeight = rate * (height - ySeparation);
                float heightDiff = height - barHeight;

                if (i >= (u32)heightDiff) {
                    printPadding(' ', xSlotSize - 1);
                    if (barHeight < 0.5)
                        printf("_");
                    else if (i == (u32)heightDiff) {
                        printf("\334");
                    } else {
                        printf("\333");
                    }
                }
            }
        }

        printf("\n");

        i++;
    }

    i = 0;

    // Diubujar el eje x
    while (i < (xSlotSize * xSlots) + xSlotSize) {
        if (i == 0) {
            if (min == 0) {
                printPadding(' ', spaces - 1);
                printf("0");
            } else {
                printPadding(' ', spaces);
            }

            printf(" \300");
        } else {
            if (i % xSlotSize == 0) {
                printf("\302");
            } else {
                printf("\304");
            }
        }

        i++;
    }

    printf("  Provincias\n");

    u32 index = 0;
    u32 cursor = 1;
    printPadding(' ', spaces + 1);

    // Palabras de 3 letras
    while (cursor <= xSlotSize * xSlots) {
        if (cursor % xSlotSize == 0) {
            printf("%s", shortenedProvinceName(data[index].province));

            index++;
            cursor += 3;
        } else {
            printf(" ");
            cursor++;
        }
    }

    printf("\n\n");
}

void handleProvinceBestResults(Mortality *mortalityData) {
    MortalityCausesMenu cause = causesMenu();

    if (cause != ExitMortalityCauses) {
        Mortality data = regionWithLeastDeathsFrom(mortalityData, cause);
        showBestProvince(data.province, cause, deathsFrom(data, &cause, 1));

        awaitKey();
        clearScreen();
        handleProvinceBestResults(mortalityData);
    }
}

void handleNationalDeaths(Mortality *mortalityData) {
    MortalityCausesMenu cause = causesMenu();

    if (cause != ExitMortalityCauses) {
        u32 deaths = nationalDeathsFrom(mortalityData, &cause, 1);
        showDeathsFrom(deaths, cause);

        awaitKey();
        clearScreen();
        handleNationalDeaths(mortalityData);
    }
}

void handleMortalityRate(Mortality *data) {
    double rate = mortalityRate(data);
    showMortalityRate(rate);

    awaitKey();
}

void handleSortedTable(Mortality *data) {
    Mortality cpy[PROVINCES];
    // Hacer una copia del array para no afectar el array
    // original
    u32 i;
    for (i = 0; i < PROVINCES; i++) {
        cpy[i] = data[i];
    }

    heading("Provincias con mayor cantidad de fallecidos al mes de vida");

    sortByDeathInOneMonth(cpy);
    printTable(cpy);
    printChart(cpy);

    awaitKey();
}

// Ejecuta la funcion pertinente segun cada accion
void handleChoice(MainScreenMenu action, Mortality *state) {
    switch (action) {
    case CountrywideMortalityRate:
        handleMortalityRate(state);
        break;
    case ProvinceWithLeastDeathsFrom:
        handleProvinceBestResults(state);
        break;
    case CountrywideDeathsFrom:
        handleNationalDeaths(state);
        break;
    case ProvincesDataList:
        printTable(state);
        awaitKey();
        break;
    case SortProvincesWithMostDeathsInOneMonth:
        handleSortedTable(state);
        break;
    default:
        printf("Acci\242n no v\240lida\n");
        awaitKey();
        break;
    }
}

void displayMainScreenMenu(Mortality *state) {
    Option options[MAIN_SCREEN_OPTIONS];
    setMainOptions(options);

    bool exit = false;

    do {
        heading("Control de mortalidad infantil en Cuba");

        // Verificacion real de un cambio
        bool dataChanged = verifyChanges(state);

        MainScreenMenu action = menu(options, MAIN_SCREEN_OPTIONS);
        exit = action == ExitMainScreen;

        if (!exit) {
            clearScreen();

            // No permitir ninguna consulta de datos
            // hasta que al menos uno se haya introducido
            if (action == InsertMortalityData)
                handleInsertion(state);
            else if (dataChanged) handleChoice(action, state);
            else {
                warning("Se necesita que introduzcas al menos un dato de mortalidad\n");
                awaitKey();
            }

            clearScreen();
        }
    } while (!exit);
}
