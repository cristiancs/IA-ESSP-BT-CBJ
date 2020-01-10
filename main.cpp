#include <bits/stdc++.h>
#include <thread>
using namespace std;

// Split tipo python para leer el input
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

// {empleado: {dia: {tipo_turno: penalizacion}}}
map<string, map<int, map<string, int>>> readShiftOnRequests()
{
    string linea;
    map<string, map<int, map<string, int>>> resultado;
    vector<string> data;
    string siguientePaso = "SECTION_SHIFT_OFF_REQUESTS";
    string lastEmployee = "";
    map<int, map<string, int>> toSave;
    while (getline(cin, linea) && linea.compare(siguientePaso) != 0) {
        data = customSplit(linea, ',');
        if (lastEmployee.compare("") != 0 && lastEmployee.compare(data[0]) != 0) {
            resultado[lastEmployee]
                = toSave;
            toSave.empty();
        }
        if (toSave.find(stoi(data[1])) == toSave.end()) {
            map<string, int> temp;
            toSave[stoi(data[1])] = temp;
        }
        toSave[stoi(data[1])][data[2]] = stoi(data[3]);
        lastEmployee = data[0];
    }
    // for (const auto& d : resultado) {
    //     cout << " " << d.first << endl;
    //     for (const auto& e : resultado[d.first])
    //         cout << "  " << e.first << endl;
    // }
    return resultado;
}

// Retorna un mapa cuyas llaves son los empleados y los valores son vectores de pares de la forma [ [TurnoID, [Día, Peso] ] ] (tuve que desordenar esto para almacenarlo fácilmente)
map<string, map<int, map<string, int>>> readShiftOffRequests()
{
    string linea;
    map<string, map<int, map<string, int>>> resultado;
    vector<string> data;
    string siguientePaso = "SECTION_COVER";
    string lastEmployee = "";
    map<int, map<string, int>> toSave;
    while (getline(cin, linea) && linea.compare(siguientePaso) != 0) {
        data = customSplit(linea, ',');
        //A , 0 , E , 3
        if (toSave.find(stoi(data[1])) == toSave.end()) {
            map<string, int> temp;
            toSave[stoi(data[1])] = temp;
        }
        if (lastEmployee.compare("") != 0 && lastEmployee.compare(data[0]) != 0) {
            resultado[lastEmployee]
                = toSave;
            lastEmployee = data[0];
            toSave.empty();
        }
        toSave[stoi(data[1])][data[2]] = stoi(data[3]);
    }

    return resultado;
}

//   {dia: { turno: [ Empleados, costoPorEmpleadoFaltante, costoPorEmpleadoSobrante]} } , ...

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
    map<int, map<string, int>> shiftOnRequests;
    map<int, map<string, int>> shiftOffRequests;

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
    map<string, map<int, map<string, int>>> shiftOnRequests;
    map<string, map<int, map<string, int>>> shiftOffRequests;
    map<int, map<string, vector<int>>> cover;
    vector<Empleado> empleados;
    vector<string> tiposTurno;
    vector<pair<int, vector<Empleado>>> asignacionesFinales;
    int cantidadTurnos;
    int totalAvailableEmployees;
    int lastEmployee;
    int currentBestSolutionCost;
    int searchWindow;
    int debug;
    string rawEmployees;
    vector<int> currentBestSolutionPath;
    map<string, vector<Empleado>> snapshots;
    time_t startTime;

public:
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
            rawEmployees += linea + "\n";
            data
                = customSplit(linea, ',');
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
        lastEmployee = -1;
        currentBestSolutionCost = -1;
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
        startTime = time(NULL);
    }
    bool isWeekend(int dia)
    {
        return dia != 0 && (dia % 7 == 5 || dia % 7 == 6);
    }
    bool notSurpassedMaxWorkTime(Empleado emp, int turno)
    {
        string tipoTurno = getTipoTurno(turno);
        int dia = getDiaTurno(turno);
        bool sirve = true;
        int duracionTurno = shifts[tipoTurno].first;
        // Minutos máximo de trabajo
        if (emp.currentM + duracionTurno > emp.MaxM) {
            cout << "  "
                 << "[✖] Minutos máximo de trabajo (" << emp.currentM << "+" << duracionTurno << "/"
                 << emp.MaxM << ")" << endl;
            sirve = false;
        }
        return sirve;
    }
    bool checkIfWorks(Empleado emp, int turno)
    {
        string tipoTurno = getTipoTurno(turno);
        int dia = getDiaTurno(turno);
        int duracionTurno = shifts[tipoTurno].first;
        bool sirve = true;

        notSurpassedMaxWorkTime(emp, turno);

        // Cantidad de turnos de tipo T que puede hacer
        if (emp.currentT[tipoTurno] + 1 > emp.MaxT[tipoTurno]) {
            cout << "  "
                 << "[✖] Cantidad de turnos de tipo T que puede hacer (" << emp.currentT[tipoTurno] << "/" << emp.MaxT[tipoTurno] << ")" << endl;
            sirve = false;
        }
        // Max turnos consecutivos
        if (emp.MaxCT < emp.currentCT + 1) {
            cout << "  "
                 << "[✖] Max turnos consecutivos" << endl;
            sirve = false;
        }
        // Fines de Semana (recordar que lunes = 0)
        if (isWeekend(dia)) {
            if (!isWeekend(getDiaTurno(emp.lastWorkedShift))) {
                if (emp.currentFD + 1 > emp.MaxFD) {
                    cout << "  "
                         << "[✖] Fines de Semana (" << emp.currentFD << "/" << emp.MaxFD << ")" << endl;
                    sirve = false;
                }
            }
        }

        // Dias libres obligatorios
        if (find(emp.daysOff.begin(), emp.daysOff.end(), dia) != emp.daysOff.end()) {
            cout << "  "
                 << "[✖] Dias libres obligatorios" << endl;
            sirve = false;
        }

        // Continuación de turnos
        if (emp.lastWorkedShift != -10) {
            vector<string> latestShiftRestrictions = shifts[getTipoTurno(emp.lastWorkedShift)].second;
            if (find(latestShiftRestrictions.begin(), latestShiftRestrictions.end(), getTipoTurno(emp.lastWorkedShift)) != latestShiftRestrictions.end()) {
                cout << "  "
                     << "[✖] Continuación de turnos" << endl;
                sirve = false;
            }
        }
        if (sirve) {
            cout << "  "
                 << "[✔] " << turno << " sirve " << endl;
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
    int checkFinalShifts(Empleado emp, int turno)
    {
        if (emp.currentM < emp.minM) {
            cout << "  "
                 << "[✖] Minimo de minutos trabajados (" << emp.currentM << "/" << emp.minM << ")" << endl;
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
                if (currentCT > maxCT) {
                    maxCT = currentCT;
                }
                currentCDL = 0;
            } else {
                currentCT = 0;
                currentCDL += 1;
                if (currentCDL > maxCDL) {
                    maxCDL = currentCDL;
                }
                // No es dia libre obligatorio
                //if (find(emp.daysOff.begin(), emp.daysOff.end(), i) == emp.daysOff.end()) {
                //
                //
                //}
            }
            i += 1;
        }
        if (maxCDL < emp.MinCDL) {
            cout << "  "
                 << "[✖] Minimo dias libres consecutivos" << endl;
            return 2;
        }
        if (maxCT < emp.minCT) {
            cout << "  "
                 << "[✖] Minimo de turnos consecutivos (" << maxCT << "/" << emp.minCT << ")" << endl;
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
    void writeCaminoVector(vector<int> array, ofstream& outfile)
    {
        int i = 0;
        int cantidadTurnos = array.size();
        int spaces = cantidadTurnos >= 3 ? cantidadTurnos : 3;

        int cantidadUnos = 0;
        for (int val : array) {
            outfile << i << ":";
            if (val == 1) {
                outfile << string(spaces - to_string(i).length() - 1, ' ') << 1 << endl;
                spaces -= 1;
                cantidadUnos += 1;
            } else {
                outfile << string(spaces + 1 - to_string(i).length() - 1, ' ') << " 0" << endl;
                spaces += 1;
            }
            i += 1;
        }
        outfile << "Cantidad de 1's: " << cantidadUnos << endl;
    }
    void writeCamino(Empleado emp, ofstream& outfile)
    {
        writeCaminoVector(emp.assignedShifts, outfile);
    }
    int lastWorkingShift(Empleado emp)
    {
        auto result = find_if(emp.assignedShifts.rbegin(), emp.assignedShifts.rend(),
            [](int i) { return i == 1; });
        int lastOne = distance(result, emp.assignedShifts.rend()) - 1;
        return lastOne;
    }

    int calcularCostoEmpleado(Empleado emp)
    {
        int shift = 0;
        int penalizacionEmpleado = 0;
        for (int val : emp.assignedShifts) {
            int dia = getDiaTurno(shift);
            if (val == 1) {
                //cout << "[shiftoffrequest] Looking for request on " << dia << endl;
                if (emp.shiftOffRequests.find(dia) != emp.shiftOffRequests.end()) {
                    //cout << "[shiftoffrequest] Found restrictions for this day" << endl;
                    if (emp.shiftOffRequests[dia].find(getTipoTurno(shift)) != emp.shiftOffRequests[dia].end()) {
                        // cout << "[shiftoffrequest] Found restrictions for this shift" << endl;
                        penalizacionEmpleado += emp.shiftOffRequests[dia][getTipoTurno(shift)];
                    }
                }
            } else {
                //cout << "[shiftOnRequests] Looking for request on " << dia << endl;
                if (emp.shiftOnRequests.find(dia) != emp.shiftOnRequests.end()) {
                    //cout << "[shiftOnRequests] Found restrictions for this day" << endl;
                    if (emp.shiftOnRequests[dia].find(getTipoTurno(shift)) != emp.shiftOnRequests[dia].end()) {
                        //cout << "[shiftOnRequests] Found restrictions for this shift" << endl;
                        penalizacionEmpleado += emp.shiftOnRequests[dia][getTipoTurno(shift)];
                    }
                }
            }
            shift += 1;
        }
        return penalizacionEmpleado;
    }
    int calcularCostoEmpresa(vector<int> array)
    {
        vector<int> sumaEmpleados(cantidadTurnos * h);
        int shift = 0;
        // Recorre todos los empleados
        for (int val : array) {
            // Ya se recorrieron todos los turnos que existe (ergo, cambio de empleado)
            if (shift == cantidadTurnos * h) {
                shift = 0;
            }
            sumaEmpleados[shift] += val;
            shift += 1;
        }
        int sumaCostos = 0;
        shift = 0;
        for (int empleadosEnTurno : sumaEmpleados) {
            int dia = getDiaTurno(shift);
            string tipoTurno = getTipoTurno(shift);
            vector<int> dataDia = cover[dia][tipoTurno];
            // cout << dia << " | " << tipoTurno << " | " << dataDia.size() << endl;
            // {dia: { turno: [ Empleados, costoPorEmpleadoFaltante, costoPorEmpleadoSobrante]} } , ...
            int requiredEmployees = dataDia[0];
            int costoPorEmpleadoFaltante = dataDia[1];
            int costoPorEmpleadoSobrante = dataDia[2];

            // cout << "[DEBUG]" << requiredEmployees << "|" << empleadosEnTurno << endl;
            if (requiredEmployees > empleadosEnTurno) {
                sumaCostos += (requiredEmployees - empleadosEnTurno) * costoPorEmpleadoFaltante;
            } else {
                sumaCostos += (empleadosEnTurno - requiredEmployees) * costoPorEmpleadoSobrante;
            }

            shift += 1;
        }
        return sumaCostos;
    }
    void setSearchWindow(int time)
    {
        searchWindow = time;
    }
    bool hasTimeWindowExpired()
    {
        time_t currentTime = time(NULL);
        bool hasExpired = currentTime - startTime > searchWindow && currentBestSolutionCost != -1;
        return hasExpired;
    }
    void run(int empleado = 0, int turno = 0)
    {

        if (hasTimeWindowExpired()) {
            cout << "Time Window expired, stoping search" << endl;
            return;
        }
        Empleado emp = empleados[empleado];
        if (empleado != lastEmployee) {
            cout << "Testing employee: " << emp.id << endl;
            lastEmployee = empleado;
        }
        emp.assignedShifts.resize(turno);

        string tipoTurno = getTipoTurno(turno);
        int dia = getDiaTurno(turno);
        int duracionTurno = shifts[tipoTurno].first;
        cout << " Testing turno: " << turno << " [" << emp.id << "]" << endl;
        bool isValidTurn = checkIfWorks(emp, turno);
        Empleado cachedEmployee = emp;

        if (isValidTurn) { // Es valido y no es el turno a cambiar
            emp.currentM += duracionTurno;
            emp.currentT[tipoTurno] += 1;

            if (isWeekend(dia) && !isWeekend(emp.lastWorkedShift)) {
                emp.currentFD += 1;
            }
            emp.lastWorkedShift = turno;
            emp.currentCT += 1;
            emp.assignedShifts.push_back(1);
            empleados[empleado] = emp;
            if (turno + 1 < cantidadTurnos * h) {
                cout << " Running positive " << turno << endl;
                run(empleado, turno + 1);
                cout << "[DEB]"
                     << " Back to " << turno << " of employee " << emp.id << " (positive)" << endl;
            }
        }
        if (hasTimeWindowExpired()) {
            cout << "Time Window expired, stoping search" << endl;
            return;
        }

        if (!isValidTurn) {
            emp = cachedEmployee;
            emp.assignedShifts.push_back(0);
            emp.currentCT = 0;
            empleados[empleado] = emp;
        }
        if (turno + 1 < cantidadTurnos * h) {
            emp = cachedEmployee;
            emp.assignedShifts.push_back(0);
            emp.currentCT = 0;
            empleados[empleado] = emp;
            cout << "[DEB]"
                 << " Running negative " << turno << endl;
            run(empleado, turno + 1);
            cout << "[DEB]"
                 << " Back to " << turno << " of employee " << emp.id << " (negative)" << endl;
        }

        if (turno + 1 == cantidadTurnos * h) {
            cout << "[DEB]"
                 << " Revisando si cumple  con los requisitos minimos"
                 << turno << " of employee " << emp.id << endl;
            // emp.assignedShifts.resize(turno);
            // empleados[empleado] = emp;
            int resultado = checkFinalShifts(emp, turno); // -1,1,2,3 = Valido,  Minimo de minutos trabajados, Minimo dias libres consecutivos, Minimo de turnos consecutivos
            if (resultado == -1) {

                cout << "Turnos del empleado valido" << endl;
                if (empleado < 100) {
                    ofstream outfile;
                    outfile.open("parcial_" + to_string(empleado) + ".txt", ios::app);
                    writeCamino(emp, outfile);
                    outfile << "Costo de la solución Empleado " << calcularCostoEmpleado(emp) << endl;
                    outfile.close();
                }

                if (empleado + 1 == totalAvailableEmployees) {
                    cout << "Encontrada solución valida" << endl;
                    vector<int> tempPathSolution;
                    int costoTotalEmpleados = 0;
                    for (int i = 0; i < empleados.size(); i++) {
                        costoTotalEmpleados += calcularCostoEmpleado(empleados[i]);
                        // cout << "[DEB]" << turno << " " << i << " " << empleados[i].assignedShifts.size() << endl;
                        for (int value : empleados[i].assignedShifts) {
                            tempPathSolution.push_back(value);
                        }
                    }
                    int costoTotal = costoTotalEmpleados + calcularCostoEmpresa(tempPathSolution);
                    if (costoTotal < currentBestSolutionCost || currentBestSolutionCost == -1) {
                        cout << "Solución mejor que la existente, guardando" << endl;
                        currentBestSolutionCost = costoTotal;
                        currentBestSolutionPath = tempPathSolution;
                        ofstream outfile;
                        outfile.open("all_sollutions.txt", ios::app);
                        outfile << "Costo de la solución Actual " << currentBestSolutionCost << endl;
                        outfile.close();
                    }

                } else {
                    cout << "Continuando con otro empleado" << endl;
                    resetToTurno(empleados[empleado + 1], empleado + 1, 0);
                    return run(empleado + 1, 0);
                }
            } else {
                cout << "Esta asignación de turnos no es valida, deteniendo rama" << endl;
                // Buscar los conflictos se reduce a encontrar el último 1 y probarlo con 0
                //int lastOne = lastWorkingShift(emp);
                //cout << "Returning to " << lastOne << endl;
                // run(empleado, lastOne);
            }
        }
    }
    void write_better()
    {
        ofstream outfile;
        outfile.open("final.txt");
        outfile << "#Valor Función Objetivo:" << endl
                << currentBestSolutionCost << endl;
        outfile << "#Calendario horizonte de " << h << " días:" << endl;
        string head = "Empleados / Dias ";
        outfile << head << "|";
        for (int i = 0; i < h; i++) {
            int spaces = ((2 + to_string(cantidadTurnos).length() * 2) - to_string(i).length()) / 2;
            outfile << string(spaces, ' ') << i << string(spaces, ' ') << "|";
        }
        outfile << endl;
        int solutionPosition = 0;
        for (auto& emp : empleados) {
            outfile << emp.id << string(head.length() - emp.id.length(), ' ') << "|";
            int shift = 0;
            int i = 0;
            string toWrite;
            for (int i = 0; i < cantidadTurnos * h; i++) {

                if (shift == cantidadTurnos) {
                    shift = 0;

                    outfile << toWrite << " |";
                    toWrite = "";
                }
                // cout << "[DEB]" << emp.id << " " << solutionPosition << " | " << currentBestSolutionPath[solutionPosition] << endl;
                if (currentBestSolutionPath[solutionPosition] == 1) {
                    toWrite += getTipoTurno(shift) + ",";
                }

                shift += 1;
                solutionPosition += 1;
            }
            outfile << toWrite << " |" << endl;
        }
        outfile << "#Penalizaciones por empleados:" << endl;
        for (auto& emp : empleados) {
            outfile << emp.id << "    " << calcularCostoEmpleado(emp) << endl;
        }
        outfile << "#Tabla de cobertura de turnos y penalización por día:" << endl;
        head = "Turnos/Días ";
        outfile << head << "|";
        for (int i = 0; i < h; i++) {
            int spaces = ((6 - to_string(i).length()) / 2);
            outfile << string(spaces, ' ') << i << string(spaces, ' ') << "|";
        }
        outfile << endl;
        vector<int> sumaEmpleados(cantidadTurnos * h);
        int shift = 0;
        for (int val : currentBestSolutionPath) {
            if (shift == cantidadTurnos * h) {
                shift = 0;
            }
            sumaEmpleados[shift] += val;
            shift += 1;
        }
        shift = 0;
        map<string, vector<vector<int>>> empleadosPorTurno;
        for (int empleadosEnTurno : sumaEmpleados) {
            int dia = getDiaTurno(shift);
            string tipoTurno = getTipoTurno(shift);
            vector<int> dataDia = cover[dia][tipoTurno];
            int requiredEmployees = dataDia[0];
            int costoPorEmpleadoFaltante = dataDia[1];
            int costoPorEmpleadoSobrante = dataDia[2];
            int costoTurno = 0;
            if (requiredEmployees > empleadosEnTurno) {
                costoTurno = (requiredEmployees - empleadosEnTurno) * costoPorEmpleadoFaltante;
            } else {
                costoTurno = (empleadosEnTurno - requiredEmployees) * costoPorEmpleadoSobrante;
            }
            if (empleadosPorTurno.find(tipoTurno) == empleadosPorTurno.end()) {
                vector<int> toAssign;
                toAssign.push_back(requiredEmployees);
                toAssign.push_back(empleadosEnTurno);
                toAssign.push_back(costoTurno);
                vector<vector<int>> temp;
                temp.push_back(toAssign);
                empleadosPorTurno[tipoTurno] = temp;
            } else {
                vector<int> toAssign;

                toAssign.push_back(requiredEmployees);
                toAssign.push_back(empleadosEnTurno);
                toAssign.push_back(costoTurno);
                empleadosPorTurno[tipoTurno].push_back(toAssign);
            }
            shift += 1;
        }
        for (string turno : tiposTurno) {
            outfile << turno << string(head.length() - turno.length() - 1, ' ') << "|";
            for (auto& asignados : empleadosPorTurno[turno]) {
                string toWrite = to_string(asignados[1]) + "/" + to_string(asignados[0]) + " | ";
                outfile << toWrite;
            }
            outfile << endl;
        }
        outfile << "Costo Total | ";
        vector<int> costosDia;
        for (int i = 0; i < h; i++) {
            costosDia.push_back(0);
        }
        for (string turno : tiposTurno) {
            int dia = 0;
            for (auto& asignados : empleadosPorTurno[turno]) {
                costosDia[dia] += asignados[2];
                dia += 1;
            }
        }
        for (int costo : costosDia) {
            outfile << costo << " | ";
        }
        outfile << endl;
        outfile << "#Resumen información por empleado:" << endl;
        outfile << rawEmployees << endl;
        writeCaminoVector(currentBestSolutionPath, outfile);
        outfile.close();
    };
};

int main(int argc, char const* argv[])
{

    Main programa;
    cout << "Parsing Input" << endl;
    programa.parseInput();
    cout << "Building Structures" << endl;
    programa.buildStructures();
    cout << "Starting Program" << endl;
    programa.setSearchWindow(100);
    programa.run();
    programa.write_better();
    return 0;
}