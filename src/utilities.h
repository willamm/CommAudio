#ifndef UTILITIES_H
#define UTILITIES_H

class QComboBox;
// Utility class
class Utilities
{
public:
    Utilities() = delete;
    Utilities(const Utilities&) = delete;
    Utilities& operator=(const Utilities&) = delete;
    Utilities(Utilities&&) = delete;
    ~Utilities() = delete;

    static void fillComboBox(QComboBox*);
};

#endif // UTILITIES_H
