#ifndef SEP_API_API_EXCEPTION_H
#define SEP_API_API_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace sep::api {

/**
 * @brief Custom exception class for API client errors.
 */
class APIException : public std::runtime_error {
public:
    /**
     * @brief Constructs an APIException.
     * @param message The error message.
     * @param transient True if the error is considered transient and might succeed on retry, false otherwise.
     */
    APIException(const std::string& message, bool transient = false)
        : std::runtime_error(message), transient_(transient) {}

    /**
     * @brief Checks if the error is transient.
     * @return True if the error is transient, false otherwise.
     */
    bool isTransient() const noexcept {
        return transient_;
    }

private:
    bool transient_;
};

} // namespace sep::api

#endif // SEP_API_API_EXCEPTION_H
