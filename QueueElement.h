//
// Created by apilia on 5/8/2019.
//

#ifndef SKI_LIFT_QUEUEELEMENT_H
#define SKI_LIFT_QUEUEELEMENT_H


class QueueElement {
public:

    QueueElement(int e_id, int e_time, int e_weight) : id(e_id), time(e_time), weight(e_weight) {}

    int getId() const;

    void setId(int id);

    int getTime() const;

    void setTime(int time);

    int getWeight() const;

    void setWeight(int weight);

    bool operator<(const QueueElement &rhs) const;

private:
    int id;
    int time;
    int weight;

};


#endif //SKI_LIFT_QUEUEELEMENT_H
