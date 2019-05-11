#include <sstream>
#include <iomanip>
#include "QueueElement.h"

int QueueElement::getId() const {
    return id;
}

int QueueElement::getWeight() const {
    return weight;
}

bool QueueElement::operator<(QueueElement &rhs) {
    if (time != rhs.time) {
        return time < rhs.time;
    }
    return id < rhs.id;

}

string QueueElement::toString() {
    std::stringstream ss;
    ss << left;
    ss << "(CLOCK:" << setw(3) << this->time;
    ss << "ID:" << setw(2) << this->id;
    ss << "WEIGHT:" << setw(4) << this->weight;
    ss << ")";
    return ss.str();
}
