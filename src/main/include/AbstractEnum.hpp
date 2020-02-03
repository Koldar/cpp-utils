#ifndef _CPPUTILS_ABSTRACTENUM_HEADER__
#define _CPPUTILS_ABSTRACTENUM_HEADER__

#include <vector>
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
     *  struct MyEnum: public AbstractEnum<MyEnum> {
     *  private:
     *      static std::vector<const AbstractEnum<MyEnum>*> VALUES;
     *  private:
     *      MyEnum(const std::string& name): AbstractEnum{name, VALUES} {
     *      }
     *  public:
     *      static const MyEnum OK;
     *      static const MyEnum KO;
     *      static const MyEnum BOH;
     *  };
     * 
     *  //cpp file
     *  std::vector<const AbstractEnum<MyEnum>*> MyEnum::VALUES{};
     *  const MyEnum MyEnum::OK = MyEnum{"OK"};
     *  const MyEnum MyEnum::KO = MyEnum{"KO"};
     *  const MyEnum MyEnum::BOH = MyEnum{"BOH"};
     * @endcode
     * 
     * You can now use the enumeration as you like
     * 
     */
    template <typename CHILD>
    class AbstractEnum {
    public:
        using This = AbstractEnum;
    private:
        const std::vector<const AbstractEnum<CHILD>*>& values;
        int index;
        std::string name;
    protected:
        AbstractEnum(const std::string& name, std::vector<const AbstractEnum<CHILD>*>& values): index{AbstractEnum::getNextId(values)}, name{name}, values{values} {
            values.push_back(this);
        }
    public:
        virtual ~AbstractEnum() {

        }

        AbstractEnum(const This& o): index{o.index}, name{o.name}, values{o.values} {

        }
        
        AbstractEnum(This&& o): index{o.index}, name{o.name}, values{o.values} {
        }

        This& operator =(const This& o) {
            this->index = o.index;
            this->name = o.name;
            this->values = o.values;
            return *this;
        }

        This& operator = (This&& o) {
            this->index = o.index;
            this->name = o.name;
            this->values = o.values;
            return *this;
        }
    private:
        static int getNextId(const std::vector<const AbstractEnum<CHILD>*>& values) {
            return values.size();
        }
    public:
        int getIndex() const {
            return this->index;
        }

        std::string getName() const {
            return this->name;
        }

        const CHILD& getFirst() const {
            return this->values[0];
        }

        const CHILD& getValues() const {
            return this->values;
        }
    public:
        friend bool operator ==(const This& a, const This& b) {
            return a.index == b.index;
        }

        friend bool operator !=(const This& a, const This& b) {
            return a.index != b.index;
        }

        friend std::ostream& operator <<(std::ostream& out, const This& a) {
            out << a.name;
            return out;
        }

    };

}

#endif