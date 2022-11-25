template <typename T>
object::T_pointer<T>::T_pointer(const T& value) : value(value) {}

template <typename T>
const std::type_info& object::T_pointer<T>::type() const {
    return typeid(T);
}

template <typename T>
object::object_pointer* object::T_pointer<T>::copy() const {
    return new T_pointer<T>(this->value);
}

template <typename T>
object::object(const T& value) {
    this->obj_ptr = new T_pointer<T>(value);
}

object::object(const object& other) {
    this->obj_ptr = other.obj_ptr->copy();
}

object::~object() {
    delete this->obj_ptr;
}

object& object::operator=(const object& other) {
    if (this != &other) {
        delete this->obj_ptr;
        this->obj_ptr = other.obj_ptr->copy();
    }
    return *this;
}

const std::type_info& object::type() const {
    return this->obj_ptr->type();
}

template <typename T>
T object::cast_back() const {
    if (this->type() != typeid(T))
        throw std::runtime_error("Object casting failed!");
    return dynamic_cast<T_pointer<T>*>(this->obj_ptr)->value;
}

template <typename T>
object object::T_pointer<T>::operator+(const object& other) const {
    if (other.type() != typeid(T))
        throw std::runtime_error("Types needed to be the same when performing +");
    return this->value + other.cast_back<T>();
}

object object::operator+(const object& other) const {
    return *obj_ptr + other;
}

template <typename T>
bool object::T_pointer<T>::operator==(const object& other) const {
    if (other.type() != typeid(T))
        return false;
    return this->value == other.cast_back<T>();
}

bool object::operator==(const object& other) const {
    return *obj_ptr == other;
}

template <typename T>
std::ostream& object::T_pointer<T>::print(std::ostream& os) const {
    return os << this->value;
}

std::ostream& operator<<(std::ostream& os, const object& obj) {
    return obj.obj_ptr->print(os);
}