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

static string departmentToString(Department d) {
    switch (d) {
        case CARDIOLOGY:  return "Cardiology";
        case NEUROLOGY:   return "Neurology";
        case ORTHOPEDICS: return "Orthopedics";
        case PEDIATRICS:  return "Pediatrics";
        case EMERGENCY:   return "Emergency";
        case GENERAL:     return "General";
        default:          return "Unknown";
    }
}

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
    Patient(int pid, string n, int a, string c) {
        id = pid;
        name = n;
        age = a;
        contact = c;
        isAdmitted = false;
    }

    void admitPatient(RoomType type) {
        if (isAdmitted) {
            cout << "Patient " << name << " is already admitted." << endl;
            return;
        }
        isAdmitted = true;
        roomType = type;
        medicalHistory.push("Admitted to room type " + to_string(static_cast<int>(type)));
        cout << "Patient " << name << " admitted successfully!" << endl;
    }

    void dischargePatient() {
        if (!isAdmitted) {
            cout << "Patient " << name << " is not admitted." << endl;
            return;
        }
        isAdmitted = false;
        medicalHistory.push("Discharged");
        cout << "Patient " << name << " discharged." << endl;
    }

    void addMedicalRecord(string record) {
        medicalHistory.push(record);
    }

    void requestTest(string testName) {
        testQueue.push(testName);
        medicalHistory.push("Requested test: " + testName);
    }

    string performTest() {
        if (testQueue.empty()) {
            return "No tests pending";
        }
        string testName = testQueue.front();
        testQueue.pop();
        medicalHistory.push("Performed test: " + testName);
        return testName;
    }

    void displayHistory() {
        if (medicalHistory.empty()) {
            cout << "No medical history for patient " << name << "." << endl;
            return;
        }
        stack<string> temp = medicalHistory;
        cout << "Medical history for " << name << ":\n";
        while (!temp.empty()) {
            cout << "- " << temp.top() << endl;
            temp.pop();
        }
    }

    int getId() { return id; }
    string getName() { return name; }
    bool getAdmissionStatus() { return isAdmitted; }
};

// ========== DOCTOR CLASS ========== //
class Doctor {
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;

public:
    Doctor(int did, string n, Department d) {
        id = did;
        name = n;
        department = d;
    }

    void addAppointment(int patientId) {
        appointmentQueue.push(patientId);
    }

    int seePatient() {
        if (appointmentQueue.empty()) {
            cout << "No appointments in the queue." << endl;
            return -1;
        }
        int pId = appointmentQueue.front();
        appointmentQueue.pop();
        return pId;
    }

    int getId() { return id; }
    string getName() { return name; }
    string getDepartment() { return departmentToString(department); }
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
    Hospital() {
        patientCounter = 0;
        doctorCounter = 0;
    }

    int registerPatient(string name, int age, string contact) {
        patientCounter++;
        Patient newPatient(patientCounter, name, age, contact);
        patients.push_back(newPatient);
        return patientCounter;
    }

    int addDoctor(string name, Department dept) {
        doctorCounter++;
        Doctor newDoc(doctorCounter, name, dept);
        doctors.push_back(newDoc);
        return doctorCounter;
    }

    void admitPatient(int patientId, RoomType type) {
        for (auto &p : patients) {
            if (p.getId() == patientId) {
                p.admitPatient(type);
                return;
            }
        }
        cout << "Patient with ID " << patientId << " not found!" << endl;
    }

    void addEmergency(int patientId) {
        emergencyQueue.push(patientId);
        cout << "Patient ID " << patientId << " added to emergency queue." << endl;
    }

    int handleEmergency() {
        if (emergencyQueue.empty()) {
            cout << "No emergency cases pending." << endl;
            return -1;
        }
        int patientId = emergencyQueue.front();
        emergencyQueue.pop();
        cout << "Handling emergency for patient ID " << patientId << "." << endl;
        return patientId;
    }

    void bookAppointment(int doctorId, int patientId) {
        int doctorIndex = -1;
        int patientIndex = -1;

        for (int i = 0; i < (int)doctors.size(); i++) {
            if (doctors[i].getId() == doctorId) {
                doctorIndex = i;
                break;
            }
        }
        for (int i = 0; i < (int)patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                patientIndex = i;
                break;
            }
        }

        if (doctorIndex == -1) {
            cout << "Doctor not found" << endl;
            return;
        }
        if (patientIndex == -1) {
            cout << "Patient not found" << endl;
            return;
        }

        doctors[doctorIndex].addAppointment(patientId);
        patients[patientIndex].addMedicalRecord("Booked appointment with Dr. " + doctors[doctorIndex].getName());
        cout << "Appointment booked successfully!" << endl;
    }

    void displayPatientInfo(int patientId) {
        for (int i = 0; i < (int)patients.size(); i++) {
            if (patients[i].getId() == patientId) {
                cout << "Patient ID: " << patients[i].getId() << endl;
                cout << "Patient Name: " << patients[i].getName() << endl;
                cout << "Admission Status: " << (patients[i].getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
                patients[i].displayHistory();
                return;
            }
        }
        cout << "Patient not found" << endl;
    }

    void displayDoctorInfo(int doctorId) {
        for (int i = 0; i < (int)doctors.size(); i++) {
            if (doctors[i].getId() == doctorId) {
                cout << "Doctor ID: " << doctors[i].getId() << endl;
                cout << "Doctor Name: " << doctors[i].getName() << endl;
                cout << "Department: " << doctors[i].getDepartment() << endl;
                return;
            }
        }
        cout << "Doctor not found" << endl;
    }
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
    hospital.admitPatient(p1, SEMI_PRIVATE); // already admitted

    // Test Case 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    hospital.bookAppointment(999, p1); // invalid doctor
    hospital.bookAppointment(d1, 999); // invalid patient

    // Test Case 5: Emergency cases
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    hospital.handleEmergency();
    hospital.handleEmergency();
    hospital.handleEmergency(); // no emergencies

    // Test Case 6: Displaying information
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // invalid patient

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // invalid doctor

    return 0;
}
