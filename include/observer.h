#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <memory>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(int clientSocket) = 0;
};

class Subject {
public:
    void addObserver(std::shared_ptr<Observer> observer);
    void removeObserver(std::shared_ptr<Observer> observer);
    void notifyObservers(int clientSocket);

private:
    std::vector<std::shared_ptr<Observer>> observers;
};

#endif // OBSERVER_H