// =====================================================================
// Vehicle Rental System - Single File LLD Implementation
// =====================================================================
#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <ctime>
#include <algorithm>
#include <stdexcept>

using namespace std;

// =====================================================================
// ENUMS
// =====================================================================

enum class VehicleType {
    ECONOMY,
    COMPACT,
    SEDAN,
    SUV,
    LUXURY,
    VAN,
    TRUCK
};

enum class VehicleStatus {
    AVAILABLE,
    RESERVED,
    RENTED,
    MAINTENANCE,
    OUT_OF_SERVICE
};

enum class ReservationStatus {
    PENDING,
    CONFIRMED,
    IN_PROGRESS,
    COMPLETED,
    CANCELED
};

string vehicleTypeToString(VehicleType type) {
    switch (type) {
        case VehicleType::ECONOMY: return "ECONOMY";
        case VehicleType::COMPACT: return "COMPACT";
        case VehicleType::SEDAN:   return "SEDAN";
        case VehicleType::SUV:     return "SUV";
        case VehicleType::LUXURY:  return "LUXURY";
        case VehicleType::VAN:     return "VAN";
        case VehicleType::TRUCK:   return "TRUCK";
        default: return "UNKNOWN";
    }
}

// =====================================================================
// VEHICLE (Factory Pattern - Product Hierarchy)
// =====================================================================

class Vehicle {
protected:
    string registrationNumber;
    string model;
    VehicleType type;
    VehicleStatus status;
    double baseRentalPrice;

public:
    Vehicle(string registrationNumber, string model, VehicleType type, double baseRentalPrice) {
        this->registrationNumber = registrationNumber;
        this->model = model;
        this->type = type;
        this->status = VehicleStatus::AVAILABLE;
        this->baseRentalPrice = baseRentalPrice;
    }

    virtual double calculateRentalFee(int days) = 0;

    string getRegistrationNumber() { return registrationNumber; }
    string getModel() { return model; }
    VehicleType getType() { return type; }
    VehicleStatus getStatus() { return status; }
    void setStatus(VehicleStatus status) { this->status = status; }
    double getBaseRentalPrice() { return baseRentalPrice; }

    virtual ~Vehicle() {}
};

class EconomyVehicle : public Vehicle {
    static constexpr double RATE_MULTIPLIER = 1.0;

public:
    EconomyVehicle(string registrationNumber, string model, VehicleType type, double baseRentalPrice)
        : Vehicle(registrationNumber, model, type, baseRentalPrice) {}

    double calculateRentalFee(int days) override {
        return getBaseRentalPrice() * days * RATE_MULTIPLIER;
    }
};

class LuxuryVehicle : public Vehicle {
    static constexpr double RATE_MULTIPLIER = 1.5;
    static constexpr double PREMIUM_FEE = 50.0;

public:
    LuxuryVehicle(string registrationNumber, string model, VehicleType type, double baseRentalPrice)
        : Vehicle(registrationNumber, model, type, baseRentalPrice) {}

    double calculateRentalFee(int days) override {
        return (getBaseRentalPrice() * days * RATE_MULTIPLIER) + PREMIUM_FEE;
    }
};

class SUVVehicle : public Vehicle {
    static constexpr double RATE_MULTIPLIER = 1.2;

public:
    SUVVehicle(string registrationNumber, string model, VehicleType type, double baseRentalPrice)
        : Vehicle(registrationNumber, model, type, baseRentalPrice) {}

    double calculateRentalFee(int days) override {
        return getBaseRentalPrice() * days * RATE_MULTIPLIER;
    }
};

class VehicleFactory {
public:
    static Vehicle* createVehicle(
        VehicleType vehicleType,
        string registrationNumber,
        string model,
        double baseRentalPrice
    ) {
        switch (vehicleType) {
            case VehicleType::ECONOMY:
                return new EconomyVehicle(registrationNumber, model, vehicleType, baseRentalPrice);
            case VehicleType::LUXURY:
                return new LuxuryVehicle(registrationNumber, model, vehicleType, baseRentalPrice);
            case VehicleType::SUV:
                return new SUVVehicle(registrationNumber, model, vehicleType, baseRentalPrice);
            default:
                throw invalid_argument("Unsupported vehicle type");
        }
    }
};

// =====================================================================
// LOCATION
// =====================================================================

class Location {
    string address;
    string city;
    string state;
    string zipCode;

public:
    Location(string address, string city, string state, string zipCode) {
        this->address = address;
        this->city = city;
        this->state = state;
        this->zipCode = zipCode;
    }

    string getAddress() { return address; }
    string getCity() { return city; }
    string getState() { return state; }
    string getZipCode() { return zipCode; }
};

// =====================================================================
// RENTAL STORE
// =====================================================================

class RentalStore {
    int id;
    string name;
    Location location;
    unordered_map<string, shared_ptr<Vehicle>> vehicles;

public:
    RentalStore(int id, string name, Location location) : location(location) {
        this->id = id;
        this->name = name;
    }

    vector<shared_ptr<Vehicle>> getAvailableVehicles() {
        vector<shared_ptr<Vehicle>> availableVehicles;
        for (auto& entry : vehicles) {
            if (entry.second->getStatus() == VehicleStatus::AVAILABLE) {
                availableVehicles.push_back(entry.second);
            }
        }
        return availableVehicles;
    }

    void addVehicle(shared_ptr<Vehicle> vehicle) {
        vehicles[vehicle->getRegistrationNumber()] = vehicle;
    }

    void removeVehicle(string registrationNumber) {
        vehicles.erase(registrationNumber);
    }

    bool isVehicleAvailable(string registrationNumber) {
        auto it = vehicles.find(registrationNumber);
        if (it == vehicles.end()) return false;
        return it->second->getStatus() == VehicleStatus::AVAILABLE;
    }

    shared_ptr<Vehicle> getVehicle(string registrationNumber) {
        auto it = vehicles.find(registrationNumber);
        if (it != vehicles.end()) return it->second;
        return nullptr;
    }

    unordered_map<string, shared_ptr<Vehicle>> getAllVehicles() {
        return vehicles;
    }

    int getId() { return id; }
};

// Forward declaration needed for User -> Reservation relationship
class Reservation;

// =====================================================================
// USER
// =====================================================================

class User {
    int id;
    string name;
    string email;
    vector<shared_ptr<Reservation>> reservations;

public:
    User(int id, string name, string email) {
        this->id = id;
        this->name = name;
        this->email = email;
    }

    void addReservation(shared_ptr<Reservation> reservation) {
        reservations.push_back(reservation);
    }

    void deleteReservation(shared_ptr<Reservation> reservation) {
        reservations.erase(
            remove(reservations.begin(), reservations.end(), reservation),
            reservations.end()
        );
    }

    int getId() { return id; }
    string getName() { return name; }
    string getEmail() { return email; }
    vector<shared_ptr<Reservation>> getReservations() { return reservations; }
};

// =====================================================================
// RESERVATION
// =====================================================================

class Reservation {
    int id;
    shared_ptr<User> user;
    shared_ptr<Vehicle> vehicle;
    shared_ptr<RentalStore> pickupStore;
    shared_ptr<RentalStore> returnStore;
    time_t startDate;
    time_t endDate;
    ReservationStatus status;
    double totalAmount;

public:
    Reservation(
        int id,
        shared_ptr<User> user,
        shared_ptr<Vehicle> vehicle,
        shared_ptr<RentalStore> pickupStore,
        shared_ptr<RentalStore> returnStore,
        time_t startDate,
        time_t endDate
    ) {
        this->id = id;
        this->user = user;
        this->vehicle = vehicle;
        this->pickupStore = pickupStore;
        this->returnStore = returnStore;
        this->startDate = startDate;
        this->endDate = endDate;
        this->status = ReservationStatus::PENDING;

        double seconds = difftime(endDate, startDate);
        int days = static_cast<int>(seconds / (60 * 60 * 24)) + 1;

        this->totalAmount = vehicle->calculateRentalFee(days);
    }

    void confirmReservation() {
        if (status == ReservationStatus::PENDING) {
            status = ReservationStatus::CONFIRMED;
            vehicle->setStatus(VehicleStatus::RESERVED);
        }
    }

    void startRental() {
        if (status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::IN_PROGRESS;
            vehicle->setStatus(VehicleStatus::RENTED);
        }
    }

    void completeRental() {
        if (status == ReservationStatus::IN_PROGRESS) {
            status = ReservationStatus::COMPLETED;
            vehicle->setStatus(VehicleStatus::AVAILABLE);
        }
    }

    void cancelReservation() {
        if (status == ReservationStatus::PENDING || status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::CANCELED;
            vehicle->setStatus(VehicleStatus::AVAILABLE);
        }
    }

    int getId() { return id; }
    double getTotalAmount() { return totalAmount; }
};

// =====================================================================
// RESERVATION MANAGER
// =====================================================================

class ReservationManager {
    unordered_map<int, shared_ptr<Reservation>> reservations;
    int nextReservationId;

public:
    ReservationManager() {
        nextReservationId = 1;
    }

    shared_ptr<Reservation> createReservation(
        shared_ptr<User> user,
        shared_ptr<Vehicle> vehicle,
        shared_ptr<RentalStore> pickupStore,
        shared_ptr<RentalStore> returnStore,
        time_t startDate,
        time_t endDate
    ) {
        auto reservation = make_shared<Reservation>(
            nextReservationId++,
            user,
            vehicle,
            pickupStore,
            returnStore,
            startDate,
            endDate
        );

        reservations[reservation->getId()] = reservation;
        user->addReservation(reservation);

        return reservation;
    }

    void confirmReservation(int reservationId) {
        auto it = reservations.find(reservationId);
        if (it != reservations.end()) it->second->confirmReservation();
    }

    void startRental(int reservationId) {
        auto it = reservations.find(reservationId);
        if (it != reservations.end()) it->second->startRental();
    }

    void completeRental(int reservationId) {
        auto it = reservations.find(reservationId);
        if (it != reservations.end()) it->second->completeRental();
    }

    void cancelReservation(int reservationId) {
        auto it = reservations.find(reservationId);
        if (it != reservations.end()) it->second->cancelReservation();
    }

    shared_ptr<Reservation> getReservation(int reservationId) {
        auto it = reservations.find(reservationId);
        if (it != reservations.end()) return it->second;
        return nullptr;
    }
};

// =====================================================================
// PAYMENT STRATEGY PATTERN
// =====================================================================

class PaymentStrategy {
public:
    virtual void processPayment(double amount) = 0;
    virtual ~PaymentStrategy() {}
};

class CashPayment : public PaymentStrategy {
public:
    void processPayment(double amount) override {
        cout << "Processing cash payment of $" << amount << endl;
    }
};

class CreditCardPayment : public PaymentStrategy {
public:
    void processPayment(double amount) override {
        cout << "Processing credit card payment of $" << amount << endl;
    }
};

class PaypalPayment : public PaymentStrategy {
public:
    void processPayment(double amount) override {
        cout << "Processing PayPal payment of $" << amount << endl;
    }
};

class PaymentProcessor {
public:
    bool processPayment(double amount, PaymentStrategy* paymentStrategy) {
        if (paymentStrategy) {
            paymentStrategy->processPayment(amount);
            return true; // Assume payment is successful for simplicity
        }
        return false;
    }
};

// =====================================================================
// RENTAL SYSTEM (Singleton)
// =====================================================================

class RentalSystem {
    static RentalSystem* instance;

    vector<shared_ptr<RentalStore>> stores;
    unordered_map<int, shared_ptr<User>> users;
    ReservationManager reservationManager;
    PaymentProcessor paymentProcessor;
    int nextUserId;

    RentalSystem() {
        nextUserId = 1;
    }

public:
    // Disable copy/assignment to enforce singleton semantics
    RentalSystem(const RentalSystem&) = delete;
    RentalSystem& operator=(const RentalSystem&) = delete;

    static RentalSystem& getInstance() {
        if (instance == nullptr) {
            instance = new RentalSystem();
        }
        return *instance;
    }

    void addStore(shared_ptr<RentalStore> store) {
        stores.push_back(store);
    }

    shared_ptr<RentalStore> getStore(int storeId) {
        for (auto& store : stores) {
            if (store->getId() == storeId) return store;
        }
        return nullptr;
    }

    vector<shared_ptr<RentalStore>> getStores() { return stores; }

    shared_ptr<User> getUser(int userId) {
        if (users.find(userId) != users.end()) return users[userId];
        return nullptr;
    }

    shared_ptr<Reservation> createReservation(
        int userId,
        string vehicleRegistration,
        int pickupStoreId,
        int returnStoreId,
        time_t startDate,
        time_t endDate
    ) {
        auto user = getUser(userId);
        auto pickupStore = getStore(pickupStoreId);
        auto returnStore = getStore(returnStoreId);

        shared_ptr<Vehicle> vehicle = nullptr;
        if (pickupStore != nullptr) {
            vehicle = pickupStore->getVehicle(vehicleRegistration);
        }

        if (user != nullptr && pickupStore != nullptr && returnStore != nullptr && vehicle != nullptr) {
            return reservationManager.createReservation(
                user, vehicle, pickupStore, returnStore, startDate, endDate
            );
        }

        return nullptr;
    }

    bool processPayment(int reservationId, PaymentStrategy* paymentStrategy) {
        auto reservation = reservationManager.getReservation(reservationId);

        if (reservation != nullptr) {
            bool result = paymentProcessor.processPayment(reservation->getTotalAmount(), paymentStrategy);
            if (result) {
                reservationManager.confirmReservation(reservationId);
                return true;
            }
        }

        return false;
    }

    void startRental(int reservationId) {
        reservationManager.startRental(reservationId);
    }

    void completeRental(int reservationId) {
        reservationManager.completeRental(reservationId);
    }

    void cancelReservation(int reservationId) {
        reservationManager.cancelReservation(reservationId);
    }

    void registerUser(shared_ptr<User> user) {
        int userId = user->getId();
        if (users.find(userId) != users.end()) {
            cout << "User with id : " << userId << " Already exists in the system" << endl;
            return;
        }
        users[userId] = user;
    }
};

RentalSystem* RentalSystem::instance = nullptr;

// =====================================================================
// MAIN
// =====================================================================

int main() {

    // Singleton Rental System
    RentalSystem& rentalSystem = RentalSystem::getInstance();

    // Create Stores
    auto store1 = make_shared<RentalStore>(
        1, "Downtown Rentals",
        Location("123 Main St", "New York", "NY", "10001")
    );

    auto store2 = make_shared<RentalStore>(
        2, "Airport Rentals",
        Location("456 Airport Rd", "Los Angeles", "CA", "90045")
    );

    rentalSystem.addStore(store1);
    rentalSystem.addStore(store2);

    // Create Vehicles
    shared_ptr<Vehicle> economyCar(
        VehicleFactory::createVehicle(VehicleType::ECONOMY, "EC001", "Toyota", 50.0)
    );

    shared_ptr<Vehicle> luxuryCar(
        VehicleFactory::createVehicle(VehicleType::LUXURY, "LX001", "Mercedes", 200.0)
    );

    shared_ptr<Vehicle> suvCar(
        VehicleFactory::createVehicle(VehicleType::SUV, "SV001", "Honda", 75.0)
    );

    // Add vehicles to stores
    store1->addVehicle(economyCar);
    store1->addVehicle(luxuryCar);
    store2->addVehicle(suvCar);

    // Display Vehicles
    unordered_map<string, shared_ptr<Vehicle>> vehicles = store1->getAllVehicles();

    for (const auto& entry : vehicles) {
        cout << "Vehicle Number : " << entry.second->getRegistrationNumber()
             << " Vehicle Type : " << vehicleTypeToString(entry.second->getType())
             << endl;
    }

    // Register Users
    auto user1 = make_shared<User>(123, "ABC", "abc@gmail.com");
    auto user2 = make_shared<User>(345, "BCD", "bcd@yahoo.com");

    rentalSystem.registerUser(user1);
    rentalSystem.registerUser(user2);

    // Reservation Dates
    tm startDate = {};
    startDate.tm_year = 2025 - 1900;
    startDate.tm_mon = 4;
    startDate.tm_mday = 1;

    tm endDate = {};
    endDate.tm_year = 2025 - 1900;
    endDate.tm_mon = 5;
    endDate.tm_mday = 15;

    time_t pickupDate = mktime(&startDate);
    time_t returnDate = mktime(&endDate);

    // Create Reservation
    shared_ptr<Reservation> reservation1 = rentalSystem.createReservation(
        user1->getId(),
        economyCar->getRegistrationNumber(),
        store1->getId(),
        store1->getId(),
        pickupDate,
        returnDate
    );

    if (reservation1 == nullptr) {
        cout << "Reservation creation failed." << endl;
        return 0;
    }

    // Payment
    cout << "\nProcessing payment for reservation #" << reservation1->getId() << endl;
    cout << "Total amount: $" << reservation1->getTotalAmount() << endl;

    cout << "Select payment method:\n";
    cout << "1. Credit Card\n";
    cout << "2. Cash\n";
    cout << "3. PayPal\n";

    int choice = 1;
    // cin >> choice;

    unique_ptr<PaymentStrategy> paymentStrategy;

    switch (choice) {
        case 1:
            paymentStrategy = make_unique<CreditCardPayment>();
            break;
        case 2:
            paymentStrategy = make_unique<CashPayment>();
            break;
        case 3:
            paymentStrategy = make_unique<PaypalPayment>();
            break;
        default:
            cout << "Invalid choice." << endl;
            paymentStrategy = make_unique<CreditCardPayment>();
    }

    bool paymentSuccess = rentalSystem.processPayment(reservation1->getId(), paymentStrategy.get());

    if (paymentSuccess) {
        cout << "\nPayment Successful!" << endl;

        rentalSystem.startRental(reservation1->getId());
        cout << "\nRental Started..." << endl;

        rentalSystem.completeRental(reservation1->getId());
        cout << "Rental Completed." << endl;
    } else {
        cout << "Payment Failed." << endl;
    }

    return 0;
}
