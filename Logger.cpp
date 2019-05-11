#include <sstream>
#include <iomanip>
#include "Logger.h"
#include "easylogging++.h"

const string Logger::main_thread = "[MAI]";
const string Logger::rec = "[REC]";
const string Logger::ack = "[ACK]";
const string Logger::rel = "[REL]";
const string Logger::req = "[REQ]";

void Logger::afterReceiveAck(ThreadManagerBase threadManager, int sourceId) {
    std::stringstream s1, s2, s3;
    s1 << rec << ack << threadManager.toString() << "Ack was received from [" << sourceId
       << "] and ACK Table was updated" << endl;
    s2 << rec << ack << threadManager.toString() << left << setw(4) << "ID:";
    s3 << rec << ack << threadManager.toString() << left << setw(4) << "TAB:";

    for (int i = 0; i < threadManager.getSize(); i++) {
        s2 << setw(2) << i;
        s3 << setw(2) << threadManager.getTabAcks().at(i);
    }

    s2 << endl;
    LOG(DEBUG) << s1.str() << s2.str() << s3.str();
}

void Logger::queueUpdate(ThreadManagerBase threadManager) {
    std::stringstream s1, s2;
    s1 << rec << req << "Request was received and queue was updated";
    s2 << rec << req << "Queue" << endl;
    for (QueueElement &elem : threadManager.getQueue()) {
        s2 << elem.toString() << ",";
    }
    LOG(DEBUG) << s1.str();
    LOG(DEBUG) << s2.str();

}
