#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

const int baseFareCar = 50;
const int baseFareBike = 30;
int totalVehicles = 0;
int numOfCars = 0, numOfBikes = 0;
int numOfSlots;

typedef struct{
    int hour;
    int min;
    int sec;
}TIME;

TIME storeTIME(time_t time);
void printtime(TIME &t);
int calculatetimediff(TIME &in, TIME &out);
int slotSelector(int type, int NOS, vector<int>& UsedSlots);

class vehicle{
    public:
        int vNo;
        char vLicensePlate[11];
        string vDate;
        TIME vInTime;
        TIME vOutTime;
        int totalTime;
        vehicle(int t=0){
            totalTime = t;
            vDate = __DATE__;
        }
        virtual ~vehicle() {}
        virtual void calFare(){ cout << "Calculating Fare";}
        virtual void displayDetails(){ cout << "Displaying Data";}
};

class car:public vehicle{
    int cParkingSlot;
    int cfare = 0;
    public:
    void calFare(){ cfare = baseFareCar*totalTime;}
    void displayDetails(){
        cout << vNo << endl << vLicensePlate << endl << vDate << endl << "In Time: ";
        printtime(vInTime);
        cout << endl << "Out Time: ";
        printtime(vOutTime);
        cout << endl << "Fare: " << cfare << endl << "Parking Slot: " << cParkingSlot << endl;
    }
    void slotpick(int n,vector<int>& UsedSlots);
    void inputdata();
    int checkout();
};

class bike:public vehicle{
    int bParkingSlot;
    int bfare = 0;
    public:
    void calFare(){ bfare = baseFareBike*totalTime;}
    void displayDetails(){
        cout << vNo << endl << vLicensePlate << endl << vDate << endl << "In Time: ";
        printtime(vInTime);
        cout << endl << "Out Time: ";
        printtime(vOutTime);
        cout << endl << "Fare: " << bfare << endl << "Parking Slot: " << bParkingSlot << endl;
    }
    void slotpick(int n,vector<int>& UsedSlots);
    void inputdata();
    int checkout();
};

vector<car> cars;
vector<bike> bikes;

// CAR input output ----------------------------------------------
void car::inputdata(){
    char licenseplate[11];
    time_t currentTime = time(nullptr);
    vNo = ++numOfCars;
    cin >> licenseplate;
    memcpy(vLicensePlate,licenseplate,10);
    vLicensePlate[10] = '\0';
    for (int i = 0; i < strlen(vLicensePlate); i++) {
        vLicensePlate[i] = toupper(vLicensePlate[i]);
    }
    vInTime = storeTIME(currentTime);
    vOutTime = storeTIME(currentTime);
}

int car::checkout(){
    time_t currentTime = time(nullptr);
    vOutTime = storeTIME(currentTime);
    totalTime = calculatetimediff(vInTime,vOutTime);
    calFare();
    int slot = cParkingSlot;
    cParkingSlot = 0;
    return slot;
}

inline void car::slotpick(int n,vector<int>& UsedSlots){
    cParkingSlot = slotSelector(n,numOfSlots,UsedSlots);
}

// ---------------------------------------------------------------
// BIKE input output ---------------------------------------------
void bike::inputdata(){
    char licenseplate[11];
    time_t currentTime = time(nullptr);
    vNo = ++numOfBikes;
    cin >> licenseplate;
    memcpy(vLicensePlate,licenseplate,10);
    vLicensePlate[10] = '\0';
    for (int i = 0; i < strlen(vLicensePlate); i++) {
        vLicensePlate[i] = toupper(vLicensePlate[i]);
    }
    vInTime = storeTIME(currentTime);
    vOutTime = storeTIME(currentTime);
}

int bike::checkout(){
    time_t currentTime = time(nullptr);
    vOutTime = storeTIME(currentTime);
    totalTime = calculatetimediff(vInTime,vOutTime);
    calFare();
    int slot = bParkingSlot;
    bParkingSlot = 0;
    return slot;
}

inline void bike::slotpick(int n,vector<int>& UsedSlots){
    bParkingSlot = slotSelector(n,numOfSlots,UsedSlots);
}

// ----------------------------------------------------------------

// TIME reading and storing functions--------------------------
void printtime(TIME& t) {
    cout << setw(2) << setfill('0') << t.hour << ":";
    cout << setw(2) << setfill('0') << t.min << ":";
    cout << setw(2) << setfill('0') << t.sec;
}

TIME storeTIME(time_t time){
    TIME t;
    tm *timeinfo = localtime(&time);

    t.hour = timeinfo->tm_hour;
    t.min = timeinfo->tm_min;
    t.sec = timeinfo->tm_sec;

    return t;
}

int calculatetimediff(TIME &in, TIME &out){
    int hours = out.hour - in.hour;
    if(hours < 0){ hours += 24;}
    int mins = out.min - in.min;
    if(mins < 0){ hours -= 1; mins += 60;}
    int secs = out.sec - in.sec;
    if(secs < 0){ mins -= 1; secs += 60;}
    double dInHours = ((hours*3600)+(mins*60)+(secs))/3600;
    int t = ceil(dInHours);
    if (t==0) t=1;
    return t;
}

// --------------------------------------------------------------

// SLOTS --------------------------------------------------------
int* createArray(int n) {
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        arr[i] = i + 1;
    }
    return arr;
}

int slotSelector(int type, int NOS, vector<int>& UsedSlots) {
    srand((unsigned) time(NULL));
    int NOS_for_car = (NOS * 0.8);
    int NOS_for_bike = NOS - NOS_for_car;

    if (type == 1) {
        while (true) {
            int random = rand() % NOS_for_car+1;
            bool slotExists = false;

            for (int i = 0; i < UsedSlots.size(); ++i) {
                if (random == UsedSlots[i]) {
                    slotExists = true;
                    break;
                }
            }

            if (!slotExists) {
                UsedSlots.push_back(random);
                return random;
            }
        }
    }
    else if (type == 2) {
        while (true) {
            int random = rand() % NOS_for_bike+1;
            random += NOS_for_car;
            bool slotExists = false;

            for (int i = 0; i < UsedSlots.size(); ++i) {
                if (random == UsedSlots[i]) {
                    slotExists = true;
                    break;
                }
            }

            if (!slotExists) {
                UsedSlots.push_back(random);
                return random;
            }
        }
    }
    else { 
        cout << "Invalid Type" << endl;
    }
    return -1;
}

// ---------------------------------------------------------------

int main(){
    cout << "Enter number of parking slots: ";
    cin >> numOfSlots;
    vector<int> usedSlots;
    while(true){
        if (numOfSlots < 1) {
            cout << "Please enter a positive number." << endl;
            cin >> numOfSlots;
        } else if(numOfSlots < 50){
            cout <<"Atleast 50 slots are required!" << endl;
            cin >> numOfSlots;
        } else{
            break;
        }
    }
    
    int* AvailableSlots = createArray(numOfSlots);    

    int choice, type;

    cout << "MENU:" << endl;
    cout << "1: Vehicle Enter" << endl;
    cout << "2: Vehicle Exit" << endl;
    cout << "3: Display Parking" << endl;
    cout << "4: Search Vehicle" << endl;
    cout << "0: Exit" << endl;
    cout << "Enter Choice:";

    while(choice!=0){
        cin >> choice;
        switch(choice){
            case 1:
            // vehicle enter
                cout << "Enter vehicle type (1 for car, 2 for bike): ";
                cin >> type;
                if (type == 1) {
                    car c;
                    cout << "Enter car license plate : ";
                    c.inputdata();
                    c.slotpick(1,usedSlots);
                    cars.push_back(c);
                } else if (type == 2) {
                    bike b;
                    cout << "Enter bike license plate : ";
                    b.inputdata();
                    b.slotpick(2,usedSlots);
                    bikes.push_back(b);
                } else {
                    cout << "Invalid vehicle type! Re-enter choice." << endl;
                    break;
                }
                cout << "Successful!" << endl;
                cout << endl << "Enter choice: ";
                break;

            case 2:
            // vehicle exit
                cout << "Enter vehicle type (1 for car, 2 for bike): ";
                cin >> type;
                if (type == 1 && !cars.empty()) {
                    char licensePlate[10];
                    cout << "Enter car license plate: ";
                    cin >> licensePlate;
                    for (int i = 0; i < strlen(licensePlate); i++) {
                        licensePlate[i] = toupper(licensePlate[i]);
                    }
                    bool found = false;
                    for (auto& c : cars) {
                        if (strcmp(c.vLicensePlate, licensePlate) == 0) {
                            int slot = c.checkout();
                            c.displayDetails();
                            auto it = find(usedSlots.begin(), usedSlots.end(), slot);
                            if (it != usedSlots.end())
                                *it = -1;
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << "Car with license plate " << licensePlate << " not found." << endl;
                } else if (type == 2 && !bikes.empty()) {
                    char licensePlate[10];
                    cout << "Enter bike license plate: ";
                    cin >> licensePlate;
                    for (int i = 0; i < strlen(licensePlate); i++) {
                        licensePlate[i] = toupper(licensePlate[i]);
                    }
                    bool found = false;
                    for (auto& b : bikes) {
                        if (strcmp(b.vLicensePlate, licensePlate) == 0) {
                            int slot = b.checkout();
                            b.displayDetails();
                            auto it = find(usedSlots.begin(), usedSlots.end(), slot);
                            if (it != usedSlots.end())
                                *it = -1;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "Bike with license plate " << licensePlate << " not found." << endl;
                    }
                } else {
                    cout << "No vehicles of the specified type found or vector is empty." << endl;
                    break;
                }
                cout << endl << "Enter choice: ";
                break;
            case 3:
            // display data
                cout << "Cars:" << endl;
                for (auto& c : cars) {
                    c.displayDetails();
                    cout << ">----------------------------<"<< endl;
                }
                cout << "==========================================" << endl;
                cout << "Bikes:" << endl;
                for (auto& b : bikes) {
                    b.displayDetails();
                    cout << ">----------------------------<"<< endl;
                }
                cout << "==========================================" << endl;
                cout << endl << "Enter choice: ";
                break;

            case 4:
            // search data
                int searchChoice;
                cout << "Search by:" << endl;
                cout << "1. License Plate" << endl;
                cout << "2. In Time (Hour)" << endl;
                cin >> searchChoice;
                if (searchChoice == 1) {
                    char searchLicensePlate[10];
                    cout << "Enter license plate to search for: ";
                    cin >> searchLicensePlate;
                    // Search by license plate
                    for (int i = 0; i < strlen(searchLicensePlate); i++) {
                        searchLicensePlate[i] = toupper(searchLicensePlate[i]);
                    }
                    cout << "Search results:" << endl;
                    bool found = false;
                    cout << "Cars: " << endl;
                    for (auto& c : cars) {
                        if (strcmp(c.vLicensePlate, searchLicensePlate) == 0) {
                            c.displayDetails();
                            found = true;
                        }
                    }
                    cout << "Bikes: " << endl;
                    for (auto& b : bikes) {
                        if (strcmp(b.vLicensePlate, searchLicensePlate) == 0) {
                            b.displayDetails();
                            found = true;
                        }
                    }
                    if (!found) {
                        cout << "No vehicles found with the specified license plate." << endl;
                    }
                } else if (searchChoice == 2) {
                    int searchHour;
                    cout << "Enter hour to search for: ";
                    cin >> searchHour;

                    // Search by hour of vInTime
                    cout << "Search results:" << endl;
                    bool found = false;
                    for (auto& c : cars) {
                        if (c.vInTime.hour == searchHour) {
                            c.displayDetails();
                            found = true;
                        }
                    }
                    for (auto& b : bikes) {
                        if (b.vInTime.hour == searchHour) {
                            b.displayDetails();
                            found = true;
                        }
                    }
                    if (!found) {
                        cout << "No vehicles found with the specified hour of vInTime." << endl;
                    }
                }
                cout << endl << "Enter choice: ";
                break;
            default:
                // input handling if choice is not 0
                if(choice == 0){
                    cout << "Thanks for using the PARKING LOT MANAGEMENT SYSTEM" << endl;
                    break;
                }
                else cout << "Please input a valid choice!" << endl;
                cout << endl << "Enter choice: ";
                break;
        }
    }
}