#ifndef _CPPUTILS_ABSTRACTENUM_HEADER__
#define _CPPUTILS_ABSTRACTENUM_HEADER__

#include <string>

namespace cpp_utils {

    /**
     * @brief allows you to generate a class representing a Enumeration, like in Java
     * 
     * This can be used insterad fo `enum class`es, since those classes don't allow you to add methods to the  instances of the enum.
     * 
     * Example
     * =======
     * 
     * @code
     *  //header file
     * 
     *  struct MyEnum: public AbstractEnum {
     *   private:
     *       MyEnum(const std::string& name): AbstractEnum{name} {
     * 
     *       }
     *  public:
     *      static const MyEnum OK;
     *      static const MyEnum KO;
     *      static const MyEnum BOH;
     *  };
     * 
     *  //cpp file
     *  const MyEnum MyEnum::OK = MyEnum{"OK"};
     *  const MyEnum MyEnum::KO = MyEnum{"KO"};
     *  const MyEnum MyEnum::BOH = MyEnum{"BOH"};
     * @endcode
     * 
     * You can now use the enumeration as you like
     * 
     */
    class AbstractEnum {
    public:
        typedef AbstractEnum This;
    private:
        int index;
        std::string name;
    protected:
        AbstractEnum(const std::string& name);
    public:
        virtual ~AbstractEnum();
        AbstractEnum(const This& o);
        AbstractEnum(This&& o);
        This& operator =(const This& o);
        This& operator = (This&& o);
    private:
        static int getNextId();
    public:
        int getIndex() const;
        std::string getName() const;
    public:
        friend bool operator ==(const This& a, const This& b);
        friend bool operator !=(const This& a, const This& b);
        friend std::ostream& operator <<(std::ostream& out, const This& a);

    };

}

#endif