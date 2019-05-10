#ifndef SKI_LIFT_QUEUEELEMENT_H
#define SKI_LIFT_QUEUEELEMENT_H


class QueueElement {
public:

    QueueElement(int e_id, int e_time, int e_weight) : id(e_id), time(e_time), weight(e_weight) {}

    int getId();

    void setId(int id);

    int getTime();

    void setTime(int time);

    int getWeight();

    void setWeight(int weight);

    bool operator<(QueueElement &rhs);

private:
    int id;
    int time;
    int weight;

};


#endif //SKI_LIFT_QUEUEELEMENT_H
