//
// Created by ido on 27/01/17.
//

#ifndef EX2TESTS_CLIENTCHECKER_H
#define EX2TESTS_CLIENTCHECKER_H


class Checker {
public:
    Checker(int id, int age, char status, int exp, int cabId);

    Checker(char *driver);

private:
    char id;
    char age;
    char status;
    char exp;
    char cabId;
    char driver [5];
public:

    bool CheckClientDriver();
};


#endif //EX2TESTS_CLIENTCHECKER_H
