#include "QueueElement.h"

int QueueElement::getId() const {
    return id;
}

void QueueElement::setId(int id) {
    QueueElement::id = id;
}

int QueueElement::getTime() {
    return time;
}

void QueueElement::setTime(int time) {
    QueueElement::time = time;
}

int QueueElement::getWeight() const {
    return weight;
}

void QueueElement::setWeight(int weight) {
    QueueElement::weight = weight;
}

bool QueueElement::operator<(QueueElement &rhs) {
    if (time == rhs.time) {
        return id < rhs.id;
    } else {
        return time < rhs.time;
    }
}
