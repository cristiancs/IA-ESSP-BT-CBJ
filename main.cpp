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
    // cout << "SECTION_HORIZON " << h << endl;
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

    string siguientePaso = "SECTION_DAYS_OFF";

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
    string siguientePaso = "SECTION_SHIFT_OFF_REQUESTS";

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
            // 	cout << "      " << d.first << endl;
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
    resultado[currentDia] = dataTurnos;

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
    vector<pair<int, vector<Empleado>>> asignacionesFinales;
    int cantidadTurnos;
    int totalAvailableEmployees;
    int totalToAsign;
    int maximumAmountOfEmployeesToAssign;
    map<string, vector<Empleado>> snapshots;

public:
    string
    getTipoTurno(int i)
    {
        if (i < 0) {
            return "NO_SHIFT";
        }
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
        totalAvailableEmployees = staffs.size();

        for (const auto& staff : staffs) {
            Empleado emp = Empleado();
            emp.id = staff.first;
            map<string, int> maxT;
            for (const auto& turno : staff.second.first) {
                maxT[turno.first] = turno.second;
            }

            emp.MaxT = maxT;
            emp.MaxM = staff.second.second[0];
            emp.minM = staff.second.second[1];
            emp.MaxCT = staff.second.second[2];
            emp.minCT = staff.second.second[3];
            emp.MinCDL = staff.second.second[4];
            emp.MaxFD = staff.second.second[5];
            emp.daysOff = dayOff[staff.first];
            emp.shiftOnRequests = shiftOnRequests[staff.first];
            emp.shiftOffRequests = shiftOffRequests[staff.first];
            emp.penalizaciones = 0;
            emp.currentM = 0;
            emp.currentCT = 0;
            emp.currentFD = 0;
            emp.lastWorkedShift = -10;
            empleados.push_back(emp);
        }

        maximumAmountOfEmployeesToAssign = 0;
        for (const auto& dia : cover) {
            for (const auto& tipoTurno : dia.second) {
                totalToAsign += tipoTurno.second[0];
                if (maximumAmountOfEmployeesToAssign < tipoTurno.second[0]) {
                    maximumAmountOfEmployeesToAssign = tipoTurno.second[0];
                }
            }
        }
    }
    bool isWeekend(int dia)
    {
        return dia != 0 && (dia % 7 == 5 || dia % 7 == 6);
    }
    bool checkIfWorks(Empleado emp, int turno)
    {
        string tipoTurno = getTipoTurno(turno);
        int dia = getDiaTurno(turno);
        int duracionTurno = shifts[tipoTurno].first;
        bool sirve = true;

        // Minutos máximo de trabajo
        if (emp.currentM + duracionTurno > emp.MaxM) {
            cout << "  "
                 << "[x] Minutos máximo de trabajo (" << emp.currentM << "+" << duracionTurno << "/"
                 << emp.MaxM << ")" << endl;
            sirve = false;
        }
        // Cantidad de turnos de tipo T que puede hacer
        if (emp.currentT[tipoTurno] + 1 > emp.MaxT[tipoTurno]) {
            cout << "  "
                 << "[x] Cantidad de turnos de tipo T que puede hacer (" << emp.currentT[tipoTurno] << "/" << emp.MaxT[tipoTurno] << ")" << endl;
            sirve = false;
        }
        // Max turnos consecutivos
        if (emp.MaxCT < emp.currentCT + 1) {
            cout << "  "
                 << "[x] Max turnos consecutivos" << endl;
            sirve = false;
        }
        // Fines de Semana (recordar que lunes = 0)
        if (isWeekend(dia)) {
            if (!isWeekend(getDiaTurno(emp.lastWorkedShift))) {
                if (emp.currentFD + 1 > emp.MaxFD) {
                    cout << "  "
                         << "[x] Fines de Semana (" << emp.currentFD << "/" << emp.MaxFD << ")" << endl;
                    sirve = false;
                }
            }
        }

        // Dias libres obligatorios
        if (find(emp.daysOff.begin(), emp.daysOff.end(), dia) != emp.daysOff.end()) {
            cout << "  "
                 << "[x] Dias libres obligatorios" << endl;
            sirve = false;
        }

        // Continuación de turnos
        if (emp.lastWorkedShift != -10) {
            vector<string> latestShiftRestrictions = shifts[getTipoTurno(emp.lastWorkedShift)].second;
            if (find(latestShiftRestrictions.begin(), latestShiftRestrictions.end(), getTipoTurno(emp.lastWorkedShift)) != latestShiftRestrictions.end()) {
                cout << "  "
                     << "[x] Continuación de turnos" << endl;
                sirve = false;
            }
        }
        if (sirve) {
            cout << " Turno " << turno << " sirve" << endl;
        }
        return sirve;
    }
    void resetToTurno(Empleado emp, int empleadoIndex, int turno)
    {
        emp.currentM = 0;
        emp.currentT.clear();
        emp.lastWorkedShift = -10;
        emp.currentFD = 0;
        emp.currentCT = 0;
        for (int i = 0; i < turno; i++) {
            if (emp.assignedShifts[i] == 1) {
                string tipoTurno = getTipoTurno(i);
                int dia = getDiaTurno(i);
                int duracionTurno = shifts[tipoTurno].first;
                int neededEmployees = cover[dia][tipoTurno][0];

                emp.currentM += duracionTurno;
                emp.currentT[tipoTurno] += 1;
                emp.currentCT += 1;
                emp.lastWorkedShift = i;
                if (isWeekend(dia) && !isWeekend(emp.lastWorkedShift)) {
                    emp.currentFD += 1;
                }
            } else {
                emp.currentCT = 0;
            }
        }

        emp.assignedShifts.resize(turno);
        empleados[empleadoIndex] = emp;
    }
    int checkFinalShifts(Empleado emp)
    {
        if (emp.currentM < emp.minM) {
            cout << "  "
                 << "[x] Minimo de minutos trabajados (" << emp.currentM << "/" << emp.minM << ")" << endl;
            return 1;
        }
        int maxCT = 0;
        int maxCDL = 0;
        int currentCDL = 0;
        int currentCT = 0;
        int i = 0;
        for (int val : emp.assignedShifts) {
            if (val == 1) {
                currentCT += 1;
                if (currentCDL > maxCDL) {
                    maxCDL = currentCDL;
                    currentCDL = 0;
                }
            } else {
                if (currentCT > maxCT) {
                    maxCT = currentCT;
                }
                currentCT = 0;
                if (find(emp.daysOff.begin(), emp.daysOff.end(), i) == emp.daysOff.end()) {
                    currentCDL += 1;
                }
            }
            i += 1;
        }
        if (maxCDL < emp.MinCDL) {
            cout << "  "
                 << "[x] Minimo dias libres consecutivos" << endl;
            return 2;
        }
        if (maxCT < emp.minCT) {
            cout << "  "
                 << "[x] Minimo de turnos consecutivos (" << maxCT << "/" << emp.minCT << ")" << endl;
            return 3;
        }
        return -1;
    }
    void printCamino(Empleado emp)
    {
        int i = 0;
        int cantidadTurnos = emp.assignedShifts.size();
        int spaces = cantidadTurnos;

        int cantidadUnos = 0;
        for (int val : emp.assignedShifts) {
            cout << i << ":";
            if (val == 1) {
                cout << string(spaces - to_string(i).length() - 1, ' ') << 1 << endl;
                spaces -= 1;
                cantidadUnos += 1;
            } else {
                cout << string(spaces + 1 - to_string(i).length() - 1, ' ') << " 0" << endl;
                spaces += 1;
            }
            i += 1;
        }
        cout << "Cantidad de 1's: " << cantidadUnos << endl;
    }
    int lastWorkingShift(Empleado emp)
    {
        auto result = find_if(emp.assignedShifts.rbegin(), emp.assignedShifts.rend(),
            [](int i) { return i == 1; });
        int lastOne = distance(result, emp.assignedShifts.rend()) - 1;
        return lastOne;
    }
    void run(int empleado = 0, int initialTurn = 0)
    {
        cout << "Starting" << endl;
        Empleado emp = empleados[empleado];
        cout << "Testing employee: " << emp.id << endl;

        resetToTurno(emp, empleado, initialTurn);
        emp = empleados[empleado];

        for (int turno = initialTurn; turno < cantidadTurnos * h; turno++) {

            string tipoTurno = getTipoTurno(turno);
            int dia = getDiaTurno(turno);
            int duracionTurno = shifts[tipoTurno].first;
            int neededEmployees = cover[dia][tipoTurno][0];
            cout << " Testing turno: " << turno << endl;
            bool isValidTurn = checkIfWorks(emp, turno);
            if (isValidTurn && ((initialTurn != 0 && initialTurn != turno) || initialTurn == 0)) { // Es valido y no es el turno a cambiar
                cout << "  Turno works, assigning employee to turno" << endl;
                emp.currentM += duracionTurno;
                emp.currentT[tipoTurno] += 1;

                if (isWeekend(dia) && !isWeekend(emp.lastWorkedShift)) {
                    emp.currentFD += 1;
                }
                emp.lastWorkedShift = turno;
                emp.currentCT += 1;
                emp.assignedShifts.push_back(1);
            } else {
                emp.currentCT = 0;
                emp.assignedShifts.push_back(0);
            }
        }
        empleados[empleado] = emp;
        cout << " Revisando si cumple  con los requisitos minimos" << endl;
        printCamino(emp);
        int resultado = checkFinalShifts(emp); // -1,1,2,3 = Valido,  Minimo de minutos trabajados, Minimo dias libres consecutivos, Minimo de turnos consecutivos
        if (resultado == -1) {
            cout << "Turnos del empleado valido" << endl;
            if (empleado == totalAvailableEmployees) {
                cout << "Encontrada solución valida, hora de escribir";
            } else {
                cout << "Continuando con otro empleado" << endl;
                //run(empleado + 1);
            }
        } else {
            cout << "Esta asignación de turnos no es valida, probando alternativas" << endl;
            // Buscar los conflictos se reduce a encontrar el último 1 y probarlo con 0
            int lastOne = lastWorkingShift(emp);
            cout << "Returning to " << lastOne << endl;
            run(empleado, lastOne);
        }
    }
};

int main(int argc, char const* argv[])
{

    Main programa;
    cout << "Parsing Input" << endl;
    programa.parseInput();
    cout << "Building Structures" << endl;
    programa.buildStructures();
    cout << "Starting Program" << endl;
    programa.run();
    return 0;
}