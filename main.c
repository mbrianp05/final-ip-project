#include <logica/logica.h>
#include <interfaz/io.h>

int main() {
    // Para datos de prueba y no tener que
    // introducirlos manualmente usa: mockCountrywideData(state)
    Mortality state[PROVINCES];
    initState(state);

    displayMainScreenMenu(state);

    return 0;
}
