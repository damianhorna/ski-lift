#ifndef SKI_LIFT_QUEUEELEMENT_H
#define SKI_LIFT_QUEUEELEMENT_H

#include <string>

using namespace std;

class QueueElement {
public:

    QueueElement(int e_id, int e_time, int e_weight) : id(e_id), time(e_time), weight(e_weight) {}

    int getId() const;

    int getWeight() const;

    bool operator<(QueueElement &rhs);

    string toString();

private:
    int id;
    int time;
    int weight;
};


#endif //SKI_LIFT_QUEUEELEMENT_H
