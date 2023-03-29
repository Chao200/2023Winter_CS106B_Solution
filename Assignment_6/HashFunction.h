#ifndef Hashes_Included
#define Hashes_Included

#include <functional>
#include "hashcode.h"
#include "error.h"
#include "collections.h"

class HashProvider;

template <typename T> class HashFunction {
public:
    /**
     * Constructs a new hash function for the specified number of slots.
     * For example:
     *
     *    HashFunction<string> hashFn = forSize(137); // 137 slots
     */
    HashFunction(const HashProvider& provider);

    /**
     * Constructs a new HashFunction. This HashFunction cannot be used because
     * it won't have been initialized with a number of buckets, and trying to
     * use it will cause a runtime error.
     *
     * You shouldn't directly use this constructor; it's only here so that
     * you can declare variables of type HashFunction and initialize them
     * later.
     */
    HashFunction();

    /**
     * Constructs a hash function that specifically uses the underlying raw
     * hash code as its hash function. This is useful if you want to guarantee
     * predictable values for your hash function when testing.
     */
    static HashFunction wrap(int numSlots,
                             std::function<int (const T &)> hashFn);

    /**
     * Returns the number of slots this hash function is designed to operate
     * over.
     */
    int numSlots() const;

    /**
     * Applies the hash function to the specified argument. The syntax for
     * using this function is
     *
     *     hashFn(argument)
     *
     * That is, you'll treat the variable of type HashFunction as though it's
     * an honest-to-goodness function rather than a variable of some type.
     */
    int operator() (const T& argument) const;

private:
    std::function<int(const T&)> callback;
    int mNumSlots;

    static_assert(stanfordcpplib::collections::IsHashable<T>::value,
                  "Oops! You've tried to make a HashFunction for a type that isn't hashable. "
                  "Double-click this error message for more details.");

    /*
     * Hello CS106 students! If you got directed to this line of code in a compiler error,
     * it probably means that you tried making a HashFunction<T> with a custom struct or
     * class type.
     *
     * In order to have a HashFunction<T> for a type T, the type T needs to have a hashCode
     * function defined and be capable of being compared using the == operator. If you were
     * directed here, one of those two conditions wasn't met.
     *
     * There are two ways to fix this. The first option would simply be to not use your custom
     * type in conjunction with HashFunction<T>. This is probably the easiest option.
     *
     * The second way to fix this is to explicitly define a hashCode() and operator== function
     * for your type. To do so, first define hashCode as follows:
     *
     *     int hashCode(const YourCustomType& obj) {
     *         return hashCode(obj.data1, obj.data2, ..., obj.dataN);
     *     }
     *
     * where data1, data2, ... dataN are the data members of your type. For example, if you had
     * a custom type
     *
     *     struct MyType {
     *         int myInt;
     *         string myString;
     *     };
     *
     * you would define the function
     *
     *     int hashCode(const MyType& obj) {
     *         return hashCode(obj.myInt, obj.myString);
     *     }
     *
     * Second, define operator== as follows:
     *
     *     bool operator== (const YourCustomType& lhs, const YourCustomType& rhs) {
     *         return lhs.data1 == rhs.data1 &&
     *                lhs.data2 == rhs.data2 &&
     *                         ...
     *                lhs.dataN == rhs.dataN;
     *     }
     *
     * Using the MyType example from above, we'd write
     *
     *     bool operator== (const MyType& lhs, const MyType& rhs) {
     *         return lhs.myInt == rhs.myInt && lhs.myString == rhs.myString;
     *     }
     *
     * Hope this helps!
     */
};

class HashProvider {
private:
    int numSlots;
    std::function<int(int)> coreHash;

    friend HashProvider forSize(int numSlots);
    template <typename T> friend class HashFunction;
};

HashProvider forSize(int numSlots);

/* * * * * Implementation Below This Point * * * * */
template <typename T>
HashFunction<T>::HashFunction(const HashProvider& provider) {
    /* Copy these to locals so we don't have dangling references
     * if we copy the HashFunction object.
     */
    auto scrambler = provider.coreHash;
    int numSlots = provider.numSlots;

    mNumSlots = provider.numSlots;
    callback = [=](const T& key) {
        return (scrambler(hashCode(key)) & 0x7FFFFFFF) % numSlots;
    };
}

template <typename T>
HashFunction<T> HashFunction<T>::wrap(int numSlots,
                                      std::function<int (const T&)> hashFn) {
    if (numSlots <= 0) {
        error("HashFunction<T>::wrap(): numSlots must be positive.");
    }

    HashFunction result;
    result.callback = [hashFn, numSlots] (const T& key) {
        return (0x7FFFFFFF & hashFn(key)) % numSlots;
    };
    result.mNumSlots = numSlots;

    return result;
}

template <typename T> int HashFunction<T>::numSlots() const {
    return mNumSlots;
}

/* Default constructor sets up a hash function that always reports an error. */
template <typename T> HashFunction<T>::HashFunction() {
    callback = [](const T&) -> int {
        error("Attempted to use an uninitialized HashFunction object.");
    };
    mNumSlots = 0;
}

/* Call operator forwards to the callback. */
template <typename T> int HashFunction<T>::operator()(const T& arg) const {
    return callback(arg);
}

#endif
