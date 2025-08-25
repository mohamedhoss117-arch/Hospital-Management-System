#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department
{
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType
{
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== PATIENT CLASS ========== //
class Patient
{
private:
    int id;
    string name;
    int age;
    string contact;
    stack<string> medicalHistory;
    queue<string> testQueue;
    bool isAdmitted;
    RoomType roomType;

public:

    Patient(int pid, string n, int a, string c){
        id=pid;
        name =n;
        age=a;
        contact=c;
        isAdmitted=false
    }
    
    void admitPatient(RoomType type){
        if(isAdmitted){
            cout<<"Patient"<<name<<"is already admitted."<<endl;
            return;
        }else{
            isAdmitted=true;
            roomType=type;
            cout<<"Patient"<<name<<"admitted successfully!"<<endl;
            return;
        }
    }
    void dischargePatient(){
        if(isAdmitted){
            isAdmitted=false;
            cout<<"Patient"<<name<<"discharged."<<endl;
            return;
        }else{
            cout<<"Patient"<<name<<"is not admitted."<<endl;
            return;
        }
    }
    
    int getId(){
        return id;
    }
    string getName(){
        return name;
    }
    bool getAdmissionStatus(){
        return isAdmitted;
    }
    Patient(int pid, string n, int a, string c);

    void admitPatient(RoomType type);
    void dischargePatient();
    void addMedicalRecord(string record)
    {
        medicalHistory.push(record);
    }
    void requestTest(string testName)
    {
        testQueue.push(testName);
        medicalHistory.push("Requested test: " + testName);
    }
    string performTest()
    {
        if (testQueue.empty())
        {
            return "No tests pending";
        }
        string testName = testQueue.front();
        testQueue.pop();
        medicalHistory.push("Performed test: " + testName);
        return testName;
    }
    void displayHistory();

};

// ========== DOCTOR CLASS ========== //
class Doctor
{
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;

public:
    Doctor(int did, string n, Department d){
        id = did;
        name = n;
        department = d;
    }


    void addAppointment(int patientId){
        appointmentQueue.push(patientId);
    }
    int seePatient(){
        if (appointmentQueue.empty()) {
            cout<<"There are no patients registered"<<endl;
            return -1;
        }
        int pId=appointmentQueue.front();
        appointmentQueue.pop();
        return pId;
    }

    int getId(){
        return id;
    }
    string getName(){
        return name;
    }
    string getDepartment(){
        return to_string(department);
    }



};

// ========== HOSPITAL CLASS ========== //
class Hospital
{
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    queue<int> emergencyQueue;
    int patientCounter;
    int doctorCounter;

public:


    Hospital(){
        patientCounter=0;
        doctorCounter=0;
    }
    
    int registerPatient(string name, int age, string contact){
        patientCounter++;
        Patient newPatient(patientCounter,name,age,contact);
        patients.push_back(newPatient);
        return patientCounter;
    }
    int addDoctor(string name, Department dept){
        doctorCounter++;
        Doctor newDoc(doctorCounter,name,dept);
        doctors.push_back(newDoc);
        return doctorCounter;
    }
    void admitPatient(int patientId, RoomType type){
        for (auto &p : patients) {
            if (p.getId() == patientId) {
                p.admitPatient(type); 
                return;
            }
        }
        cout << "Patient with ID " << patientId << " not found!" << endl;
    }
  
    void addEmergency(int patientId)
    {
        emergencyQueue.push(patientId);
        cout << "Patient ID " << patientId << " added to emergency queue.\n";
    }
    int handleEmergency()
    {
        if (emergencyQueue.empty())
        {
            cout << "No emergency cases pending.\n";
            return -1;
        }
        int patientId = emergencyQueue.front();
        emergencyQueue.pop();
        cout << "Handling emergency for patient ID " << patientId << ".\n";
        return patientId;
    }
     void bookAppointment(int doctorId, int patientId){
            int doctorIndex = -1;
            int patientIndex = -1;
            for (int i=0; i<doctors.size(); i++){
                if (doctors[i].getId() == doctorId){
                    doctorIndex = i;
                    break;
                }
            }
            for (int i=0; i<patients.size(); i++){
                if (patients[i].getId() == patientId){
                    patientIndex = i;
                    break;
                }
            }
            if (doctorIndex == -1){
                cout<<"Doctor not found"<<endl;
                return;
            }
            if (patientIndex == -1){
                cout<<"Patient not found"<<endl;
                return;
            }
            doctors[doctorIndex].addAppointment(patientId);
            patients[patientIndex].addMedicalRecord("Booked appointment with Dr. " + doctors[doctorIndex].getName());
            cout<<"Appointment booked successfully!"<<endl;
    }
    void displayPatientInfo(int patientId){
        for (int i=0; i<patients.size(); i++){
            if (patients[i].getId() == patientId){
                cout<<"Patient ID: "<< patients[i].getId()<<endl;
                cout<<"Patient Name: "<< patients[i].getName()<<endl;
                cout<<"Admission Status: "<< (patients[i].getAdmissionStatus() ? "Admitted" : "Not Admitted")<< endl;
            }
            else{
                cout<<"Patient not found"<<endl;
            }
        }
    }
    void displayDoctorInfo(int doctorId){
        for (int i=0; i<doctors.size(); i++){
            if (doctors[i].getId() == doctorId){
                cout<<"Doctor ID: "<< doctors[i].getId()<<endl;
                cout<<"Doctor Name: "<< doctors[i].getName()<<endl;
                cout<<"Department: "<< doctors[i].getDepartment()<< endl;
            }
            else{
                cout<<"Doctor not found"<<endl;
            }
        }
    }
  
};

// ========== MAIN PROGRAM ========== //
int main()
{
    Hospital hospital;

    // Test Case 1: Registering patients
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");

    // Test Case 2: Adding doctors
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);

    // Test Case 3: Admitting patients
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    // Try admitting already admitted patient
    hospital.admitPatient(p1, SEMI_PRIVATE);

    // Test Case 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    // Try booking with invalid doctor/patient
    hospital.bookAppointment(999, p1); // Invalid doctor
    hospital.bookAppointment(d1, 999); // Invalid patient

    // Test Case 5: Handling medical tests
    // These would normally be called on Patient objects
    // In a real implementation, we'd need a way to access patients

    // Test Case 6: Emergency cases
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency(); // No more emergencies

    // Test Case 7: Discharging patients
    // Would normally call dischargePatient() on Patient objects

    // Test Case 8: Displaying information
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // Invalid patient

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // Invalid doctor

    // Test Case 9: Doctor seeing patients
    // These would normally be called on Doctor objects
    // In a real implementation, we'd need a way to access doctors

    // Test Case 10: Edge cases
    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1); // No patients
    emptyHospital.displayDoctorInfo(1);  // No doctors
    emptyHospital.handleEmergency();     // No emergencies

    return 0;
}
