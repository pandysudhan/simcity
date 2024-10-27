#pragma once
#ifndef WORKER_ENTITY_H
#define WORKER_ENTITY_H

class WorkerEntity {
private:
    bool available;
public:
    WorkerEntity() : available(true) {}

    bool assign() {
        if (!available) return false;
        available = false;
        return true;
    }

    bool isAvailable() const { return available; }
};

#endif
