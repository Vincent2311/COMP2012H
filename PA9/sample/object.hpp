#include <typeinfo>
#include <iostream>

class object {
    public:
        //Complete the missing functions declarations
        struct object_pointer {
            virtual ~object_pointer() = default;
            virtual const std::type_info& type() const = 0;
            virtual object_pointer* copy() const = 0;

            virtual object operator+(const object& other) const = 0;
            virtual bool operator==(const object& other) const = 0;
            virtual std::ostream& print(std::ostream& os) const = 0;
        };

        //Complete the class definition
        template <typename T>
        struct T_pointer : public object_pointer {
            T value;
            T_pointer(const T& value);
            const std::type_info& type() const;
            object_pointer* copy() const;

            object operator+(const object& other) const;
            bool operator==(const object& other) const;
            std::ostream& print(std::ostream& os) const;
        };

    private:
        object_pointer* obj_ptr;

    public:
    
        //Constructor
        template <typename T>
        object(const T& value);

        //Copy constructor
        object(const object& other);

        ~object();

        object& operator=(const object& other);

        //Return the type that `object_pointer` is really storing
        const std::type_info& type() const;

        template <typename T>
        T cast_back() const;

        //Default constructor defaults a value of int 0
        object() : object(0) {}

        //Bonus part
        object operator+(const object& other) const;
        bool operator==(const object& other) const;
        friend std::ostream& operator<<(std::ostream &os, const object& obj);
};

#include "object.tpp"
