#include <logica/types.h>
#include <stdbool.h>

bool isNumber(const char[]);
bool parseFloat(const char[], float*);
bool has(const char[], char, u32);
void trim(char[]);
bool isInt(const char[]);
bool isPositive(const char[]);
bool notNegSentinel(const char[]);
bool all(const char[], fn_ptr*, u32);
bool verifyChanges(Mortality*);

char *causeName(MortalityCausesMenu);
char *provinceName(Province);
char *shortenedProvinceName(Province);

void setDeathsData(Mortality*, MortalityCausesMenu, u32);
void setMainOptions(Option*);
void setCauseOptions(Option*);
void setBinaryOptions(Option*);
void initState(Mortality*);
void mockCountrywideData(Mortality*);
void sortByDeathInOneMonth(Mortality*);
void setDefaultValues(Mortality*);

u32 findMortalityByProvince(Mortality*, Province);
u32 nationalBirths(Mortality*);
u32 provinceTotalDeaths(Mortality);
u32 deathsFrom(Mortality, MortalityCausesMenu*, u32);
u32 nationalDeathsFrom(Mortality*, MortalityCausesMenu*, u32);
u32 nationalDeaths(Mortality*);
u32 countDigits(int);

Mortality regionWithLeastDeathsFrom(Mortality*, MortalityCausesMenu);

double mortalityRate(Mortality*);
