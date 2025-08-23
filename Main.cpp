#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department {
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType {
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== PATIENT CLASS ========== //
class Patient {
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
    void addMedicalRecord(string record);
    void requestTest(string testName);
    string performTest();
    void displayHistory();
    
    int getId(){
        return id;
    }
    string getName(){
        return name;
    }
    bool getAdmissionStatus(){
        return isAdmitted;
    }
};

// ========== DOCTOR CLASS ========== //
class Doctor {
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;
    
public:
    Doctor(int did, string n, Department d){
        id=did;
        name=n;
        department=d;
    }
    
    void addAppointment(int patientId);
    int seePatient();
    
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
class Hospital {
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
    void addEmergency(int patientId);
    int handleEmergency();
    void bookAppointment(int doctorId, int patientId);
    void displayPatientInfo(int patientId);
    void displayDoctorInfo(int doctorId);
};

// ========== MAIN PROGRAM ========== //
int main() {
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