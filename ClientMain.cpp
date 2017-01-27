
#include <iostream>
#include "src/Driver.h"
#include "src/StandardCab.h"
#include "src/LuxuryCab.h"
#include "src/TaxiCenter.h"
#include "sockets/Tcp.h"
#include "Checker.h"
#include <boost/lexical_cast.hpp>

using namespace std;

int main(int argc, char *argv[]) {
    Tcp client(0, atoi(argv[2]));
    client.initialize();
    std::list <Driver*> drivers;
    std::list <Cab*> cabs;
    TaxiCenter tc = TaxiCenter(&drivers, &cabs);
    char buffer[4096];
    //using for managing case 9
    bool hasANewTrip = true;
    char choice[2];
    do {
        client.receiveData(choice, sizeof(choice), 0);
        switch (choice[0]) {
        //create a driver
        case '1': {
            int numOfDrivers = 1;
            int numOfCabs = 1;
            while (numOfDrivers != 0) {
                string stringDriver;
                getline(cin,stringDriver);
                if (std::count(stringDriver.begin(),stringDriver.end(),',') != 4) {
                    choice[0] = 7;
                    break;
                }
                char *input[5];
                int i = 0;
                char *split;
                split = strtok(stringDriver, ",");
                while (split != NULL && i < 5) {
                    input[i] = split;
                    i++;
                    split = strtok(NULL, ",");
                }
                //char dummy;

               // cin >> id >> dummy >> age >> dummy >> status >> dummy >> exp >> dummy >> cabId;
                Checker checker (*input);
                if (!checker.CheckClientDriver()) {
                    choice[0] = 7;
                    break;
                }

                int id = atoi(input[0]);
                int age = atoi(input[1]);
                char status = *input[2];
                int exp =  atoi(input[3]);
                int cabId =  atoi(input[4]);
                Driver *d = new Driver(id, age, status, exp, cabId);
                //sending a serialized driver to server
                string str = boost::lexical_cast<string>(id) + "," + boost::lexical_cast<string>(age) + "," +
                             status + "," + boost::lexical_cast<string>(exp) + "," +
                             boost::lexical_cast<string>(cabId);
                drivers.push_back(d);
                client.sendData(str, 0);
                numOfDrivers--;
            }
            while (numOfCabs != 0) {
                //getting a serialized cab
                client.receiveData(buffer, sizeof(buffer), 0);
                char *cab[4];
                int i = 0;
                char *split;
                split = strtok(buffer, ",");
                while (split != NULL && i < 4) {
                    cab[i] = split;
                    i++;
                    split = strtok(NULL, ",");
                }
                //create a cab
                if (*cab[1] == '1') {
                    StandardCab *c = new StandardCab(atoi(cab[0]), atoi(cab[1]), *cab[2], *cab[3]);
                    c->setLocation(Point(0, 0));
                    cabs.push_back(c);
                } else {
                    LuxuryCab *c = new LuxuryCab(atoi(cab[0]), atoi(cab[1]), *cab[2], *cab[3]);
                    c->setLocation(Point(0, 0));
                    cabs.push_back(c);
                }
                numOfCabs--;
            }
            tc.assignCabsToDrivers();
            break;
        }
            //updating that we have a new trip in delivery
        case '2':
            hasANewTrip = true;
            break;
        case '9':
            //checking if there are trips
            if (hasANewTrip) {
                //now we took care of the new trip. therefore  we are updating the hasANewTrip to false.
                hasANewTrip = false;
                //else we dont have a new trip
            } else {
                client.sendData("a", 0);
                //client receive a new location of each driver
                client.receiveData(buffer, sizeof(buffer), 0);

                char *point[3];
                int j = 0;
                char *split;
                split = strtok(buffer, ",");
                while (split != NULL && j < 3) {
                    point[j] = split;
                    j++;
                    split = strtok(NULL, ",");
                }
                list<Cab *>::iterator cabsIteratorStart = cabs.begin();
                list<Cab *>::iterator cabsIteratorEnd = cabs.end();
                while (cabsIteratorStart != cabsIteratorEnd) {
                    //updating the location for the right cab
                    if ((*cabsIteratorStart)->getId() == atoi(point[0])) {
                        (*cabsIteratorStart)->setLocation(Point(atoi(point[1]), atoi(point[2])));
                    }
                    cabsIteratorStart++;
                }
            }
            break;
        default:
            break;
        }
    } while (choice[0] != '7');
    drivers.clear();
    cabs.clear();
    return 0;
}


