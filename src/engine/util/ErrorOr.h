#ifndef UI_FRAMEWORK_ERROROR_H
#define UI_FRAMEWORK_ERROROR_H

#include <string>

//This store the error message
struct Error {
    Error(std::string error_message) : m_error_message(std::move(error_message)) {}     //Don't use std::move() ?
    std::string m_error_message;
};

//TODO : maybe this should be redone in a better way ? idk it does the job for now
template <typename T>
class ErrorOr {
public:
    // Copy constructor
    ErrorOr(const ErrorOr &other) {
        if (other.m_is_error) {
            m_is_error = true;
            m_data.error = new Error(*other.GetError());
        } else {
            m_is_error = false;
            m_data.value = new T(other.GetValue());
        }
    }

    // Initialize with value
    ErrorOr(const T &val) {
        m_is_error = false;
        m_data.value = new T(val);
    }

    // Initialize with error
    ErrorOr(const Error &err) {
        m_is_error = true;
        m_data.error = new Error(err);
    }

    // Destructor
    ~ErrorOr() {
        if (m_is_error) delete m_data.error;
        else delete m_data.value;
    }

    // Get the error (nullptr if there is no error)
    Error *GetError() const {
        if (!m_is_error) return nullptr;
        return m_data.error;
    }

    // Get the value
    T GetValue() const {
        if (m_is_error) return T(0);
        return *(m_data.value);
    }

    // Assignment operator
    ErrorOr &operator=(const ErrorOr &other) {
        if (this == &other) return *this;       //Don't do anything in this case

        //Deallocate currently stored data
        if (m_is_error) {
            delete m_data.error;
        } else {
            delete m_data.value;
        }

        //Get the new data from the other error
        if (other.m_is_error) {
            m_data.error = new Error(*other.GetError());
        } else {
            m_data.value = new T(*other.GetValue());
        }
        m_is_error = other.m_is_error;

        return *this;
    }

private:
    union Data {
        Error *error;
        T *value;
    } m_data;
    bool m_is_error;
};

#endif // UI_FRAMEWORK_ERROROR_H
