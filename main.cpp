#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Route {
public:
    Route() {} 

    Route(string id, string source, string destination, int distance)
        : id(id), source(source), destination(destination), distance(distance) {}

    string getId() { return id; }

    string getSource() { return source; }

    string getDestination() { return destination; }

    int getDistance() { return distance; }

    friend Route* readRoutesFromFile(ifstream& file, const int& routes_count);
    friend void displayArrayOfRoutes(
        Route* arrayRoutes,
        const int& routes_count); // ta funkcja tylko dla sprawdzenia, ze faktycznie pobralo z pliku

private:
    string id;
    string source;
    string destination;
    int distance;
};

void displayArrayOfRoutes(Route* arrayRoutes, const int& routes_count) { // ta funkcja tylko dla sprawdzenia, ze
    // faktycznie pobralo z pliku
    for (int i = 0; i < routes_count; ++i) {
        cout << arrayRoutes[i].id << endl; 
        cout << arrayRoutes[i].source << endl;
        cout << arrayRoutes[i].destination << endl;
        cout << arrayRoutes[i].distance << endl << endl;
    }
}

// klasa abstrakcyjna
class Transport {
public:
    Transport(int total_seats = 50) : total_seats(total_seats) {
        for (int i = 1; i <= total_seats; ++i) {
            seats.push_back(false); // dodajemy do vectora bool false, czyli niezarezerwowane miejsca
        }
    }

    Transport(Route road, string id, string departure, string arrival)
        : road(road), id(id), departure_time(departure), arrival_time(arrival) {}
    virtual ~Transport() {}; 

    void setRoute(Route chosenRoute) { road = chosenRoute; }

    void setId(string id) { this->id = id; }

    void setDepartureTime(string departure_time) {
        this->departure_time = departure_time;
    }

    void setArrivalTime(string arrival_time) {
        this->arrival_time = arrival_time;
    }

    string getId() { return id; }

    string getDepartureTime() { return departure_time; }

    string getArrivalTime() { return arrival_time; }

    int getTotalSeats() { return total_seats; }

    Route getRoad() { return road; }

    /* Funkcja do sczytywania z pliku - Rozkladu wybranego transportu
     * @param scheduleFile - plik, z ktorego odczytujemy dane
     * @param chosenRoute - wybrana przez uzytkownika trasa
     * @param run_count - ilosc polaczen, odczytywana z pliku
     * @param route_choice - numer wybranej przez uzytkownika trasy
     */
    virtual Transport* readScheduleFromFile(ifstream& schedule_file, const Route& chosen_route,
        int& run_count, const int& route_choice) = 0;
    virtual void displaySchedule() = 0;

    bool reserveSeat(int seat_number) {
        if (seat_number >= 1 && seat_number <= total_seats && !seats[seat_number - 1]) {
            seats[seat_number - 1] = true;
            return true;
        }
        return false;
    }

    void display_seats() {
        cout << "\t\t\tTotal Seats: " << total_seats << endl;
        cout << "\t\t----- Availabe Seat -----\n";
        bool flag = false;
        for (int i = 0; i < total_seats; i++) {
            if (!seats[i]) {
                cout << "\t\t" << i + 1;
            }
            else {
                cout << "\t\tNA";
            }
            if (!flag) {
                cout << "\t";
                flag = true;
            }
            else {
                cout << "\n";
                flag = false;
            }
        }
        cout << endl;
    }

protected:
    Route road;
    string id;
    string departure_time;
    string arrival_time;
    int total_seats;
    vector<bool> seats;
};

class Plane : public Transport {
public:
    Plane(int total_seats = 50)
        : Transport(total_seats) { // w konstruktorze wypelniamy wektor bool
        // wolnymi miejscami
    }
    ~Plane(
    ) override {};

    /* Funkcja do sczytywania z pliku - Rozklad lotow
     * @param scheduleFile - plik, z ktorego odczytujemy dane
     * @param chosenRoute - wybrana przez uzytkownika trasa
     * @param run_count - ilosc polaczen, odczytywana z pliku
     * @param route_choice - numer wybranej przez uzytkownika trasy
     */
    Transport* readScheduleFromFile(ifstream& schedule_file,
        const Route& chosen_route, int& run_count,
        const int& route_choice) override {
        schedule_file >> run_count; // sczytujemy z pliku ilosc rejsow w jednym dniu
        int nr_line;                // zaczniemy odczytywac plik od tej linii
        if (route_choice == 1)
            nr_line = 5;
        else if (route_choice == 2)
            nr_line = 13;
        else
            nr_line = 21;
        string line; // zmienna pomocnicza do odczytu poszczegolnej linii z pliku

        for (int i = 1; i < nr_line; ++i) { // petla aby dostac sie do lini nr_linii
            getline(schedule_file, line);
        }

        Plane* array_planes = new Plane[run_count]; // stworzenie tablicy dynamicznej na dostepne
        // rejsy lotow
        int nr_flight = 0;

        string temp;
        // 1 iteracja petli sczytuje informacje o 1 rejsie do elementu tablicy lotow
        // cala petla wypelni tablice dostepnych rejsow na wybrany przez uzytkownika
        // kierunek
        while (nr_flight < run_count) {
            array_planes[nr_flight].setRoute(chosen_route);
            schedule_file >> temp;
            array_planes[nr_flight].setId(temp);
            schedule_file >> temp;
            array_planes[nr_flight].setDepartureTime(temp);
            schedule_file >> temp;
            array_planes[nr_flight].setArrivalTime(temp);

            ++nr_line;
            ++nr_flight;
        }
        return array_planes;
    }

    void displaySchedule() override {
        cout << id << "\t" << departure_time << "\t\t" << arrival_time;
    }
};

class Train : public Transport {
public:
    Train(int total_seats = 40)
        : Transport(total_seats) { // w konstruktorze wypelniamy wektor bool
        // wolnymi miejscami
    }
    ~Train() override {};

    /* Funkcja do sczytywania z pliku - Rozklad lotow
     * @param scheduleFile - plik, z ktorego odczytujemy dane
     * @param chosenRoute - wybrana przez uzytkownika trasa
     * @param run_count - ilosc polaczen, odczytywana z pliku
     * @param route_choice - numer wybranej przez uzytkownika trasy
     */
    Transport* readScheduleFromFile(ifstream& schedule_file,
        const Route& chosen_route, int& run_count,
        const int& route_choice) override {
        schedule_file >> run_count; // sczytujemy z pliku ilosc rejsow w jednym dniu
        int nr_line;                // zaczniemy odczytywac plik od tej linii
        if (route_choice == 1)
            nr_line = 5;
        else if (route_choice == 2)
            nr_line = 13;
        else
            nr_line = 21;
        string line; // zmienna pomocnicza do odczytu poszczegolnej linii z pliku

        for (int i = 1; i < nr_line; ++i) { // petla aby dostac sie do linii
            // nr_linii
            getline(schedule_file, line);
        }

        Train* array_traines = new Train[run_count]; // stworzenie tablicy dynamicznej na dostepne
        // rejsy pociagow
        int nr_flight = 0;

        string temp;
        // 1 iteracja petli sczytuje informacje o 1 rejsie do elementu tablicy
        // pociagow cala petla wypelni tablice dostepnych rejsow na wybrany przez
        // uzytkownika kierunek
        while (nr_flight < run_count) {
            array_traines[nr_flight].setRoute(chosen_route);
            schedule_file >> temp;
            array_traines[nr_flight].setId(temp);
            schedule_file >> temp;
            array_traines[nr_flight].setDepartureTime(temp);
            schedule_file >> temp;
            array_traines[nr_flight].setArrivalTime(temp);

            ++nr_line;
            ++nr_flight;
        }
        return array_traines;
    }

    void displaySchedule() override {
        cout << id << "\t" << departure_time << "\t\t" << arrival_time;
    }
};

class Bus : public Transport {
public:
    Bus(int total_seats = 30) : Transport(total_seats) { // w konstruktorze wypelniamy wektor bool wolnymi miejscami
    }
    ~Bus() override {};

    /* Funkcja do sczytywania z pliku - Rozklad lotow
     * @param scheduleFile - plik, z ktorego odczytujemy dane
     * @param chosenRoute - wybrana przez uzytkownika trasa
     * @param run_count - ilosc polaczen, odczytywana z pliku
     * @param route_choice - numer wybranej przez uzytkownika trasy
     */
    Transport* readScheduleFromFile(ifstream& schedule_file,
        const Route& chosen_route, int& run_count,
        const int& route_choice) override {
        schedule_file >> run_count; // sczytujemy z pliku ilosc rejsow w jednym dniu
        int nr_line;                // zaczniemy odczytywac plik od tej linii
        if (route_choice == 1)
            nr_line = 5;
        else if (route_choice == 2)
            nr_line = 13;
        else
            nr_line = 21;
        string line; // zmienna pomocnicza do odczytu poszczegolnej linii z pliku

        for (int i = 1; i < nr_line; ++i) { // petla aby dostac sie do lini nr_linii
            getline(schedule_file, line);
        }

        Bus* array_buses = new Bus[run_count]; // stworzenie tablicy dynamicznej na
        // dostepne rejsy busow
        int nr_flight = 0;

        string temp;
        // 1 iteracja petli sczytuje informacje o 1 rejsie do elementu tablicy busow
        // cala petla wypelni tablice dostepnych rejsow na wybrany przez uzytkownika
        // kierunek
        while (nr_flight < run_count) {
            array_buses[nr_flight].setRoute(chosen_route);
            schedule_file >> temp;
            array_buses[nr_flight].setId(temp);
            schedule_file >> temp;
            array_buses[nr_flight].setDepartureTime(temp);
            schedule_file >> temp;
            array_buses[nr_flight].setArrivalTime(temp);

            ++nr_line;
            ++nr_flight;
        }
        return array_buses;
    }

    void displaySchedule() override {
        cout << id << "\t" << departure_time << "\t\t" << arrival_time;
    }
};

/* Funkcje pomocnicza do wczytywania z pliku - Tras
 * @param file - plik, z ktorego odczytujemy dane o dostepnych trasach
 * @param routes_count - ilosc dostepnych tras
 */
Route* readRoutesFromFile(ifstream& file, const int& routes_count) {
    int nr_line = 1;
    string line; // zmienna pomocnicza do odczytu poszczegolnej linii z pliku

    Route* array_routes = new Route[routes_count]; // stworzenie tablicy dynamicznej na dostepne
    // trasy
    int nr_route = 0;

    while (getline(file, line)) {
        switch (nr_line) {
        case 2:
            array_routes[nr_route].id = line;
            break;
        case 3:
            array_routes[nr_route].source = line;
            break;
        case 4:
            array_routes[nr_route].destination = line;
            break;
        case 5:
            array_routes[nr_route].distance = atoi(line.c_str());
            break;
        }
        ++nr_line;
        if (nr_line == 6) { // przejscie do wierszu z kolejna trasa
            nr_line = 2;
            ++nr_route;
        }
    }
    return array_routes;
}

/*Funkcja w której klient potwierdza wybranie miejsca oraz zapisuje potwierdzenie do pliku
 * @param chosen_transport - wybrany przez użytkownika transport
 * @param chosen_route - wybrana przez użytkownika trasa
 * @param nr_run - numer rejsu
 * @param choice - wybrane przez użytkownika miejsce
*/
void displayConfirmationAndSaveToFile(Transport& chosen_transport, Route& chosen_route, int& nr_run, int& choice, 
    string& first_name, string& last_name) {
    cout << "\n======= Confirmation and Save to File =======\n";
    cout << "Client Information:\n";
    cout << "First Name: " << first_name << "\n";
    cout << "Last Name: " << last_name << "\n\n";
    cout << "Route Information:\n";
    cout << "ID Number: " << chosen_route.getId() << endl;
    cout << "Source: " << chosen_route.getSource() << endl;
    cout << "Destination: " << chosen_route.getDestination() << endl;
    cout << "Distance: " << chosen_route.getDistance() << " km\n\n";

    cout << "Run Information:\n";
    cout << "ID: " << chosen_transport.getId()
        << "\tDeparture: " << chosen_transport.getDepartureTime()
        << "\tArrival: " << chosen_transport.getArrivalTime() << endl;

    cout << "Reserved Seat Information:\n";
    cout << "Seat Number: " << choice << endl;
    cout << "==============================================\n";

    // Zapis do pliku raportu
    ofstream report_file("confirmation_report.txt", ios::app);
    if (report_file.is_open()) {
        report_file << "Client Information:\n";
        report_file << "First Name: " << first_name << "\n";
        report_file << "Last Name: " << last_name << "\n\n";
        report_file << "Reservation Details:\n\n";
        report_file << "Route Information:\n";
        report_file << "ID Number: " << chosen_route.getId() << endl;
        report_file << "Source: " << chosen_route.getSource() << endl;
        report_file << "Destination: " << chosen_route.getDestination() << endl;
        report_file << "Distance: " << chosen_route.getDistance() << " km\n\n";

        report_file << "Run Information:\n";
        report_file << "ID: " << chosen_transport.getId()
            << "\tDeparture: " << chosen_transport.getDepartureTime()
            << "\tArrival: " << chosen_transport.getArrivalTime() << endl;

        report_file << "Reserved Seat Information:\n";
        report_file << "Seat Number: " << choice << endl;
        report_file << "==============================================\n";

        cout << "Confirmation and details saved to 'confirmation_report.txt'.\n";
        report_file.close();
    }
    else {
        cerr << "Unable to open 'confirmation_report.txt' for writing.\n";
    }
}

int main() {
    // Inicjalizacja systemu, wczytanie danych o dostepnych trasach z pliku.
    ifstream routes_file;
    routes_file.open("routes.txt", ios::in);
    if (!routes_file.is_open()) { // sprawdza czy plik istnieje, jesli nie - to
        // konczy program
        cerr << "Unable to open routes.txt";
        exit(0);
    }
    int routes_count;
    routes_file >> routes_count; // pierwsza linia pliku zawiera ilosc tras w bazie danych

    Route* array_routes = readRoutesFromFile(routes_file, routes_count);
    // displayArrayOfRoutes(arrayRoutes, routes_count);  // sprawdzenie, czy udalo sie pobrac dane z pliku
    routes_file.close();

    string first_name;
    string last_name;
    int choice;
    int route_choice; // numer wybranej przez uzytkownika trasy
    Route chosen_route;
    do {
        // Wyswietlenie menu poczatkowego
        cout << "===== Welcome to the Reservation System =====\n\n";
        cout << "==================== Menu ====================\n";
        for (int i = 0; i < routes_count; ++i) {
            cout << "\t\t" << i + 1 << ". " << array_routes[i].getSource() << " to "
                << array_routes[i].getDestination() << endl;
        }
        cout << "\t\t0. Exit\n";
        cout << "==============================================\n";
        cout << "\tChoose a route (enter the number): ";
        cin >> choice;

        if (choice > 0 && choice <= routes_count) {
            // Wyswietlenie informacji o wybranym kierunku podrozy
            chosen_route = array_routes[choice - 1];
            cout << "\n\t\tRoute Information:\n";
            cout << "\t\tID Number: " << chosen_route.getId() << endl;
            cout << "\t\tSource: " << chosen_route.getSource() << endl;
            cout << "\t\tDestination: " << chosen_route.getDestination() << endl;
            cout << "\t\tDistance: " << chosen_route.getDistance() << " km\n\n";
        }
        else if (choice != 0) {
            cout << "\nInvalid choice. Please try again.\n";
        }
        else exit(0);
    } while (choice < 0 || choice > routes_count);

    route_choice = choice;
    Transport* chosen_transport = new Plane; // wskaznik na pierwszy rejs w rozkladzie wybranego typu transportu
    int number_of_runs = 0; // ilosc rejsow na trasie
    do {
        // Wyswietlenie menu po wyborze kierunku podrozy
        cout << "==================== Menu ====================\n";
        cout << "\t\t1. Display Planes\n";
        cout << "\t\t2. Display Trains\n";
        cout << "\t\t3. Display Buses\n";
        cout << "\t\t0. Exit\n";
        cout << "==============================================\n";
        cout << "\tChoose a transport (enter the number): ";
        cin >> choice;

        if (choice > 0 && choice <= 3) {
            // Wyswietlenie rozkladu wybranego rodzaju transportu na wybrana trase
            ifstream schedule_file;
            switch (choice) {
            case 1:
                schedule_file.open("Schedule_planes.txt", ios::in);
                if (!schedule_file.is_open()) { // sprawdza czy plik istnieje
                    cerr << "Unable to open file Schedule_planes.txt";
                    exit(0);
                }
                delete chosen_transport;
                chosen_transport = new Plane;
                chosen_transport = chosen_transport->readScheduleFromFile(  // number_of_runs wroci z odpowiednia liczba 
                    schedule_file, chosen_route, number_of_runs, route_choice); // po wywolaniu funkcji
                schedule_file.close();
                break;
            case 2:
                schedule_file.open("Schedule_traines.txt", ios::in);
                if (!schedule_file.is_open()) { // sprawdza czy plik istnieje
                    cerr << "Unable to open file Schedule_traines.txt";
                    exit(0);
                }
                delete chosen_transport;
                chosen_transport = new Train;
                chosen_transport = chosen_transport->readScheduleFromFile(
                    schedule_file, chosen_route, number_of_runs, route_choice);
                schedule_file.close();
                break;
            case 3:
                schedule_file.open("Schedule_buses.txt", ios::in);
                if (!schedule_file.is_open()) { // sprawdza czy plik istnieje
                    cerr << "Unable to open file Schedule_buses.txt";
                    exit(0);
                }
                delete chosen_transport;
                chosen_transport = new Bus;
                chosen_transport = chosen_transport->readScheduleFromFile(
                    schedule_file, chosen_route, number_of_runs, route_choice);
                schedule_file.close();
                break;
            }
        }
        else if (choice != 0) {
            cout << "Invalid choice. Please try again.\n";
        }
        else exit(0);
    } while (choice < 0 || choice > 3);

    do {
        // Wyswietlenie rozkladu oraz menu po wyborze rodzaju transportu
        cout << "\n=========== Schedule for the roate ===========\n";
        cout << "\tfrom " << chosen_transport[0].getRoad().getSource() << " to "
            << chosen_transport[0].getRoad().getDestination() << ":\n";
        cout << "    ID"
            << "\t\tDeparture\t"
            << "Arrival\n";
        for (int i = 0; i < number_of_runs; ++i) {
            cout << "   " << i + 1 << ".";
            chosen_transport[i].displaySchedule();
            cout << endl;
        }
        cout << "\t\t0. Exit\n";
        cout << "==============================================\n";
        cout << "\tChoose a run (enter the number): ";
        cin >> choice; // numer konkretnego rejsu z rozkladu

        if (choice < 0 || choice >= number_of_runs) {
            cout << "Invalid choice. Please try again.\n";
        }
        else if (choice == 0)
            exit(0);
    } while (choice < 0 || choice > number_of_runs);

    int nr_run = choice; // zapamietujemy numer konkretnego rejsu z rozkladu
    do {
        // wyswietlenie dostepnych miejsc na wybrany rejs
        chosen_transport[nr_run - 1].display_seats();
        cout << "\t\t0. Exit\n";
        cout << "==============================================\n";
        cout << "\tChoose the seat number you want to book: ";
        cin >> choice;

        if (choice > 0 && choice <= chosen_transport[nr_run - 1].getTotalSeats()) {
            if (chosen_transport[nr_run - 1].reserveSeat(choice)) {
                // Miejsce zostało pomyślnie zarezerwowane
                // Potwierdzenie wyboru i zapis do pliku raportu
                cout << "Seat " << choice << " has been reserved\n";
                cout << "Please enter your first and last name\n";
                cout << "First name: ";
                cin >> first_name;
                cout << "Last name: ";
                cin >> last_name;
                displayConfirmationAndSaveToFile(chosen_transport[nr_run - 1],
                    chosen_route, nr_run, choice, first_name, last_name);
            }
            else {
                // Miejsce jest już zajęte
                cout << "Error: Seat " << choice
                    << " is already occupied. Please choose another seat.\n";
            }
        }
        else if (choice != 0) {
            cout << "Invalid choice. Please try again.\n";
        }
        else if (choice == 0)
            exit(0);

    } while (choice < 0 || choice > chosen_transport[nr_run - 1].getTotalSeats());

    delete[] chosen_transport;
    delete[] array_routes;

    return 0;
}