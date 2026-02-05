#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

const string clientFile = "client.txt";

void showMainMenue();

struct sClient {
    string accountNumber;
    string pinCode;
    string name;
    string phone;
    double accountBalance;
    bool markForDelete = false;
};

vector <string> splitString(string s1, string delim) {
    vector <string> vString;
    short pos = 0;
    string word;
    while ((pos = s1.find(delim)) != string::npos) {
        word = s1.substr(0, pos);
        if (word != "") {
            vString.push_back(word);
        }
        s1.erase(0, pos + delim.length());
    }
    if (s1 != "") {
        vString.push_back(s1);
    }
    return vString;
}

sClient convertLineToRecord(string line, string delim) {
    sClient client;
    vector <string> vClientData;
    vClientData = splitString(line, delim);
    client.accountNumber = vClientData[0];
    client.pinCode = vClientData[1];
    client.name = vClientData[2];
    client.phone = vClientData[3];
    client.accountBalance = stod(vClientData[4]);

    return client;
}

string convertRecordToLine(sClient client, string delim) {
    string record = "";
    record += client.accountNumber + delim;
    record += client.pinCode + delim;
    record += client.name + delim;
    record += client.phone + delim;
    record += to_string(client.accountBalance);

    return record;
}

bool clientExitstsByAccountNumber(string fileName, string accountNumber) {
    vector <sClient> vClients;
    fstream myFile;
    myFile.open(fileName, ios::in);
    if (myFile.is_open()) {
        string line;
        sClient client;
        while (getline(myFile, line)) {
            client = convertLineToRecord(line, "#//#");
            if (client.accountNumber == accountNumber) {
                myFile.close();
                return true;
            }
            vClients.push_back(client);
        }
        myFile.close();
    }
    return false;
}

sClient readNewClient() {
    sClient client;
    cout << "\nEnter account number?";
    getline(cin >> ws, client.accountNumber);
    while (clientExitstsByAccountNumber(clientFile, client.accountNumber)) {
        cout << "\nClient with [" << client.accountNumber << "] already exists, Enter another Account Number ? ";
        getline(cin >> ws, client.accountNumber);
    }
    cout << "\nEnter Pin code?";
    getline(cin, client.pinCode);
    cout << "\nEnter name?";
    getline(cin, client.name);
    cout << "\nEnter Phone number?";
    getline(cin, client.phone);
    cout << "\nEnter account balance?";
    cin >> client.accountBalance;

    return client;
}

vector <sClient> loadClientsDataFromFile(string fileName) {
    vector <sClient> vClients;
    fstream myFile;
    myFile.open(fileName, ios::in);
    if (myFile.is_open()) {
        string line;
        sClient client;
        while (getline(myFile, line)) {
            client = convertLineToRecord(line, "#//#");
            vClients.push_back(client);
        }
        myFile.close();
    }
    return vClients;
}

void printClientRecordLine(sClient Client){
    cout << "| " << setw(15) << left << Client.accountNumber;
    cout << "| " << setw(10) << left << Client.pinCode;
    cout << "| " << setw(40) << left << Client.name;
    cout << "| " << setw(12) << left << Client.phone;
    cout << "| " << setw(12) << left << Client.accountBalance;
}

void showAllClientsScreen() {
    vector <sClient> vClients = loadClientsDataFromFile(clientFile);
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0) {
        cout << "\t\t\t\tNo Clients Available In the System!";
    }
    else {
        for (sClient& client : vClients) {
            printClientRecordLine(client);
            cout << endl;
        }
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}


void printClientCard(sClient Client){
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.accountNumber;
    cout << "\nPin Code : " << Client.pinCode;
    cout << "\nName : " << Client.name;
    cout << "\nPhone : " << Client.phone;
    cout << "\nAccount Balance: " << Client.accountBalance;
    cout << "\n-----------------------------------\n";
}

bool findClientByAccountNumber(string accountNumber, vector<sClient>& vClients, sClient& client) {
    for (sClient& c : vClients) {
        if (c.accountNumber == accountNumber) {
            client = c;
            return true;
        }
    }
    return false;
}

sClient changeClientRecord(string accountNumber) {
    sClient client;
    client.accountNumber = accountNumber;
    cout << "\n\nEnter pin code?";
    getline(cin >> ws, client.pinCode);
    cout << "Enter name?";
    getline(cin, client.name);
    cout << "Enter phone number?";
    getline(cin, client.phone);
    cout << "\n\nEnter Account balance?";
    cin >> client.accountBalance;

    return client;
}

bool markClientForDeleteByAccountNumber(string accountNumber, vector<sClient>& vClients) {
    for (sClient& c : vClients) {
        if (c.accountNumber == accountNumber) {
            c.markForDelete = true;
            return true;
        }
    }
    return false;
}

vector <sClient> saveClientDataToFile(string fileName, vector<sClient>& vClients) {
    fstream myFile;
    myFile.open(fileName, ios::out);
    string dataLine;
    if (myFile.is_open()) {
        for (sClient& c : vClients) {
            dataLine = convertRecordToLine(c, "#//#");
            myFile << dataLine << endl;
        }
        myFile.close();
    }
    return vClients;
}

void addDataLineToFile(string fileName, string dataLine) {
    fstream myFile;
    myFile.open(fileName, ios::out | ios::app);
    if (myFile.is_open()) {
        myFile << dataLine << endl;
        myFile.close();
    }
}

void addNewClient() {
    sClient client;
    client = readNewClient();
    addDataLineToFile(clientFile, convertRecordToLine(client, "#//#"));
}

void addNewClients() {
    char addMore = 'y';
    do {
        cout << "\nAdding new client:\n";
        addNewClient();

        cout << "\nClient Added Successfully, do you want to add more clients ? Y / N ? \n";
        cin >> addMore;
    } while (tolower(addMore) == 'y');
}

bool deleteClientByAccountNumber(string accountNumber, vector<sClient>& vClients) {
    sClient client;
    char answer = 'n';
    if (findClientByAccountNumber(accountNumber, vClients, client)) {
        printClientCard(client);
        cout << "\n\nAre you sure you want delete this client? y/n? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            markClientForDeleteByAccountNumber(accountNumber, vClients);
            saveClientDataToFile(clientFile, vClients);
            vClients = loadClientsDataFromFile(clientFile);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
        else
        {
            cout << "\nClient with Account Number (" << accountNumber<< ") is Not Found!";
            return false;
        }
    }
}

bool updateClientDataByAccountNumber(string accountNumber, vector <sClient>& vClients) {
    sClient client;
    char answer = 'n';

    if (findClientByAccountNumber(accountNumber, vClients, client)) {
        printClientCard(client);
        cout << "\n\nAre you sure you want update this client? y/n? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            for (sClient& c : vClients) {
                if (c.accountNumber == accountNumber) {
                    c = changeClientRecord(accountNumber);
                    break;
                }
            }
            saveClientDataToFile(clientFile, vClients);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << accountNumber << ") is Not Found!";
        return false;
    }
}

string readClientAccountNumber() {
    string accountNumber = "";
    cout << "\nPlease enter a account number?";
    cin >> accountNumber;
    return accountNumber;
}

void showDeleteClientScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";
    vector <sClient> vClients = loadClientsDataFromFile(clientFile);
    string accountNumber = readClientAccountNumber();
    deleteClientByAccountNumber(accountNumber, vClients);
}

void showUpdateClientScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Screen";
    cout << "\n-----------------------------------\n";
    vector <sClient> vClients = loadClientsDataFromFile(clientFile);
    string accountNumber = readClientAccountNumber();
    updateClientDataByAccountNumber(accountNumber, vClients);
}

void showAddNewClientsScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    addNewClients();
}

void showFindClientScreen(){
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";
    vector <sClient> vClients = loadClientsDataFromFile(clientFile);
    sClient Client;
    string accountNumber = readClientAccountNumber();
    if (findClientByAccountNumber(accountNumber, vClients,Client))
        printClientCard(Client);
    else
        cout << "\nClient with Account Number[" << accountNumber << "] is not found!";
}
void showEndScreen(){
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

enum enMainMenueOptions{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eExit = 6
};

void GoBackToMainMenue(){
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    showMainMenue();
}
short readMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption){
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        showAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        showAddNewClientsScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eDeleteClient:
        system("cls");
        showDeleteClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eUpdateClient:
        system("cls");
        showUpdateClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eFindClient:
        system("cls");
        showFindClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eExit:
        system("cls");
        showEndScreen();
        break;
    }
}

void showMainMenue(){
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Exit.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)readMainMenueOption());
}
int main()
{
    showMainMenue();
    system("pause>0");
    return 0;
}