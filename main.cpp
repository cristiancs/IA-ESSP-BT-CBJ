#include <bits/stdc++.h>

using namespace std;

// Split tipo python para leeer el input
vector<string> customSplit(string& linea, char delimitador)
{
    linea.erase(remove_if(linea.begin(), linea.end(), ::isspace), linea.end());

    string palabra;
    vector<string> palabras;
    istringstream palabraStream(linea);

    while (getline(palabraStream, palabra, delimitador)) {
        palabras.push_back(palabra);
    }

    return palabras;
}

// Leer cantidad de dias a planificar
int readHorizon()
{
    string siguientePaso = "SECTION_HORIZON";
    string linea;
    int h;

    while (getline(cin, linea)) {
        if (linea.compare(siguientePaso) == 0) {
            cin >> h;
            break;
        }
    }
    //	cout << "SECTION_HORIZON " << h << endl;
    return h;
}

// Retorna un map donde la llave es el Turno y los valores son pares de la forma (Duracion, [Turnos que no pueden seguir])
map<string, pair<int, vector<string>>> readShift()
{

    string linea;
    vector<string> data;
    map<string, pair<int, vector<string>>> resultado;

    string anteriorPaso = "SECTION_SHIFTS";

    // Encontramos la línea SECTION_SHIFTS
    while (getline(cin, linea)) {
        if (linea.compare(anteriorPaso) == 0) {
            break;
        }
    }

    string siguientePaso = "SECTION_STAFF";

    // Leémos hasta llegar a la siguiente sección
    while (getline(cin, linea) && linea.compare(siguientePaso) != 0) {

        data = customSplit(linea, ',');
        if (data.size() > 2) {
            resultado[data[0]] = make_pair(stoi(data[1]), customSplit(data[2], '|'));
            //		cout << data[0] << " " << stoi(data[1]) << " " << data[2] << endl;
        } else {
            vector<string> empty;
            resultado[data[0]] = make_pair(stoi(data[1]), empty);
            //		cout << data[0] << " " << stoi(data[1]) << " " << endl;
        }
    }
    return resultado;
}

// Retorna un map donde la llave es el empleado y los valores son pares de la forma  ( [ (Turno, Restricción)... ], [MaxT, MaxM, MinM, MaxCT, MinCT, MinCDL, MaxFD ] )
map<string, pair<vector<pair<string, int>>, vector<int>>> readStaff()
{

    string linea;
    map<string, pair<vector<pair<string, int>>, vector<int>>> resultado;

    vector<string> data;
    vector<string> rawTurnos;
    vector<string> turno;

    string siguientePaso = "SECTION_DAY_OFF";

    // Leémos hasta llegar a la siguiente acción
    while (getline(cin, linea) && linea.compare(siguientePaso) != 0) {
        data = customSplit(linea, ',');
        rawTurnos = customSplit(data[1], '|');

        vector<pair<string, int>> turnos;
        vector<int> otros;

        // Insertamos en un vector los turnos
        for (int i = 0; i < rawTurnos.size(); ++i) {
            turno = customSplit(rawTurnos[i], '=');
            turnos.push_back(make_pair(turno[0], stoi(turno[1])));
        }

        // Insertamos en un vector los 6 valores restantes
        for (int i = 2; i < 8; ++i) {
            otros.push_back(stoi(data[i]));
        }
        resultado[data[0]] = make_pair(turnos, otros);
    }
    return resultado;
}

// Retorna un mapa donde las llaves son los empleados y los valores son vectores de números
map<string, vector<int>> readDayOff()
{
    map<string, vector<int>> resultado;

    string siguientePaso = "SECTION_SHIFT_ON_REQUESTS";

    string linea;
    vector<string> data;
    vector<string> rawDias;

    while (getline(cin, linea) && linea.compare(siguientePaso) != 0) {
        data = customSplit(linea, ',');
        vector<int> dias;
        if (data.size() > 1) {
            rawDias = customSplit(data[1], '|');

            for (int i = 0; i < rawDias.size(); ++i) {
                dias.push_back(stoi(rawDias[i]));
            }
        }

        resultado[data[0]] = dias;
    }
    return resultado;
}

// Retorna un mapa cuyas llaves son los empleados y los valores son vectores de pares de la forma [ [TurnoID, [Día, Peso] ] ] (tuve que desordenar esto para almacenarlo fácilmente)
map<string, vector<pair<string, pair<int, int>>>> readShiftOnRequests()
{
    string linea;
    map<string, vector<pair<string, pair<int, int>>>> resultado;
    vector<string> data;
    string siguientePaso = "SECTION_SHIFT_OFF_REQUEST";

    while (getline(cin, linea) && linea.compare(siguientePaso) != 0) {
        data = customSplit(linea, ',');
        resultado[data[0]].push_back(make_pair(data[2], make_pair(stoi(data[1]), stoi(data[3]))));
    }

    return resultado;
}

// Retorna un mapa cuyas llaves son los empleados y los valores son vectores de pares de la forma [ [TurnoID, [Día, Peso] ] ] (tuve que desordenar esto para almacenarlo fácilmente)
map<string, vector<pair<string, pair<int, int>>>> readShiftOffRequests()
{
    string linea;
    map<string, vector<pair<string, pair<int, int>>>> resultado;
    vector<string> data;
    string siguientePaso = "SECTION_COVER";

    while (getline(cin, linea) && linea.compare(siguientePaso) != 0) {
        data = customSplit(linea, ',');
        resultado[data[0]].push_back(make_pair(data[2], make_pair(stoi(data[1]), stoi(data[3]))));
    }

    return resultado;
}

//   {dia: { turno: [ Empleados, CostoPorNoAsignar, CostoPorAsignar]} } , ...

map<int, map<string, vector<int>>> readCover()
{
    map<int, map<string, vector<int>>> resultado;
    string linea;
    vector<string> data;

    int currentDia = 0;
    int numero;
    map<string, vector<int>> dataTurnos;
    while (getline(cin, linea)) {
        data = customSplit(linea, ',');
        string turno = data[1];
        int dia = stoi(data[0]);

        if (dia != currentDia) {
            // Esto es un debugger
            // cout << currentDia << endl;
            // for (const auto &d : dataTurnos)
            // {
            // 	cout << " " << d.first << endl;
            // 	for (const auto &s : dataTurnos[d.first])
            // 	{

            // 		cout << "  " << s << endl;
            // 	}
            // }
            // cout << endl;

            resultado[currentDia] = dataTurnos;
            dataTurnos.clear();
            currentDia = dia;
        }
        vector<int> otros;
        for (int i = 2; i < 5; ++i) {

            otros.push_back(stoi(data[i]));
        }
        dataTurnos[turno] = otros;
    }

    return resultado;
}

typedef struct
{
    // Valores leidos desde la DB
    map<string, int> MaxT;
    int MaxM;
    int minM;
    int MaxCT;
    int minCT;
    int MinCDL;
    int MaxFD;
    string id;
    vector<int> daysOff;
    vector<pair<string, pair<int, int>>> shiftOnRequests;
    vector<pair<string, pair<int, int>>> shiftOffRequests;

    // Valores con la asignación actual
    map<string, int> currentT;
    int currentM;
    int currentCT;
    int currentFD;
    vector<int> assignedShifts;
    int penalizaciones;
    int lastWorkedShift;
    bool hasCompletedCDL;

} Empleado;

class Main {
    int h;
    map<string, pair<int, vector<string>>> shifts;
    map<string, pair<vector<pair<string, int>>, vector<int>>> staffs;
    map<string, vector<int>> dayOff;
    map<string, vector<pair<string, pair<int, int>>>> shiftOnRequests;
    map<string, vector<pair<string, pair<int, int>>>> shiftOffRequests;
    map<int, map<string, vector<int>>> cover;
    vector<Empleado> empleados;
    vector<string> tiposTurno;
    int cantidadTurnos;
    int cantidadStaff;

public:
    string
    getTipoTurno(int i)
    {
        return tiposTurno[i % tiposTurno.size()];
    }
    int getDiaTurno(int i)
    {
        return i / cantidadTurnos;
    }
    void parseInput()
    {
        h = readHorizon();
        shifts = readShift();
        staffs = readStaff();
        dayOff = readDayOff();
        shiftOnRequests = readShiftOnRequests();
        shiftOffRequests = readShiftOffRequests();
        cover = readCover();
    }
    void buildStructures()
    {
        // Vector con los tipos de turno

        for (pair<const string, pair<int, vector<string>>>& x : shifts) {
            tiposTurno.push_back(x.first);
        }
        // Creamos una lista con cada turno vacia
        cantidadTurnos = shifts.size();
        vector<vector<Empleado>> asignaciones[cantidadTurnos * h];

        // Carguemos los empleados en un struct
        cantidadStaff = staffs.size();

        for (const auto& staff : staffs) {
            Empleado emp = Empleado();
            emp.id = staff.first;
            map<string, int> maxT;

            emp.MaxT = maxT;
            emp.MaxM = staff.second.second[1];
            emp.minM = staff.second.second[2];
            emp.MaxCT = staff.second.second[3];
            emp.minCT = staff.second.second[4];
            emp.MinCDL = staff.second.second[5];
            emp.MaxFD = staff.second.second[6];
            emp.daysOff = dayOff[staff.first];
            emp.shiftOnRequests = shiftOnRequests[staff.first];
            emp.shiftOffRequests = shiftOffRequests[staff.first];
            emp.penalizaciones = 0;
            emp.currentM = 0;
            emp.currentCT = 0;
            emp.currentFD = 0;
            emp.lastWorkedShift = -10;
            emp.hasCompletedCDL = false;
            empleados.push_back(emp);
        }
    }
    void run()
    {

        // Empezamos a iterar
        // Iteramos sobre cada turno
        for (int turno = 0; turno < cantidadTurnos * h; turno++) {
            // Partimos asignado empleados
            int assignedEmployees = 0;
            int currentEmployee = 0;
            while (assignedEmployees < empleados.size()) {
                Empleado emp = empleados[currentEmployee];

                bool sirve = true;
                // Verificamos si el empleado sirve para este turno
                string tipoTurno = getTipoTurno(turno);
                int duractionTurno = shifts[tipoTurno].first;
                int dia = getDiaTurno(turno);
                // Minutos máximo de trabajo
                if (emp.currentM + duractionTurno > emp.MaxM) {
                    sirve = false;
                }
                // Cantidad de turnos de tipo T que puede hacer
                if (emp.currentT[tipoTurno] + 1 > emp.MaxT[tipoTurno]) {
                    sirve = false;
                }
                // Max turnos consecutivos
                if (emp.lastWorkedShift + 1 == turno) {
                    if (emp.MaxCT > emp.currentCT) {
                        sirve = false;
                    }
                }
                // Fines de Semana (recordar que lunes = 0)
                if (dia % 5 == 0 || dia % 6 == 0) {
                    if (emp.currentFD + 1 > emp.MaxFD) {
                        sirve = false;
                    }
                }

                // Dias libres obligatorios
                if (find(emp.daysOff.begin(), emp.daysOff.end(), dia) != emp.daysOff.end()) {
                    sirve = false;
                }
            }
        }
    }
};

int main(int argc, char const* argv[])
{

    Main programa;

    programa.parseInput();
    programa.buildStructures();
    programa.run();
    return 0;
}