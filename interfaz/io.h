#include <logica/types.h>

void input(char[], u32, fn_ptr*, u32);

float inputFloat(fn_ptr*, u32);
int inputIntNotNegSentinel();

u32 menu(Option*, u32);
Province provincesMenu();
MortalityCausesMenu causesMenu();
BinaryMenu confirm();
u32 insertionMenu();

bool inputBirths(Mortality*);
void inputDeaths(Mortality*, MortalityCausesMenu);
void retrieveData(Mortality*);

void displayMainScreenMenu(Mortality*);
void displayInsertionHeading(Mortality);

void handleNationalDeaths(Mortality*);
void handleProvinceBestResults(Mortality*);
void handleMortalityRate(Mortality*);
void handleChoice(MainScreenMenu, Mortality*);
void handleInsertion(Mortality*);
void handleSortedTable(Mortality*);

void heading(const char*);
void success(const char*);
void error(const char*);
void warning(const char*);
void info(const char*);
void awaitKey();
void clearScreen();

void showData(Mortality, MortalityCausesMenu);
void showMortalityRate(double);
void showMortalityData(Mortality);
void showDeathsFrom(u32, MortalityCausesMenu);
void showBestProvince(Province, MortalityCausesMenu, u32);

void printPadding(char, u32);
void printTable(const Mortality*);
void printChart(Mortality*);
