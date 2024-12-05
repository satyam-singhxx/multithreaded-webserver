#include "../include/observer.h"
#include <algorithm>

void Subject::addObserver(std::shared_ptr<Observer> observer) {
    // Check if observer already exists to prevent duplicates
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it == observers.end()) {
        observers.push_back(observer);
    }
}

void Subject::removeObserver(std::shared_ptr<Observer> observer) {
    // Remove specific observer
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

void Subject::notifyObservers(int clientSocket) {
    // Notify all registered observers about the new client connection
    for (auto& observer : observers) {
        observer->update(clientSocket);
    }
}