template <typename T>  
object::T_pointer<T>::T_pointer(T value) {
    this->value = value; 
}

template <typename T>  
const std::type_info& object::T_pointer<T>::type() const {
    return typeid(T);
}

template <typename T>  
object::object_pointer*  object::T_pointer<T>::copy() const {
    object_pointer* new_pointer = new T_pointer<T>(this->value);
    return new_pointer;
}

template <typename T>
object::object(const T& value) {
    obj_ptr = new T_pointer<T>(value);
}

object::~object() {
    delete obj_ptr;
}


object& object::operator=(const object& other) {
    if(this!=&other) {
        delete obj_ptr;
        obj_ptr = other.obj_ptr->copy();
    }
    return *this;
}

object::object(const object& other) {
    obj_ptr = nullptr;
    *this = other;
}

const std::type_info& object::type() const {
    return obj_ptr->type();
}

template <typename T>
T object::cast_back() const {
    if(this->type() == typeid(T)) {
        return dynamic_cast<T_pointer<T>*>(this->obj_ptr)->value;
    }
    else {
        throw std::runtime_error("Object casting failed!");
    }
}

// object object::operator+(const object& other) const {
//     if(this->type()== other.type()) {
//         return object();
//     }
//     else {
//         throw std::runtime_error("Types needed to be the same when performing +");
//     }
// }

// bool object::operator==(const object& other) const {

// }

// std::ostream& operator<<(std::ostream &os, const object& obj) {

// }