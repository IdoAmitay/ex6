//
// Created by ido on 27/01/17.
//

#include <cctype>
#include <cstdlib>
#include "Checker.h"



Checker::Checker(char *driver) : driver(driver) {
    this->id = driver[0];
    this->age = driver[1];
    this->status = driver[2];
    this->exp = driver[3];
    this->cabId = driver[4];





}

bool Checker::CheckClientDriver() {
    if (isdigit(this->id) && atoi((const char *) this->id) >= 0) {
        if (isdigit(this->age) && atoi((const char *) this->age) >= 0) {
            bool isEqual = false;
            char statuses [4] = {'S','M','W','D'};
            for (int i = 0; i < 4; ++i) {
                if (this->status == statuses[i]) {
                    isEqual = true;
                    break;
                }
            }
            if (isEqual) {
                if (isdigit(this->exp) && atoi((const char *) this->exp) >= 0) {
                    return isdigit(this->cabId) && atoi((const char *) this->cabId) >= 0;
                } else {
                    return false;
                }

            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

