//
// Created by zyzshishui on 23-7-22.
//

#ifndef IDS_IDGENERATOR_H
#define IDS_IDGENERATOR_H

class IdGenerator {
private:
    long id = 0L;

public:
    IdGenerator();
    IdGenerator(long id);
    long nextId();


};

#endif //IDS_IDGENERATOR_H
