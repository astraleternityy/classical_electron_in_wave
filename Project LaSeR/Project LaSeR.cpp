#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
using namespace std;

template<typename unpredictedType>
void printVector(vector<unpredictedType> localVec) {
    for (auto iter = localVec.begin(); iter < localVec.end(); iter++) {
        cout << *iter << " ";
    }
    cout << endl;
}
// Wave moves throughout x axis, E oscilates by z direction, B by y
double eFieldFunct(double x, double t, double* param) {
    double eField0 = param[0]; // Wave parametrs: E0
    double eFieldK = param[2]; // k=2pi/lambda
    double eFieldW = param[3]; // w = 2pi/T
    double eFieldP = param[4]; // phase0
    double functRes = eField0 * cos(eFieldK * x - eFieldW * t + eFieldP);
    return functRes;
}
double bFieldFunct(double x, double t, double* param) {
    double bField0 = param[1]; // Wave parametrs: B0
    double bFieldK = param[2]; // k=2pi/lambda
    double bFieldW = param[3]; // w = 2pi/T
    double bFieldP = param[4]; // phase0
    double functRes = bField0 * cos(bFieldK * x - bFieldW * t + bFieldP);
    return functRes;
}
vector<double> vectorMultiplic(vector<double> vec1, vector<double> vec2) {
    vector<double> newVec;
    newVec.push_back(vec1.at(1) * vec2.at(2) - vec1.at(2) * vec2.at(1));
    newVec.push_back(-(vec1.at(0) * vec2.at(2) - vec1.at(2) * vec2.at(0)));
    newVec.push_back(vec1.at(0) * vec2.at(1) - vec1.at(1) * vec2.at(0));
    return newVec;
}
double* inputParametrs() {
    double eField0 = 1; // Wave parametrs: E0
    double bField0 = 1; // Wave parametrs: B0
    double fieldK = 1; // k=2pi/lambda
    double fieldW = 1; // w = 2pi/T
    double fieldP = 1; // phase0
    cout << "Enter wave parametrs\n";
    cout << "E0 = "; cin >> eField0;
    cout << "B0 = "; cin >> bField0;
    cout << "k = 2*pi/lambda = "; cin >> fieldK;
    cout << "w = 2*pi/T = "; cin >> fieldW;
    cout << "phase0 = "; cin >> fieldP;
    double* parametrs = new double[5];
    parametrs[0] = eField0;
    parametrs[1] = bField0;
    parametrs[2] = fieldK;
    parametrs[3] = fieldW;
    parametrs[4] = fieldP;
    return parametrs;
}
int main() {
    ofstream outFile("C:/Main/ProjectLaSeR/PhysicalData/electronSiReT2.csv");
    outFile << "t,x,y,z,vx,vy,vz\n";
    //outFile << "Hello from C++!";
    setlocale(LC_ALL, "");
    bool correctInput = false;
    // Input of wave parametrs
    // Indicators 0 -> E0, 1 -> B0, 2 -> k, 3 -> w, 4 -> p
    double* waveParametrs = new double[5];
    while (!correctInput) {
        waveParametrs = inputParametrs();
        cout << "Your input: E0 = " << waveParametrs[0] << ", B0 = " << waveParametrs[1] << ", k = " << waveParametrs[2]
             << ", w = " << waveParametrs[3] << ", p = " << waveParametrs[4] << "\nCorrect? (true/false): ";
        string keyboardData;
        cin >> keyboardData;
        if (keyboardData == "true") correctInput = true;
        else correctInput = false;
    }
    // Constants
    const double eCharge = -1.60218e-19;
    const double eMass = 9.10938e-31;
    const double pi = 3.14159;
    double time = 0;
    double simTime = 0;
    vector<double> ePosition = {0, 0, 0}; // 0:X 1:Y 2:Z
    vector<double> eVelocity = {0, 0, 0};
    vector<double> eAccelern = {0, 0, 0};
    double delta;
    cout << "Delta (in seconds): ";
    cin >> delta;
    cout << "Time of simulation (in seconds): ";
    cin >> simTime;

    // Calculations
    for (double i = 0; i < simTime / delta; i++) {
        double time = delta * i;
        vector<double> eFieldForce = { 0, 0, eFieldFunct(ePosition.at(0), time, waveParametrs) };
        vector<double> bFieldForce = { 0, bFieldFunct(ePosition.at(0), time, waveParametrs), 0 };
        // Acceleration
        vector<double> tempEF = eFieldForce;
        vector<double> vectorMultRes = vectorMultiplic(eVelocity, bFieldForce);
        eAccelern.at(0) = (eCharge * (tempEF.at(0) + vectorMultRes.at(0))) / eMass;
        eAccelern.at(1) = (eCharge * (tempEF.at(1) + vectorMultRes.at(1))) / eMass;
        eAccelern.at(2) = (eCharge * (tempEF.at(2) + vectorMultRes.at(2))) / eMass;

        // Velocity
        eVelocity.at(0) = eVelocity.at(0) + delta * eAccelern.at(0);
        eVelocity.at(1) = eVelocity.at(1) + delta * eAccelern.at(1);
        eVelocity.at(2) = eVelocity.at(2) + delta * eAccelern.at(2);

        // Position
        ePosition.at(0) = ePosition.at(0) + delta * eVelocity.at(0);
        ePosition.at(1) = ePosition.at(1) + delta * eVelocity.at(1);
        ePosition.at(2) = ePosition.at(2) + delta * eVelocity.at(2);
        
        outFile << time << ","
            << ePosition.at(0) << "," << ePosition.at(1) << "," << ePosition.at(2) << ","
            << eVelocity.at(0) << "," << eVelocity.at(1) << "," << eVelocity.at(2) << "\n";
    }
    outFile.close();
    return 0;
}
