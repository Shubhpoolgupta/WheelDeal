# 🚗 WheelDeal - Vehicle Rental System (LLD)

## Overview

WheelDeal is a Low-Level Design implementation of a Vehicle Rental System developed in C++. The system simulates the complete workflow of a real-world vehicle rental platform including vehicle management, reservations, payments, stores, and users.

The project demonstrates Object-Oriented Design principles, SOLID principles, and multiple design patterns.

---

# Functional Requirements

The WheelDeal Vehicle Rental System is designed to satisfy the following requirements:

✅ The system should manage multiple vehicles across different rental locations.
✅ Users should be able to search, filter, and reserve vehicles based on their preferences.
✅ The system should track vehicle availability and prevent booking conflicts.
✅ Billing and payment processing should be integrated for a complete rental cycle.
✅ The system should be scalable to handle operations across multiple cities.


## Features

* Multiple rental stores
* Vehicle inventory management
* Vehicle availability tracking
* User registration
* Reservation management
* Rental lifecycle management
* Multiple payment methods
* Dynamic rental fee calculation
* Singleton system controller

---

## Tech Stack

* Language: C++
* STL:

  * vector
  * unordered_map
  * memory
  * algorithm
  * time library

---

## System Components

### Vehicle

Abstract base class for all vehicles.

Derived classes:

* EconomyVehicle
* LuxuryVehicle
* SUVVehicle

---

### VehicleFactory

Responsible for creating vehicle objects.

Supported types:

* Economy
* Luxury
* SUV

---

### RentalStore

Manages:

* Vehicle inventory
* Vehicle availability
* Store information

---

### User

Stores:

* User details
* Reservation history

---

### Reservation

Handles:

* Booking details
* Rental dates
* Vehicle assignment
* Reservation status
* Rental amount calculation

---

### ReservationManager

Responsible for:

* Creating reservations
* Managing reservation lifecycle
* Searching reservations

---

### Payment Module

Supports:

* Credit Card Payment
* Cash Payment
* PayPal Payment

---

### RentalSystem

Central controller of the application.

Responsibilities:

* User registration
* Store management
* Reservation creation
* Payment processing
* Rental operations

Implemented as a Singleton.

---

## Design Patterns Used

### Singleton Pattern

* RentalSystem

### Factory Pattern

* VehicleFactory

### Strategy Pattern

* PaymentStrategy

### Polymorphism

* Vehicle hierarchy
* Payment hierarchy

### Composition

* Reservation contains Vehicle and User.
* RentalStore contains Location.

---

## Class Diagram (High Level)

RentalSystem
│
├── RentalStore
│   └── Vehicle
│       ├── EconomyVehicle
│       ├── LuxuryVehicle
│       └── SUVVehicle
│
├── User
│   └── Reservation
│
├── ReservationManager
│
└── PaymentProcessor
└── PaymentStrategy
├── CashPayment
├── CreditCardPayment
└── PaypalPayment

---

## Sample Workflow

1. Create stores.
2. Add vehicles to stores.
3. Register users.
4. Create reservation.
5. Select payment method.
6. Process payment.
7. Confirm reservation.
8. Start rental.
9. Complete rental.

---

## Future Improvements

* Observer Pattern for notifications.
* State Pattern for reservation states.
* Pricing strategies.

---

## Learning Outcomes

* Object-Oriented Design
* SOLID Principles
* Design Patterns
* Smart Pointers
* STL Containers
* System Design Concepts

---

## Author

Developed as a Low-Level Design project to demonstrate software design principles and object-oriented architecture.
