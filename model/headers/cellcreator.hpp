#ifndef CELLCREATOR_HPP
#define CELLCREATOR_HPP

template<typename T> class CellCreator
{
public:
    T* operator ()()
    {
        if(cell == nullptr) {
            return cell = new T();
        } else {
            return cell;
        }
    }

    T* cell = nullptr;
};

#endif // CELLCREATOR_HPP
